window.onload = () => {

    Module.print("Page Loaded")

    Module.postRun = () => {

        Module.print("Module loaded")
        
        Module.print("Run Utilisation simpliste")
        const ret = _info("Utilisation simpliste")
        Module.print("Valeur retourné : " + ret);

        Module.print("Gestion des pointeurs manuels")
        const ptr = Module.allocate(Module.intArrayFromString("Gestion des pointeurs manuels"), 'i8', ALLOC_NORMAL);
        const retPtr = _info(ptr);
        Module.print("Valeur retourné : " + Pointer_stringify(retPtr))

        Module.print("Gestion des pointeurs via ccall")
        var result = Module.ccall(
            'info',
            'string',
            ['string'],
            ['Gestion des pointeurs via ccall']

        );
        Module.print("Valeur retourné : " + result)

    }


}