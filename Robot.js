var Buffer = require('buffer');

function Robot(options) {
 this.buffer = new Buffer(2);
 console.log(this);
 return this;
}

module.exports = Robot;
