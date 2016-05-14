import botServer from '../utils/botServer';
export const PROFILE_UPDATED = "PROFILE_UPDATED";
export const PROFILE_CLEAR_ADD_FIELDS = "PROFILE_CLEAR_ADD_FIELDS";
export const PROFILE_UPDATE_ADD_FIELD = "PROFILE_UPDATE_ADD_FIELD";

function updateProfile(botId, positions) {
    return {
        type: PROFILE_UPDATED,
        botId,
        positions
    }
}

function profileClearAddFields(botId) {
    return {
        type: PROFILE_CLEAR_ADD_FIELDS,
        botId
    }
}

function updatePositions(botId, newPositions) {
    return (dispatch) => {
        botServer.request(botId, {
            type: 'set-profile',
            payload: {
                positions: newPositions,
            }
        }).then(response => {
            dispatch(updateProfile(botId, newPositions));
            dispatch(profileClearAddFields(botId));
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

export function updateAddPosition(botId, field, value) {
    return {
        type: PROFILE_UPDATE_ADD_FIELD,
        botId,
        field,
        value
    }
}
