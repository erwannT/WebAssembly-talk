console.log("Index.js loaded")

console.log("Web assembly loading")

const importObject = {
    imports: {
        imported_func: function (arg) {
            console.log(arg);
        }
    },
    env: {
        // memoryBase: 0,
        // tableBase: 0,
        memory: new WebAssembly.Memory({ initial: 256 }),
        table: new WebAssembly.Table({ initial: 0, element: 'anyfunc' })
    }
};

fetch('module.wasm')
    .then(
    response => response.arrayBuffer()
    )
    .then(
    bytes => WebAssembly.instantiate(bytes, importObject)
    ).then(results => {
        results.instance.exports.exported_func();
    });
