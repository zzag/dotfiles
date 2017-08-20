import app from "app";
import BrowserWindow from "browser-window";

require("crash-reporter").start();
let mainWindow = null;

console.log(app.getDataPath());


app.on("window-all-closed", () => {
      if (process.platform != 'darwin') {
            app.quit();
              }
});


app.on("ready", () => {
      mainWindow = new BrowserWindow({
            width: 800,
                height: 600,
                    title: "Lab 2",
                        "auto-hide-menu-bar": true
                          });

        mainWindow.loadUrl("file://" + __dirname + "/index.html");

          mainWindow.on("closed", () => {
                mainWindow = null;
                  });
});
