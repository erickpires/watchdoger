var watchdoger = require('./lib/watchdoger');

let t1 = new Date();

function deu_merda_callback() {
    let t2 = new Date();
    console.log('Eita, deu merda! Passou: ' + (t2 - t1) + 'ms');
}

// set the limit of execution time to 2000 milliseconds
watchdoger.setShouldThrowException(false);
watchdoger.setTimeoutCallback(deu_merda_callback);
watchdoger.resetWatchdoger(5000);

// execute code that will block the event loop for longer
while(!false);

console.log('Not reached');

