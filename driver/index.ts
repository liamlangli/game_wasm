import { Driver } from "./driver";
import { wasm_load } from "./wasm";

let driver: Driver;
async function main() {
    const module = await wasm_load('wasm/editor.wasm', 8);
    const canvas = document.getElementById('view') as HTMLCanvasElement;
    driver = new Driver(module, canvas);
    console.log(driver);
    console.log(`game wasm kernel version ${driver.version()}`);

    (window as any).driver = driver;
    tick();
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