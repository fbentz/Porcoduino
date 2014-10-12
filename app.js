var express = require('express');
var app = express();
var http = require('http');

app.listen = function() {
  var server = http.createServer(this);
  return server.listen.apply(server, arguments);
}

module.exports = app;

if (!module.parent) {
  app.listen(3000);
}
