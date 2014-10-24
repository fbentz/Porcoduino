var $ = require('jquery');
var io = require('socket.io-client');

$(document).ready(function() {
  var socket = io('http://localhost:8000');
  var doc = this;
  var speed = { alpha: 0, amplitude: 0};

  doc.addEventListener('keypress', function(e) {
    if (e.which === 122) {
      speed.amplitude = 100;
    }
    if (e.which === 115) {
      speed.amplitude = 0;
    }
    if (e.which === 113) {
      speed.alpha = Math.PI/2;
    }
    if (e.which === 100) {
      speed.alpha = -Math.PI/2;
    }
    if (e.which === 120) {
      speed.alpha = Math.PI;
      speed.amplitude = 100;
    }
  });

  setInterval(function() {
    socket.emit('move', speed);
  }, 100);
});
