const gulp = require('gulp');
const spawn = require('child_process').spawn;
const livereload = require('gulp-livereload');
let node;

function server() {
    if(node) {
        node.kill();
    }
    node = spawn('node', ['server.js'], {
        stdio: 'inherit'
    });
    node.on('close', function (code) {
        if(code === 8) {
            gulp.log('Error detected, waiting for changes...');
        }
    });
}

gulp.task('server', function () {
    server();
});

gulp.task('watch', ['server'], function () {
    livereload.listen({
        port: 3001
    });
    gulp.watch(['server.js'], {
        interval: 200
    }, function () {
        server();
    });
    gulp.watch(['*.html'], {
        interval: 200
    }, function () {
        livereload.reload();
    });
});

gulp.task('default', ['watch']);

// clean up if an error goes unhandled.
process.on('exit', function () {
    if(node) {
        node.kill();
    }
});
