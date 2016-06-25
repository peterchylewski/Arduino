
var cp = require('child_process');

var s = require('SerialConnector')(this);

var volume = 10;

this.data = function(data) {
	console.log('data', data);
	volume = data;
	var buffer = cp.execSync('mocp --volume ' + volume);
	//console.log('buffer', buffer.toString());
};

var buffer = cp.execSync('mocp --format "%album"');
console.log('buffer', buffer.toString());