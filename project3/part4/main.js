window.onload = () => {

    Module.print("Page Loaded")

    Module.postRun = () => {

        Module.print("Module loaded")

        _alert();
        _alert2();
        
    }


}