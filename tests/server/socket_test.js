'use strict';
var chai = require('chai');
var expect = chai.expect;
var app = require('../../app');

describe('Socket', function() {

  var engine;

  before(function() {
    engine = app.listen(3000);
  });

  after(function() {
    engine.close();
  });

  it('should return 2', function() {
    expect(1).to.be.equal(1);
  });
});
