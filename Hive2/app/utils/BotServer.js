const { ipcRenderer } = require('electron');
const { RandomUInt32 } = require('./Helpers');
const botActions = require('../actions/bot');

class BotServer {

    constructor() {
        this.dispatch = () => {};
        this.bots = new Set();

        ipcRenderer.on('bot-connected', (event, arg) => {
            console.log("Got connection" + arg);
            this.bots.add(arg.botId);
            this.dispatch(botActions.connected(arg.botId));
            this.dispatch(botActions.getConfig(arg.botId));
        });

        ipcRenderer.on('bot-disconnected', (event, arg) => {
            console.log("Got disconnection" + arg);
            this.bots.delete(arg.botId);
            this.dispatch(botActions.disconnected(arg.botId));
        });
    }

    setStore(store) {
        this.dispatch = store.dispatch;
    }

    bots() {
        return this.bots;
    }

    onResponse(botId, id) {
        return new Promise( (resolve, reject) => {
            let timeout = setTimeout( () => {
                return reject(new Error(`[${botId}] Request ${id}: Timeout`));
            }, 10000);
            ipcRenderer.on(botId, (event, arg) => {
                if (arg.type == 'response' && arg['request-id'] == id) {
                    clearTimeout(timeout);
                    return resolve(arg);
                }
            });
        });
    }

    send(botId, msg) {
        ipcRenderer.send(botId, msg);
    }

    request(botId, object) {
        const requestId = RandomUInt32();
        object['id'] = requestId;
        this.send(botId, object);
        return this.onResponse(botId, requestId)
            .then(response => {
                if (response.success) {
                    return response;
                } else {
                    return Promise.reject(response);
                }
            });
    }
}

export default new BotServer();
