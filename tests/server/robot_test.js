var chai = require('chai');
var expect = chai.expect;
var Robot = require('../../Robot');
var SerialPort = require('serialPort').SerialPort;

describe('Robot', function() {

  describe('#contructor', function() {
    it('should have a buffer with 2 arrays', function(done) {
      var robot = new Robot();
      console.log(robot);
      expect(robot.buffer).to.be.instanceof(Buffer);
      done();
    });
  });

  describe('#setPosition', function() {
    it('', function(done) {
      done();
    });
  });

});
