import { Driver } from "./driver";
import { wasm_load } from "./wasm";

let image_select = document.getElementById('image-select') as HTMLSelectElement;

let driver: Driver;
async function main() {
    const module = await wasm_load('wasm/editor.wasm', 8);
    const canvas = document.getElementById('view') as HTMLCanvasElement;
    driver = new Driver(canvas);
    driver.load_module(module);
    console.log(driver);
    console.log(`game wasm kernel version ${driver.version!()}`);

    (window as any).driver = driver;
    tick();

    image_select.onchange = function () {
        wasm_load('wasm/' + image_select.value, 8).then(function (module) {
            driver.load_module(module);
        });
    }
}

let loop_flag = -1;
let last_time = performance.now();
function tick() {
    let time = performance.now();
    let delta_time = time - last_time;
    last_time = time;

    driver.tick(delta_time * 0.001); // ms to s
    loop_flag = requestAnimationFrame(tick);
}

function pause() {
    cancelAnimationFrame(loop_flag);
    loop_flag = -1;
}

main().then();