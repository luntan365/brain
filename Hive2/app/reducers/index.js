import { combineReducers } from 'redux';
import { routerReducer as routing } from 'react-router-redux';
import bot from './bot';
import counter from './counter';

const rootReducer = combineReducers({
    bot,
    counter,
    routing
});

export default rootReducer;
