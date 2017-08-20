var gulp = require("gulp");
var babel = require("gulp-babel");
var exec = require("child_process").exec;


gulp.task("transpile", function () {
    return gulp.src("app/**/*.es6")
               .pipe(babel())
               .pipe(gulp.dest("dist/"));
});


gulp.task("copy", function () {
    return gulp.src("app/index.html")
               .pipe(gulp.dest("dist/"));
});


gulp.task("run", function () {
    return exec('"node_modules/.bin/electron" .');
});


gulp.task("default", ["transpile", "copy"], function () {
    gulp.start("run");
});

