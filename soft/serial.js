const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline');
const simConnect = require('msfs-simconnect-nodejs');

const port = new SerialPort('COM8', { baudRate: 115200 });
const parser = port.pipe(new Readline({ delimiter: '\n' }));// Read the port data

port.on("open", () => {
    console.log('serial port open');
});

parser.on('data', data => {
    console.log('got word from arduino:', data);
});

setInterval(() => {
    port.write('d1515;98;959.85;1894512;378.748;135489;164821.5451;\n', (err) => {
        if (err) {
            return console.log('Error on write: ', err.message);
        }
        //  console.log('message written');
    });
}, 1000);

var success = simConnect.open("MyAppName",
    (name, version) => {
        //connectedCallback
        console.log("Connected to: " + name + "\nSimConnect version: " + version);
        // Safe to start interacting with SimConnect here (request data, etc)
    }, () => {
        //simExitedCallback
        console.log("Simulator exited by user");
    }, (exception) => {
        //exceptionCallback
        console.log("SimConnect exception: " + exception.name + " (" + exception.dwException + ", " + exception.dwSendID + ", " + exception.dwIndex + ", " + exception.cbData + ")");
    }, (error) => {
        //errorCallback
        console.log("Undexpected disconnect/error: " + error); // Look up error code in ntstatus.h for details
    });
/*
simConnect.requestDataOnSimObject(
    //reqData
    [
        ["PLANE LATITUDE", "degrees"],
        ["PLANE LONGITUDE", "degrees"],
        ["PLANE ALTITUDE", "feet"],
        ["AIRSPEED TRUE", "knots"],
    ],
    (data) => {
        //callback
        // Called when data is received

        console.log(
            "Latitude:  " + data["PLANE LATITUDE"] + "\n" +
            "Longitude: " + data["PLANE LONGITUDE"] + "\n" +
            "Altitude:  " + data["PLANE ALTITUDE"] + " feet\n" +
            "Speed:  " + data["AIRSPEED TRUE"] + " knots\n"
        );

    },
    simConnect.objectId.USER,               // User aircraft
    simConnect.period.SECOND,            // Get data every sim frame...
    simConnect.dataRequestFlag.CHANGED      // ...but only if one of the variables have changed
);*/