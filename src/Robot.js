function Robot(serialPort) {
  this.serialPort = serialPort;
}

Robot.prototype.setSpeed = function(alpha, amplitude) {
  this.x = Math.round(Math.cos(parseInt(alpha, 10)) * parseInt(amplitude, 10));
  this.y = Math.round(Math.sin(parseInt(alpha, 10)) * -1 * parseInt(amplitude, 10));
}

Robot.prototype.move = function() {
  this.serialPort.write(new Buffer([this.x,this.y]));
};

module.exports = Robot;
