
// ------------------- Exit gracefully  ------------------- //

process.stdin.resume();		//so the program will not close instantly

function exitHandler(options, err) {
	if (options.cleanup) {
		console.log('clean');
		serialPort.close();
	}
	if (err) console.log(err.stack);
	if (options.exit) process.exit();
}

//do something when app is closing
process.on('exit', exitHandler.bind(null, { cleanup: true }));

//catches ctrl+c event
process.on('SIGINT', exitHandler.bind(null, { exit: true }));

//catches uncaught exceptions
process.on('uncaughtException', exitHandler.bind(null, { exit: true }));

// ------------------- Exit gracefully  (end) ----------------- //


Object.defineProperty(global, '__stack', {
	get: function(){
		var orig = Error.prepareStackTrace;
		Error.prepareStackTrace = function(_, stack){ return stack; };
		var err = new Error;
		Error.captureStackTrace(err, arguments.callee);
		var stack = err.stack;
		Error.prepareStackTrace = orig;
		return stack;
	}
});

Object.defineProperty(global, '__line', {
	get: function(){
		return __stack[1].getLineNumber();
	}
});

console.log(__line);

// https://github.com/voodootikigod/node-serialport

var sp = require('serialport'),
	serialPort,
	rescanIntervalID,
	arduinoComName = '';

function scanPorts() {
	console.log('scanning ports...');
	sp.list(function (err, ports) {
		ports.forEach(function(port) {
			console.log('-------------------------');
			console.log('port', port);
			if (port.manufacturer.indexOf('Arduino') > -1) {
				arduinoComName = port.comName;
				startConnection(arduinoComName);
			} else if (port.comName === '/dev/cu.SLAB_USBtoUART') {
				arduinoComName = port.comName;
				startConnection(arduinoComName);
			}
		});
	});
}

function rescanPorts() {
	scanPorts();
	rescanIntervalID = setInterval(scanPorts, 1000);
}

console.log('-------------------------');

function startConnection(comName) {
	console.log(__line, 'starting connection to serial port "' + comName + '"...');
	//serialPort.parser = serialPort.parsers.readline("\n");
	
	clearInterval(rescanIntervalID);
	
	serialPort = new sp.SerialPort(comName, {
			baudrate: 9600,
			buffersize: 99999,
			flowControl: false,
			parser: sp.parsers.readline("\n")
		}, true); // this is the openImmediately flag [default is true]
		
	serialPort.open(function (error) {
		if (error) {
			console.log('failed to open: ' + error);
			rescanPorts();
		} else {
			console.log('open');
			serialPort.on('data', function(data) {
				
				var s = data.toString();
				console.log('data received:', s);
				
				if (s !== '') {
					
					//var parts = s.split(/IR: {0-9}*  Full: ({0-9}*)  Visible: ({0-9}*)  Lux: ({0-9}*)$/);
					//console.log('parts', parts);
					
					try {
						var json = JSON.parse(s);
						console.log('json', json);
					} catch(e) {
							// console.log(e.message);
					}
				}
				
			});
			serialPort.on('close', function(data) {
				console.log('connection closed.');
				rescanPorts();
			});
			serialPort.on('error', function(data) {
				console.log('there was an error with the connection.');
			});
			/*
			serialPort.write("ls\n", function(err, results) {
				console.log('err ' + err);
				console.log('results ' + results);
			});
			*/
		}
	});
}

rescanPorts();



