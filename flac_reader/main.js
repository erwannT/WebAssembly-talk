window.onload = () => {

    Module.print("Page Loaded")

    Module.postRun = () => {

        var source;

        document.getElementById("file").addEventListener("change", function (event) {
            var myFile = event.target.files[0];

            if (myFile) {
                var reader = new FileReader();
                reader.onload = function (e) {
                    var arrayBuffer = reader.result;
                    FS.writeFile('sample.flac', new Uint8Array(arrayBuffer), { encoding: 'binary' });

                    source = _play();

                    console.log("Current source : " + source);
                }

                reader.readAsArrayBuffer(myFile);

            }
        });

        document.getElementById("play").addEventListener("click", function () {
            _playSource(source);
        });


        document.getElementById("pause").addEventListener("click", function () {

            _pauseSource(source);

        });

        document.getElementById("stop").addEventListener("click", function () {

            _stopSource(source);
        });

    }


}