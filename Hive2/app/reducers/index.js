import { combineReducers } from 'redux';
import { routerReducer as routing } from 'react-router-redux';
import bot from './bot';

const rootReducer = combineReducers({
    bot,
    routing
});

export default rootReducer;
