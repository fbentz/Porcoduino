'use strict';
var gulp = require('gulp');
var del = require('del');
var mocha = require('gulp-mocha');
var path = require('path');
var source = require('vinyl-source-stream');
var browserify = require('browserify');

var options = {
  paths: {
    build: './build',
    src: './src',
    client: {
      js: 'client/js'
    },
    tests: {
      server: './tests/server/**/*_test.js',
      client: './tests/client/**/*_test.js'
    }
  },
};

gulp.task('clean', function(cb) {
  del(options.paths.build, cb);
});

gulp.task('copy', ['clean'], function() {
  return gulp.src('./' + path.join(options.paths.src, options.paths.client.js,'index.html'))
    .pipe(gulp.dest(options.paths.build));
});

gulp.task('tests:server', function() {
  return gulp.src(options.paths.tests.server, {
    read: false
  })
    .pipe(mocha({
      ui: 'bdd',
      reporter: 'dot'
    }));
});

gulp.task('browserify:app', ['browserify:vendors'], function() {
  var bundleStream = browserify('./' + path.join(options.paths.src, options.paths.client.js,'app.js'));

  bundleStream
    .external('socket.io-client')
    .external('jquery')
    .bundle()
    .pipe(source('app.js'))
    .pipe(gulp.dest(options.paths.build + '/js'));

  return bundleStream;
});


gulp.task('browserify:vendors', function() {
  var bundleStream = browserify();

  bundleStream
    .require('socket.io-client')
    .require('jquery')
    .bundle()
    .pipe(source('vendors.js'))
    .pipe(gulp.dest(options.paths.build + '/js'));

  return bundleStream;
});

gulp.task('watch', function() {
  gulp.watch(options.paths.tests.server,['tests:server']);
});

gulp.task('tests', ['tests:server']);
gulp.task('browserify', ['browserify:app']);
gulp.task('default', ['copy', 'browserify', 'tests']);
gulp.task('dev', ['default', 'watch']);
