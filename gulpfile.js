'use strict';
var gulp = require('gulp');
var del = require('del');
var path = require('path');
var mocha = require('gulp-mocha');

var options = {
  folders: {
    build: './build',
    src: './src',
    tests: './tests'
  },
};

gulp.task('clean', function(cb) {
  del(options.build, cb);
});

gulp.task('tests:server', function() {
  return gulp.src(path.join(options.folders.tests, 'server/**/*_test.js'), {
    read: false
  })
    .pipe(mocha({
      reporter: 'spec'
    }));
});

gulp.task('tests', ['tests:server']);

gulp.task('default', ['tests']);
