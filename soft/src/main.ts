// imports
const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline');
const simConnect = require('msfs-simconnect-nodejs');
import { v4 as uuidv4 } from 'uuid';
import { simData } from './simData';

//constants
const portname = 'COM8';
const port = new SerialPort(portname, { baudRate: 115200 });
const parser = port.pipe(new Readline({ delimiter: '\n' }));// Read the port data

var lastmessageid: string;
var simdata: simData = new simData();

// when the serial port connects
port.on("open", () => {
    console.log('Serial port ' + portname + ' open');
});

// when the serial port receives data
parser.on('data', (data: string) => {
    data = data.trim();
    if (data == lastmessageid) {
        // all good
        console.log('✔')
    }
    else if (data == "KO") {
        // got an error
        console.error("❌")
    }
});

setInterval(() => {
    lastmessageid = uuidv4().split('-')[0]
    var sendData = {
        id: lastmessageid,
        data: simdata.getArduinoArray()
    }
    var datajson = JSON.stringify(sendData) + "\n";
    console.log(datajson.length);
    port.write(datajson, function (err: Error) {
        if (err) {
            return console.log('Error on write: ', err.message)
        }
        //   console.log('message written')
    })

}, 2500)

var success = simConnect.open("MyAppName",
    (name: string, version: string) => {
        //connectedCallback
        console.log("Connected to: " + name + "\nSimConnect version: " + version);
        // Safe to start interacting with SimConnect here (request data, etc)
    }, () => {
        //simExitedCallback
        console.log("Simulator exited by user");
    }, (exception: any) => {
        //exceptionCallback
        console.log("SimConnect exception: " + exception.name + " (" + exception.dwException + ", " + exception.dwSendID + ", " + exception.dwIndex + ", " + exception.cbData + ")");
    }, (error: any) => {
        //errorCallback
        console.log("Undexpected disconnect/error: " + error); // Look up error code in ntstatus.h for details
    });

simConnect.requestDataOnSimObject(
    //https://docs.microsoft.com/en-us/previous-versions/microsoft-esp/cc526981(v=msdn.10)
    [
        ["PLANE LATITUDE", "degrees"],
        ["PLANE LONGITUDE", "degrees"],
        ["PLANE ALTITUDE", "feet"],
        ["AIRSPEED INDICATED", "knots"],
        ["ATTITUDE INDICATOR PITCH DEGREES", "degrees"],
        ["ATTITUDE INDICATOR BANK DEGREES", "degrees"],
    ],
    (data: any) => {
        // Called when data is received
        simdata.lat = parseFloat(data["PLANE LATITUDE"].toFixed(5));
        simdata.long = parseFloat(data["PLANE LONGITUDE"].toFixed(5));
        simdata.speed = parseInt(data["AIRSPEED INDICATED"]);
        simdata.alt = parseInt(data["PLANE ALTITUDE"]);
        simdata.pitch = parseFloat(data["ATTITUDE INDICATOR PITCH DEGREES"].toFixed(2));
        simdata.bank = parseFloat(data["ATTITUDE INDICATOR BANK DEGREES"].toFixed(2));

      //  console.log(simdata);
       // console.log(data);

    },
    simConnect.objectId.USER,               // User aircraft
    simConnect.period.SECOND,            // Get data every sim frame...
    simConnect.dataRequestFlag.CHANGED      // ...but only if one of the variables have changed
);
