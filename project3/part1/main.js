window.onload = () => {

    Module.print("Page Loaded")

    Module.postRun = () => {
       
        Module.print("Module loaded")

        Module.print("Appel de fonction")
        var result = Module.ccall(
            'add',
            'number',
            ['number', 'number'],
            [1, 1]);
        Module.print("Le resultat est " + result)

        Module.print("Le resultat est " + _add(12, 25))
        Module.print("Le resultat est " + _addFloat(12.2, 25.3))


        Module.print("Appel de fonction via a wrap function")
        var add = Module.cwrap(
            'add',
            'number',
            ['number', 'number']);

        Module.print("Le resultat est "+ add(25, 12));
    }
    

}