var $ = require('jquery');
var io = require('socket.io-client');
var Kinetic = require('kinetic');

var app = (function($, window) {

  $(document).ready(function() {

    var stage = new Kinetic.Stage({
      width: 500,
      height: 500,
      container: $('#joystick')[0]
    });

    var layer = new Kinetic.Layer();

    var circle = new Kinetic.Circle({
      x: 500 / 2,
      y: 500 / 2,
      radius: 10,
      fill: 'red',
    });

    layer.add(circle);
    stage.add(layer);
    layer.draw();

    var doc = this;
    var speed = {
      alpha: 0,
      amplitude: 0
    };

    doc.addEventListener('keypress', function(e) {
      if (e.which === 122) {
        speed.amplitude = 100;
      }
      if (e.which === 115) {
        speed.amplitude = 0;
      }
      if (e.which === 113) {
        speed.alpha = Math.PI / 2;
      }
      if (e.which === 100) {
        speed.alpha = -Math.PI / 2;
      }
      if (e.which === 120) {
        speed.alpha = Math.PI;
        speed.amplitude = 100;
      }
    });

    /*    setInterval(function() {
      socket.emit('move', speed);
    }, 100);

    socket.on('moved', function(data) {
      console.log(data);
    });*/
  });

})($, window);
