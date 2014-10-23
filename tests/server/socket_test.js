'use strict';
var chai = require('chai');
var expect = chai.expect;
var app = require('../../app');
var ioClient = require('socket.io-client');
var config = require('../../config');
describe('Socket', function() {

    var engine;
    var client;

    beforeEach(function(done) {
      app.listen(config.server.port);
      done();
    });

    afterEach(function(done) {
      app.close(done());
    });

    it('should received speed by the ws', function(done) {
      client = ioClient('http://localhost:' + config.server.port);
      client.emit('move', {alpha: 3, amplitude: 75});
      client.on('moved', function(data) {
        expect(data.alpha).to.be.equal(3);
        done();
      });
    });
});
