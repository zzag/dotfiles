var app = require('app');
var BrowserWindow = require('browser-window');


var mainWindow = null;


app.on('window-all-closed', function() {
	app.quit();
});


app.on('ready', function() {
	var initData = {
		width: 640,
		height: 480
	};

	mainWindow = new BrowserWindow(initData);
	
	mainWindow.loadUrl('file://' + __dirname + '/index.html');
	mainWindow.on('closed', function() {
		mainWindow = null;
	});
});