import { combineReducers } from 'redux';
import {
    BOT_CONNECTED,
    BOT_DISCONNECTED,
    BOT_ERROR,
    BOT_LAUNCHED,
    BOT_STARTING,
    BOT_RUNNING,
    BOT_STOPPING,
    BOT_STOPPED,

    BOT_CONFIG_FIELD_UPDATED,
    BOT_CONFIG_UPDATE,
    BOT_CONFIG_UPDATE_START,
    BOT_CONFIG_UPDATE_SUCCESS,
    BOT_CONFIG_UPDATE_FAILURE,
    BOT_CONFIG_UPDATE_CLEAR,

    BOT_GOT_BOT_CHOICES,
    BOT_START_SELECT_BOT,
    BOT_SELECT_BOT_SUCCESS,
    BOT_SELECT_BOT_ERROR,

    BOT_UPDATE_GAME_STATE,

    UPDATE_IDLE,
    UPDATE_IN_PROGRESS,
    UPDATE_SUCCESS,
    UPDATE_FAILURE
} from '../actions/bot';

import {
    PROFILE_UPDATED,
    PROFILE_CLEAR_ADD_FIELDS,
    PROFILE_UPDATE_ADD_FIELD
} from '../actions/profile';

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

function configReducer(state, action) {
    switch (action.type) {
        case BOT_CONFIG_UPDATE:
            return {...state, config: action.config};
        case BOT_CONFIG_FIELD_UPDATED:
            console.log(state);
            let newState = {
                ...state,
                config: Object.assign({}, state.config, {
                    [action.key]: action.value
                })
            };
            console.log(newState);
            return newState;
        case BOT_CONFIG_UPDATE_START:
            return {...state, updateState: UPDATE_IN_PROGRESS};
        case BOT_CONFIG_UPDATE_SUCCESS:
            return {...state, updateState: UPDATE_SUCCESS};
        case BOT_CONFIG_UPDATE_FAILURE:
            return {
                ...state,
                updateState: UPDATE_FAILURE,
                errorMessage: action.errorMessage
            };
        case BOT_CONFIG_UPDATE_CLEAR:
            return {...state, updateState: UPDATE_IDLE};
        default:
            return state;
    }
}

function botChoiceReducer(state, action) {
    switch (action.type) {
        case BOT_GOT_BOT_CHOICES:
            return { ...state, bots: action.bots };
        case BOT_START_SELECT_BOT:
            return {
                ...state,
                updateState: UPDATE_IN_PROGRESS,
                selected: action.selected
            };
        case BOT_SELECT_BOT_SUCCESS:
            return {
                ...state,
                updateState: UPDATE_SUCCESS,
                selected: action.selected
            };
        case BOT_SELECT_BOT_ERROR:
            return {
                ...state,
                updateState: UPDATE_IN_PROGRESS,
                selected: action.selected,
                errorMessage: action.errorMessage
            };
        default:
            return state;
    }
}

function gameStateReducer(state, action) {
    switch (action.type) {
        case BOT_UPDATE_GAME_STATE:
            return action.state;
    }
    return state;
}

function profileReducer(state, action) {
    switch (action.type) {
        case PROFILE_UPDATED:
            return {
                ...state,
                positions: action.positions
            }
        case PROFILE_CLEAR_ADD_FIELDS:
            let newState = {...state};
            delete newState.addX;
            delete newState.addY;
            delete newState.addZ;
            return newState;
        case PROFILE_UPDATE_ADD_FIELD:
            return {
                ...state,
                'x': action.value,
                ["add" + action.field.toUpperCase()]: action.value
            }
        default:
            return state;
    }
}

function botState(state, action) {
    return Object.assign({}, state, {
        botChoice: botChoiceReducer(state.botChoice, action),
        gameState: gameStateReducer(state.gameState, action),
        runState: runStateReducer(state.runState, action),
        config: configReducer(state.config, action),
        profile: profileReducer(state.profile, action),
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
            runState: "Idle",
            botChoice: {
                bots: [],
                selected: "",
                updateState: UPDATE_IDLE
            },
            gameState: {},
            config: {
                config: {},
                updateState: UPDATE_IDLE
            },
            profile: {
                positions: []
            }
        };
        return Object.assign({}, state, {
            [action.botId]: defaultBotState
        });
    } else if (action.type === BOT_DISCONNECTED) {
        let stateCopy = {...state};
        if (action.botId in stateCopy) {
            delete stateCopy[action.botId];
        }
        return stateCopy;
    } else if (action.botId in state) {
        return Object.assign({}, state, {
            [action.botId]: botState(state[action.botId], action)
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
