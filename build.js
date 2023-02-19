import * as esbuild from 'esbuild';

// Start esbuild's server on a random local port
let ctx = await esbuild.context({
    entryPoints: ['driver/index.ts'],
    outdir: 'public',
    bundle: true,
    sourcemap: true,
})

// The return value tells us where esbuild's local server is
let { host, port } = await ctx.serve({ servedir: 'public', port: 3000 });
console.log(`listening on http://${host}:${port}`)
