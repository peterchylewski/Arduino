
var cb = require('CritterHub');

var path = require('path');
var express = require('express');
var app = express();

//app.use(express.static(path.normalize(__dirname, 'public')));
app.use(express.static('public'));

app.get('/', function (req, res) {
  	res.sendFile( __dirname + '/public/' + 'nipkow.html' );
});

app.get('/dht', function (req, res) {
  	res.sendFile( __dirname + '/public/' + 'dht.html' );
});

app.get('/gyro_accel', function (req, res) {
  	res.sendFile( __dirname + '/public/' + 'gyro_accel.html' );
});

app.get('/timeseries', function (req, res) {
  	res.sendFile( __dirname + '/public/' + 'timeseries.html' );
});

var server = app.listen(3000, function() {
	var host = server.address().address, // '192.168.1.10'
		port = server.address().port;
	console.info('server.js: listening at http://%s:%s', host, port);
});

var io = require('socket.io').listen(server),
	users = {};

io.on('connection', function(socket) {
	console.log('we have a connection');
	
	cb.setSocket(socket);
	
	// broadcast events
	socket.on('event', function(data) {
		console.log('event received:', data);
		socket.broadcast.emit('event', data);
	});
	
	socket.on('message', function(data, fn) {
		console.log('message received:', data.text);
		// http://stackoverflow.com/questions/13549734/success-callback-for-emit-method-in-socket-io
		fn(true);
	});
	
	
	socket.on('disconnect', function() {
		console.log('disconnected :(');
	});
	
	socket.emit('message', { sender: 'server', text: 'hello, world!!!!' });
	
});



