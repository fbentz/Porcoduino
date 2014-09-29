'use strict';
var gulp = require('gulp');
var del = require('del');
var mocha = require('gulp-mocha');

var options = {
  paths: {
    build: './build',
    src: './src',
    tests: {
      server: './tests/server/**/*_test.js',
      client: './tests/client/**/*_test.js'
    }
  },
};

gulp.task('clean', function(cb) {
  del(options.build, cb);
});

gulp.task('tests:server', function() {
  return gulp.src(options.paths.tests.server, {
    read: false
  })
    .pipe(mocha({
      ui: 'bdd',
      reporter: 'spec'
    }));
});

gulp.task('watch', function() {
  gulp.watch(options.paths.tests.server,['tests:server']);
});

gulp.task('tests', ['tests:server']);

gulp.task('default', ['tests']);
gulp.task('dev', ['default', 'watch']);
