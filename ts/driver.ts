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

export class Driver {
    memory: WebAssembly.Memory;
    heap_base: number;

    video_memory: Uint16Array;

    width: number;
    height: number;

    private ctx: CanvasRenderingContext2D;
    private image_data: ImageData;

    private button: number = 0;
    private mouse_x: number = 0;
    private mouse_y: number = 0;

    constructor(public module: WebAssemblyModule, public canvas: HTMLCanvasElement) {
        this.memory = module.memory;
        const exports = module.source.instance.exports;
        this.heap_base = (exports.__heap_base as WebAssembly.Global).value;

        this.width = (exports.width as Function)() as number;
        this.height = (exports.height as Function)() as number;
        canvas.width = this.width;
        canvas.height = this.height;

        const video_memory_base = (exports.video_memory_address as Function)();
        this.video_memory = new Uint16Array(this.memory.buffer, video_memory_base, this.width * this.height);

        this.version = exports.version as any;
        this.update_mouse = exports.update_mouse as any;
        this.update_button = exports.update_button as any;
        this.update = exports.update as any;

        (exports.init as Function)();
        (exports.start as Function)();

        // prepare display
        this.ctx = canvas.getContext('2d')!;
        this.image_data = this.ctx.createImageData(this.width, this.height);
        this.resize(this.width, this.height);

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
            this.mouse_x = event.clientX - rect.left;
            this.mouse_y = event.clientY - rect.top;
        })
    }

    version: () => number;
    private update: (delta_time: number) => void;
    private update_mouse: (mosue_x: number, mouse_y: number) => void;
    private update_button: (button: number) => void;

    private flush() {
        const data = this.image_data.data;
        for (let y = 0; y < this.height; y++) {
            for (let x = 0; x < this.width; x++) {
                const index = y * this.width + x;
                const color = this.video_memory[index];
                const r = ((color >> 10) & 0x1f);
                const g = (color >> 5) & 0x1f;
                const b = color & 0x1f;
                const i = index * 4;
                data[i + 0] = r << 3;
                data[i + 1] = g << 3;
                data[i + 2] = b << 3;
                data[i + 3] = 255;
            }
        }
        this.ctx.putImageData(this.image_data, 0, 0);
    }

    tick(delta_time: number): void {
        this.update_mouse(this.mouse_x, this.mouse_y);
        this.update_button(this.button);
        this.update(delta_time);
        this.flush();
    }

    resize(width: number, height: number) {
        this.canvas.style.width = width * 2 + 'px';
        this.canvas.style.height = height * 2 + 'px';
        this.canvas.style.borderRadius = '6px';
    }
}