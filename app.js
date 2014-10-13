var http = require('http');
var express = require('express');
var config = require('./config');
var app = express();
var server = http.createServer(app);
var io = require('socket.io')(server);

app.use(express.static(__dirname + '/build'));

io.on('connection', function(socket) {
  socket.on('move', function(data) {
    socket.emit('moved', data);
  });
});

module.exports = server;

if (!module.parent) {
  server.listen(config.server.port);
}
