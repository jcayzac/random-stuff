/*
  md2pdf
  http://github.com/jcayzac

  converts Markdown content to PDF,
  using showdown.js and htmldoc

*/

var Showdown = require('showdown').Showdown;
var sys = require('sys');
var spawn = require('child_process').spawn;
var converter = new Showdown.converter();

var md=[];
process.stdin.resume();

process.stdin.on('data', function(text) { md.push(text); });
process.stdin.on('end', function() {
	var child = spawn("htmldoc", ['-t','pdf14','--color','--book','--no-jpeg','--header','.tc','--footer','..1','-']);
	child.stdout.on('data', function(x) { process.stdout.write(x); });
	child.stdin.write(converter.makeHtml(md.join('')));
	child.stdin.end();
});

