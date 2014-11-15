'use strict';
var http = require('http');
var express = require('express');
var config = require('./config');
var app = express();
var server = http.createServer(app);
var io = require('socket.io')(server);
var Robot = require('./src/lib/Robot');
var SerialPort = require('serialport').SerialPort;

var serialPort = new SerialPort(config.xbee.serialPort, {
    baudrate: config.xbee.baudrate
});

app.use(express.static(__dirname + '/build'));

var robot = new Robot(serialPort);

io.on('connection', function(socket) {
  socket.on('move', function(data) {
    robot.setSpeed(data.alpha, data.amplitude);
    robot.move();
    socket.emit('moved', data);
  });
});

module.exports = server;

if (!module.parent) {
  server.listen(config.server.port);
}
