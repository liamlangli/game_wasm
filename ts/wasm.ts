export async function fetch_arraybuffer(uri: string): Promise<ArrayBuffer> {
    return new Promise(function (resolve) {
        fetch(uri).then(function (response) {
            response.arrayBuffer().then(function (buffer) {
                resolve(buffer);
            });
        });
    });
}

export interface WebAssemblyModule {
    source: WebAssembly.WebAssemblyInstantiatedSource;
    memory: WebAssembly.Memory;
}

export async function wasm_load(url: string, page_count: number = 2): Promise<WebAssemblyModule> {
    const code = await fetch_arraybuffer(url);
    const memory = new WebAssembly.Memory({ initial: page_count });
    const source = await WebAssembly.instantiate(code, { env: { memory } });
    return { source, memory };
}