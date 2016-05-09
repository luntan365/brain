const electron = require('electron');
const app = electron.app;
const BrowserWindow = electron.BrowserWindow;

const ipc = electron.ipcMain;
const net = require('net');
const json = require('json-stream');

// Keep a global reference of the window object, if you don't, the window will
// be closed automatically when the JavaScript object is garbage collected.
let mainWindow

let mainWindowQueue = [];

function createWindow () {
    mainWindow = new BrowserWindow({width: 800, height: 600});

    mainWindow.loadURL('file://' + __dirname + '/index.html');

    mainWindow.webContents.openDevTools();

    while (mainWindowQueue.length != 0 && mainWindow !== null) {
        const obj = mainWindowQueue.shift();
        mainWindow.webContents.send('message', obj);
    };

    mainWindow.on('closed', function () {
        mainWindow = null;
    });
}

app.on('ready', createWindow)

app.on('window-all-closed', function () {
    if (process.platform !== 'darwin') {
        app.quit();
    }
});

app.on('activate', function () {
    if (mainWindow === null) {
        createWindow();
    }
});

var server = net.createServer();
server.on('connection', function(socket) {
    let encode = json.encode();
    let parse = json.parse();
    socket.pipe(parse);
    encode.pipe(socket);

    parse.on('message', function(obj) {
        if (mainWindow !== null) {
            mainWindow.webContents.send('message', obj);
        } else {
            mainWindowQueue.push(obj);
        }
    });

    ipc.on('message', function(event, arg) {
        encode.write(arg);
    });
});


server.listen(1337, '127.0.0.1');
