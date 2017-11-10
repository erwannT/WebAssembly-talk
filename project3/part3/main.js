window.onload = () => {

    Module.print("Page Loaded")

    Module.postRun = () => {

        Module.print("Module loaded")

        Module.print("Passage de tableau")

        const myArray = [300, 12, 13];        

        var result = Module.ccall(
            'logArrayOfInt',     // name of C
            'number',
            ['array', 'number'],
            [myArray, myArray.length])        


        Module.print("Passage par tableau avancées")

        var myArray2 = new Int32Array([22, 44, 66, 999])

        // taille en memoire necessaire
        const sizeArray = myArray2.length * myArray2.BYTES_PER_ELEMENT;
        // allocation d'espace
        const ptrArray = Module._malloc(sizeArray);
        // creer un point d'entree en memoire
        const dataHeap = new Uint8Array(Module.HEAPU8.buffer, ptrArray, sizeArray);
        // assigne le tableau        
        dataHeap.set(new Uint8Array(myArray2.buffer));

        var result = Module.ccall(
            'logArrayOfInt32',     // name of C
            'number',
            ['number', 'number'],
            [dataHeap.byteOffset, myArray2.length])

        
        Module.print("Passage par tableau avancées 2")

        // taille en memoire necessaire
        const sizeArray2 = myArray2.length * myArray2.BYTES_PER_ELEMENT;
        // allocation d'espace
        const ptrArray2 = Module._malloc(sizeArray2);
        // creer un point d'entree en memoire
        const dataHeap2 = new Int32Array(Module.HEAP32.buffer, ptrArray2, sizeArray2);
        // assigne le tableau        
        dataHeap2.set(myArray2.buffer);

        var result = Module.ccall(
            'logArrayOfInt32',     // name of C
            'number',
            ['number', 'number'],
            [dataHeap.byteOffset, myArray2.length])

    }


}