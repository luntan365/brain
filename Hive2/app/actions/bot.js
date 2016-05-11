import botServer from '../utils/botServer';
import { ipcRenderer } from 'electron';

export const BOT_STARTING = "BOT_STARTING";
export const BOT_RUNNING = "BOT_RUNNING";
export const BOT_STOPPING = "BOT_STOPPING";
export const BOT_STOPPED = "BOT_STOPPED";
export const BOT_ERROR = "BOT_ERROR";
export const BOT_CONNECTED = "BOT_CONNECTED";
export const BOT_DISCONNECTED = "BOT_DISCONNECTED";
export const BOT_LAUNCHED = "BOT_LAUNCHED";
export const BOT_CONFIG_UPDATE = "BOT_CONFIG_UPDATE";
export const BOT_CONFIG_UPDATE_START = "BOT_CONFIG_UPDATE_START";
export const BOT_CONFIG_UPDATE_SUCCESS = "BOT_CONFIG_UPDATE_SUCCESS";
export const BOT_CONFIG_UPDATE_FAILURE = "BOT_CONFIG_UPDATE_FAILURE";
export const BOT_CONFIG_UPDATE_CLEAR = "BOT_CONFIG_UPDATE_FAILURE";
export const BOT_GOT_BOT_CHOICES = "BOT_GOT_BOT_CHOICES";
export const BOT_START_SELECT_BOT = "BOT_START_SELECT_BOT";
export const BOT_SELECT_BOT_SUCCESS = "BOT_SELECT_BOT_SUCCESS";
export const BOT_SELECT_BOT_ERROR = "BOT_SELECT_BOT_ERROR";

export const UPDATE_IDLE = "UPDATE_IDLE";
export const UPDATE_IN_PROGRESS = "UPDATE_IN_PROGRESS";
export const UPDATE_SUCCESS = "UPDATE_SUCCESS";
export const UPDATE_FAILURE = "UPDATE_FAILURE";

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

export function updateConfig(botId, config) {
    return {
        type: BOT_CONFIG_UPDATE,
        config: config,
        botId
    };
}

export function startConfigUpdate(botId) {
    return {
        type: BOT_CONFIG_UPDATE_START,
        botId
    }
}

export function configUpdateSuccess(botId) {
    return {
        type: BOT_CONFIG_UPDATE_SUCCESS,
        botId
    }
}

export function configUpdateFailure(botId, errorMessage) {
    return {
        type: BOT_CONFIG_UPDATE_FAILURE,
        errorMessage: errorMessage,
        botId
    }
}

export function configUpdateClear(botId) {
    return {
        type: BOT_CONFIG_UPDATE_CLEAR,
        botId
    };
}

export function gotBotChoices(botId, bots) {
    return {
        type: BOT_GOT_BOT_CHOICES,
        bots,
        botId
    };
}

export function requestBotChoices(botId) {
    return dispatch => {
        botServer.request(botId, {type: 'query-bots'})
            .then(response => {
                dispatch(gotBotChoices(botId, response.payload.bots));
            });
    };
}

export function startSelectBot(botId, botName) {
    return {
        type: BOT_START_SELECT_BOT,
        selected: botName,
        botId
    };
}

export function selectBotSuccess(botId, botName) {
    return {
        type: BOT_SELECT_BOT_SUCCESS,
        selected: botName,
        botId
    };
}

export function selectBotFailure(botId, botName) {
    return {
        type: BOT_SELECT_BOT_ERROR,
        selected: botName,
        botId
    };
}

export function selectBot(botId, botName) {
    return dispatch => {
        dispatch(startSelectBot(botId, botName))
        botServer.request(botId, {
            type: 'set-bot',
            payload: {
                bot: botName
            }
        }).then(response => {
            dispatch(
                selectBotSuccess(botId, response.payload.selected)
            );
        }).catch(err => {
            dispatch(
                selectBotFailure(botId, response.payload.selected)
            );
        });
    }
}

export function requestStart(botId) {
    return dispatch => {
        dispatch(start(botId));

        botServer.request(botId, {type: 'start'})
            .then(response => dispatch(started(botId)))
            .catch(err => dispatch(error(botId)));
    };
}

export function requestStop(botId) {
    return dispatch => {
        dispatch(stop(botId));

        botServer.request(botId, {type: 'stop'})
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

export function getConfig(botId) {
    return dispatch => {
        botServer.request(botId, {type: 'config'})
            .then(response => {
                dispatch(updateConfig(botId, response.payload));
            });
    }
}

export function updateConfig(botId, config) {
    return dispatch => {
        dispatch(startConfigUpdate(botId));
        botServer.request(botId, {
            type: 'set-config',
            payload: config
        }).then(response => {
            dispatch(configUpdateSuccess(botId));
        }).catch(err => {
            dispatch(configUpdateFailure(botId, "error"));
        });
    };
}
