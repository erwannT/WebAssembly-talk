var em_module = require('./module.js');

em_module.postRun = () => {

    console.log("Module loaded")

    em_module._sayHi(); // direct calling works
    em_module.ccall("sayHi"); // using ccall etc. also work
    console.log(em_module._daysInWeek());

}