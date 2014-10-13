'use strict';
var chai = require('chai');
var expect = chai.expect;
var app = require('../../app');
var ioClient = require('socket.io-client');
var config = require('../../config');
describe('Socket', function() {

  var engine;
  var client

  before(function() {
    engine = app.listen(config.server.port);
  });

  after(function() {
    engine.close();
  });

  it('should brodcast send on server', function(done) {
    client = ioClient('http://localhost:' + config.server.port);
    client.emit('move', {
      speed: {
        x: 100,
        y: 0
      }
    });
    client.on('moved', function(data) {
      console.log(data);
      expect(data.speed.x).to.be.equal(100);
      done();
    });
  });
});
