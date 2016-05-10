const { ipcRenderer } = require('electron');
const { RandomUInt32 } = require('./Helpers');


class BotServer {

    constructor() {

    }

    onResponse(id) {
        return new Promise( (resolve, reject) => {
            let timeout = setTimeout( () => {
                return reject(new Error("Request Timeout"));
            }, 10000);
            ipcRenderer.on('message', (event, arg) => {
                if (arg.type == 'response' && arg['request-id'] == id) {
                    clearTimeout(timeout);
                    return resolve(arg);
                }
            });
        });
    }

    send(msg) {
        ipcRenderer.send('message', msg);
    }

    request(object) {
        const requestId = RandomUInt32();
        object['id'] = requestId;
        this.send(object);
        return this.onResponse(requestId)
            .then(response => {
                if (response.success) {
                    Promise.resolve(response);
                } else {
                    Promise.reject(response);
                }
            });
    }
}

let server = new BotServer();

export default server;
