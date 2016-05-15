import botServer from '../utils/botServer';
export const PROFILE_UPDATED = "PROFILE_UPDATED";
export const PROFILE_CLEAR_ADD_FIELDS = "PROFILE_CLEAR_ADD_FIELDS";
export const PROFILE_UPDATE_ADD_FIELD = "PROFILE_UPDATE_ADD_FIELD";

function updateProfile(botId, profile) {
    return {
        type: PROFILE_UPDATED,
        botId,
        profile
    }
}

function updatePositions(botId, positions) {
    return (dispatch, getState) => dispatch(setProfile(botId, {
        ...getState().bot.bots[botId].profile,
        positions
    }));
}

export function updateNpc(botId, npcKey, npcObj) {
    return (dispatch, getState) => dispatch(setProfile(botId, {
        ...getState().bot.bots[botId].profile,
        [npcKey]: npcObj
    }));
}

export function setProfile(botId, profile) {
    return (dispatch) => {
        botServer.request(botId, {
            type: 'set-profile',
            payload: profile 
        }).then(response => {
            dispatch(updateProfile(botId, profile));
        });
    };
}

export function addProfilePosition(botId, x, y, z) {
    return (dispatch, getState) => {
        const position = { x, y, z };
        const newState = getState();
        let newPositions = newState.bot.bots[botId].profile.positions.slice();
        newPositions.push(position);
        dispatch(updatePositions(botId, newPositions));
    };
}

export function removeProfilePositions(botId, indicies) {
    return (dispatch, getState) => {
        const curPositions = getState().bot.bots[botId].profile.positions;
        const newPositions = curPositions.filter((_, index) => {
            return indicies.indexOf(index) == -1;
        });
        dispatch(updatePositions(botId, newPositions));
    };
}
