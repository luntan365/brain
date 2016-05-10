const botServer = require('../utils/botServer');

export const BOT_STARTING = "BOT_STARTING";
export const BOT_RUNNING = "BOT_RUNNING";
export const BOT_STOPPING = "BOT_STOPPING";
export const BOT_STOPPED = "BOT_STOP    PED";
export const BOT_ERROR = "BOT_ERROR";

export function start() {
    return {
        type: BOT_STARTING
    };
}

export function started() {
    return {
        type: BOT_RUNNING
    };
}

export function stop() {
    return {
        type: BOT_STOPPING
    };
}

export function stopped() {
    return {
        type: BOT_STOPPED
    };
}

export function error() {
    return {
        type: BOT_ERROR
    };
}

export function requestStart() {
    return dispatch => {
        dispatch(start());

        botServer.request({'type': 'start'})
            .then(response => dispatch(started()))
            .catch(err => dispatch(error()));
    };
}

export function requestStop() {
    return dispatch => {
        dispatch(stop());

        botServer.request({'type': 'stop'})
            .then(response => dispatch(stopped()))
            .catch(err => dispatch(error()));
    };
}
