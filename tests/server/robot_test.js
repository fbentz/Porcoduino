var chai = require('chai');
var expect = chai.expect;
var Robot = require('../../src/Robot');
var sinon = require('sinon');

describe('Robot', function() {

  describe('#contructor', function() {
    it('should be instanciate', function() {
      var robot = new Robot
      expect(robot).to.be.instanceof(Robot);
    });
    it('should be instanciate with an adapater', function() {
      var fakeAdapter = {};
      var robot = new Robot(fakeAdapter);
      expect(robot.serialPort).to.deep.equal({});
    });
  });

  describe('#setSpeed', function() {
    it('should set speed with an amplitude and alpha', function() {
      var robot = new Robot();
      robot.setSpeed(2, 100);
      expect(robot.x).to.be.equal(-42);
      expect(robot.y).to.be.equal(-91);
    });
  });

  describe('#move', function() {
    var sandbox;
    var fakeSerialPort;
    var robot;

    beforeEach(function() {
      sandbox = sinon.sandbox.create();
      fakeSerialPort = {
        write: sinon.spy()
      };
      robot = new Robot(fakeSerialPort);
      robot.x = 41;
      robot.y = 90;
    });

    it('should write x,y on serial port', function(done) {
      robot.move();
      sinon.assert.calledWithExactly(fakeSerialPort.write, sinon.match(function(value) {
        return JSON.stringify(value.toJSON())=== JSON.stringify([robot.x, robot.y]);
      }));
      done();
    });

    afterEach(function(done) {
      sandbox.restore();
      done();
    });
  });

});
