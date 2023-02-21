import { Keycode } from "./keycode";
import { WebAssemblyModule } from "./wasm";

export enum DriverButton {
    UP = 1,
    DOWN = 2,
    LEFT = 4,
    RIGHT = 8,
    A = 16,
    B = 32,
    X = 64,
    Y = 128,
    L = 256,
    R = 512,
    START = 1024,
    SELECT = 2048,
    FN = 4096,
};

function clamp(value: number, min: number, max: number) {
    return Math.max(min, Math.min(max, value));
}

const vertex_shader = `#version 300 es
precision highp float;

layout(location = 0) in vec3 position;

out vec2 v_uv;

void main()
{
    v_uv = (position.xy + 1.0) * 0.5;
    gl_Position = vec4(position, 1.0);
}
`

const fragment_shader = `#version 300 es
precision highp float;
precision highp int;
precision highp usampler2D;

uniform vec2 u_size;
uniform usampler2D u_texture;

in vec2 v_uv;

out vec4 frag_data;

void main() {
    uint packed = texelFetch(u_texture, ivec2(v_uv * u_size), 0).r;
    float r = float((packed >> 24) & 0xffu) / 255.0;
    float g = float((packed >> 16) & 0xffu) / 255.0;
    float b = float((packed >> 8) & 0xffu) / 255.0;
    frag_data = vec4(r, g, b, 1.0);
}
`;

export class Driver {
    private module?: WebAssemblyModule;
    memory?: WebAssembly.Memory;

    heap_base?: number;
    global_base?: number;

    video_memory?: Uint32Array;

    width: number = 1;
    height: number = 1;

    // private ctx: CanvasRenderingContext2D;

    private gl: WebGL2RenderingContext;
    private texture?: WebGLTexture;
    private program: WebGLProgram;
    private size_location?: WebGLUniformLocation;
    private size_uniform = new Float32Array([1, 1]);

    private button: number = 0;
    private mouse_button: number = 0;

    private mouse_x: number = 0;
    private mouse_y: number = 0;

    private scale_x: number = 1;
    private scale_y: number = 1;

    constructor(public canvas: HTMLCanvasElement) {
        // prepare display
        const options = { antialias: false } as WebGLContextAttributes;
        const gl = canvas.getContext('webgl2', options)!;
        if (gl == null) throw new Error('webgl2 not supported');
        this.gl = gl;
        this.program = this.create_program();

        window.addEventListener('keydown', (event: KeyboardEvent) => {
            const keycode = event.keyCode;
            if (keycode === Keycode.Up || keycode === Keycode.w) this.button |= DriverButton.UP;
            if (keycode === Keycode.Down || keycode === Keycode.s) this.button |= DriverButton.DOWN;
            if (keycode === Keycode.Left || keycode === Keycode.a) this.button |= DriverButton.LEFT;
            if (keycode === Keycode.Right || keycode === Keycode.d) this.button |= DriverButton.RIGHT;
        });

        window.addEventListener('keyup', (event: KeyboardEvent) => {
            const keycode = event.keyCode;
            if (keycode === Keycode.Up || keycode === Keycode.w) this.button &= ~DriverButton.UP;
            if (keycode === Keycode.Down || keycode === Keycode.s) this.button &= ~DriverButton.DOWN;
            if (keycode === Keycode.Left || keycode === Keycode.a) this.button &= ~DriverButton.LEFT;
            if (keycode === Keycode.Right || keycode === Keycode.d) this.button &= ~DriverButton.RIGHT;
        });

        window.addEventListener('mousemove', (event: MouseEvent) => {
            const rect = canvas.getBoundingClientRect();
            this.mouse_x = clamp((event.clientX - rect.left) / this.scale_x, 0, this.width);
            this.mouse_y = clamp((event.clientY - rect.top) / this.scale_y, 0, this.height);
        });

        window.addEventListener('mousedown', (event: MouseEvent) => {
            this.mouse_button |= 1 << event.button;
        });

        window.addEventListener('mouseup', (event: MouseEvent) => {
            this.mouse_button &= ~(1 << event.button);
        });
    }

    version?: () => number;
    private update?: (delta_time: number) => void;
    private update_mouse?: (mosue_x: number, mouse_y: number) => void;
    private update_button?: (button: number, mouse_button: number) => void;

    private create_texture(): WebGLTexture {
        const gl = this.gl;
        if (this.texture) {
            gl.deleteTexture(this.texture);
        }

        const texture = gl.createTexture()!;
        const texture_type = gl.TEXTURE_2D;
        gl.bindTexture(texture_type, texture);

        gl.pixelStorei(gl.UNPACK_ALIGNMENT, 4);
        gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, 1);
        gl.pixelStorei(gl.UNPACK_PREMULTIPLY_ALPHA_WEBGL, 0);
    
        gl.texParameteri(texture_type, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
        gl.texParameteri(texture_type, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
        gl.texParameteri(texture_type, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
        gl.texParameteri(texture_type, gl.TEXTURE_MAG_FILTER, gl.NEAREST);

        gl.texImage2D(texture_type, 0, gl.R32UI, this.width, this.height, 0, gl.RED_INTEGER, gl.UNSIGNED_INT, this.video_memory!);
        return texture;
    }

    private create_program(): WebGLProgram {
        const gl = this.gl;
        const program = gl.createProgram()!;
        
        gl.attachShader(program, build_shader(gl, vertex_shader, gl.VERTEX_SHADER));
        gl.attachShader(program, build_shader(gl, fragment_shader, gl.FRAGMENT_SHADER));
        gl.linkProgram(program);

        this.size_location = gl.getUniformLocation(program, 'u_size')!;

        const vertex = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, vertex);
        gl.vertexAttribPointer(0, 3, gl.FLOAT, false, 0, 0);
        gl.enableVertexAttribArray(0);
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array([-1, -1, 0, 3, -1, 0, -1, 3, 0]), gl.STATIC_DRAW);
        return program;
    }

    private update_texture() {
        const gl = this.gl;
        gl.activeTexture(gl.TEXTURE0);
        gl.bindTexture(gl.TEXTURE_2D, this.texture!);
        gl.texSubImage2D(gl.TEXTURE_2D, 0, 0, 0, this.width, this.height, gl.RED_INTEGER, gl.UNSIGNED_INT, this.video_memory!);

        gl.useProgram(this.program);
        gl.uniform2fv(this.size_location!, this.size_uniform);

        gl.drawArrays(gl.TRIANGLES, 0, 3);
    }

    tick(delta_time: number): void {
        if (!this.module || !this.texture) return;
        this.update_texture();
        this.update_mouse!(this.mouse_x, this.mouse_y);
        this.update_button!(this.button, this.mouse_button);
        this.update!(delta_time);
    }

    resize(width: number, height: number) {
        this.canvas.style.width = width + 'px';
        this.canvas.style.height = height + 'px';
        this.canvas.style.borderRadius = '6px';
        this.scale_x = width / this.width;
        this.scale_y = height / this.height;
        this.gl.viewport(0, 0, this.canvas.width, this.canvas.height);
    }

    load_module(module: WebAssemblyModule) {
        this.module = module;
        this.memory = module.memory;
        const exports = module.source.instance.exports;
        this.heap_base = (exports.__heap_base as WebAssembly.Global).value;
        this.global_base = (exports.__global_base as WebAssembly.Global).value;

        this.width = (exports.width as Function)() as number;
        this.height = (exports.height as Function)() as number;

        const pixel_ratio = window.devicePixelRatio;
        this.canvas.width = this.width * pixel_ratio;
        this.canvas.height = this.height * pixel_ratio;
        this.size_uniform[0] = this.width;
        this.size_uniform[1] = this.height;

        this.video_memory = new Uint32Array(this.memory.buffer, this.heap_base, this.width * this.height);

        this.version = exports.version as any;
        this.update_mouse = exports.update_mouse as any;
        this.update_button = exports.update_button as any;
        this.update = exports.update as any;

        (exports.init as Function)(this.heap_base);
        (exports.start as Function)();

        console.log(`video memory address ${(exports.video_memory_address as Function)()}`);

        this.texture = this.create_texture();
        this.resize(this.width * 2, this.height * 2);
    }
}

export function count_decimal_bit(n: number): number {
    let c = 1;
    while (Math.abs(n) >= 10) {
        n /= 10;
        c++;
    }
    return c;
}

function build_shader(gl: WebGL2RenderingContext, source: string, type: any): WebGLShader {
    const shader = gl.createShader(type)!;
    gl.shaderSource(shader, source);
    gl.compileShader(shader);
    const shaderInfo = gl.getShaderInfoLog(shader);
    if (shaderInfo != '') {
        const lines = source.split('\n');
        const line_count = lines.length;
        const max_bit = count_decimal_bit(line_count);
        console.log(
            lines
                .map((l, i) => {
                    return `${' '.repeat(max_bit - count_decimal_bit(i + 1))}${i + 1}|${l}`;
                })
                .join('\n')
        );
        console.warn(`shader error info:\n${shaderInfo}`);
    }
    return shader;
}