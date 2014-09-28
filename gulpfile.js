'use strict';
var gulp = require('gulp');
var del = require('del');

var options = {
  build: './build',
  src: '.src'
};

gulp.task('clean', function(cb) {
  del(options.build, cb);
});

gulp.task('test', ['clean'],function() {
  return;
});
