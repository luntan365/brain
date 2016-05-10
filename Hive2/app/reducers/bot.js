import { combineReducers } from 'redux';
import {
    BOT_CONNECTED,
    BOT_DISCONNECTED,
    BOT_ERROR,
    BOT_LAUNCHED,
    BOT_STARTING,
    BOT_RUNNING,
    BOT_STOPPING,
    BOT_STOPPED
} from '../actions/bot';

function runStateReducer(state, action) {
    console.log(action);
    switch (action.type) {
        case BOT_STARTING:
            return "Starting";
        case BOT_RUNNING:
            return "Running";
        case BOT_STOPPING:
            return "Stopping";
        case BOT_ERROR:
            return "Error";
        case BOT_STOPPED:
            return "Idle";
        default:
            return state;
    }
}

function botState(state, action) {
    console.log(state);
    console.log(action);
    return Object.assign({}, state, {
        runState: runStateReducer(state.runState, action)
    });
}

function launchingCount(state = 0, action) {
    if (action.type === BOT_LAUNCHED) {
        return state + 1;
    } else if (action.type === BOT_CONNECTED) {
        return Math.max(0, state - 1);
    } else {
        return state;
    }
}

function bots(state = {}, action) {
    if (action.botId === undefined || action.botId === null) {
        return state;
    } else if (action.type === BOT_CONNECTED) {
        let defaultBotState = {
            id: action.botId,
            runState: "Idle"
        };
        return Object.assign({}, state, {
            [action.botId]: defaultBotState
        })
    } else if (action.type === BOT_DISCONNECTED) {
        let stateCopy = {...state};
        if (action.botId in stateCopy) {
            delete stateCopy[action.botId];
        }
        return stateCopy;
    } else if (action.botId in state) {
        return Object.assign({}, state, {
            [action.botId]: botState(state[action.botId])
        });
    } else {
        return state;
    }
}

const bot = combineReducers({
    launchingCount,
    bots
});

export default bot;
