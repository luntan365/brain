const botServer = require('../utils/botServer');
const { ipcRenderer } = require('electron');

export const BOT_STARTING = "BOT_STARTING";
export const BOT_RUNNING = "BOT_RUNNING";
export const BOT_STOPPING = "BOT_STOPPING";
export const BOT_STOPPED = "BOT_STOPPED";
export const BOT_ERROR = "BOT_ERROR";
export const BOT_CONNECTED = "BOT_CONNECTED";
export const BOT_DISCONNECTED = "BOT_DISCONNECTED";
export const BOT_LAUNCHED = "BOT_LAUNCHED";

export function start(botId) {
    return {
        type: BOT_STARTING,
        botId
    };
}

export function started(botId) {
    return {
        type: BOT_RUNNING,
        botId
    };
}

export function stop(botId) {
    return {
        type: BOT_STOPPING,
        botId
    };
}

export function stopped(botId) {
    return {
        type: BOT_STOPPED,
        botId
    };
}

export function error(botId) {
    return {
        type: BOT_ERROR,
        botId
    };
}

export function connected(botId) {
    return {
        type: BOT_CONNECTED,
        botId
    };
}

export function disconnected(botId) {
    return {
        type: BOT_DISCONNECTED,
        botId
    };
}

export function launched() {
    return {
        type: BOT_LAUNCHED
    };
}

export function requestStart(botId) {
    return dispatch => {
        dispatch(start(botId));

        botServer.request({'type': 'start'})
            .then(response => dispatch(started(botId)))
            .catch(err => dispatch(error(botId)));
    };
}

export function requestStop(botId) {
    return dispatch => {
        dispatch(stop(botId));

        botServer.request({'type': 'stop'})
            .then(response => dispatch(stopped(botId)))
            .catch(err => dispatch(error(botId)));
    };
}

export function startNewBot() {
    return dispatch => {
        dispatch(launched());
        ipcRenderer.send('launch-bot', {});
    };
}
