var chai = require('chai');
var expect = chai.expect;
var Robot = require('../../src/Robot');

describe('Robot', function() {

  describe('#contructor', function() {
    it('should be instanciate', function() {
      var robot = new Robot();
      expect(robot).to.be.instanceof(Robot);
    });
  });

  describe('#setPosition', function() {

    var robot;

    beforeEach(function() {
      robot = new Robot();
    });

    afterEach(function() {
      delete robot;
    })

    it('should have an array buffer with 2 dimension', function() {
      robot.setPosition(new Buffer([100,100]));
      expect(robot.buffer).to.be.instanceof(Buffer);
    });
  });

});
