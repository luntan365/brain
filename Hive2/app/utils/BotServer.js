const { ipcMain } = require('electron');
const net = require('net');
const json = require('json-stream');
const { RandomUInt32 } = require('./Helpers');

var server = net.createServer();

class BotServer {

    constructor() {

    }

    request(object) {
        const requestId = RandomUInt32();
        object['id'] = requestId;
        send(object);
        return onNextResponse(requestId)
            .then(response => {
                if (response.success) {
                    Promise.resolve(response);
                } else {
                    Promise.reject(response);
                }
            });
    }
}

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

    ipcMain.on('message', function(event, arg) {
        encode.write(arg);
    });
});

server.listen(1337, '127.0.0.1');

export default server;
