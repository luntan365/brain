import React from 'react';
import { Route, IndexRoute } from 'react-router';
import App from './containers/App';
import HomePage from './containers/HomePage';
import BotPage from './containers/BotPage';
import BotSummaryPage from './containers/BotSummaryPage';


export default (
  <Route path="/" component={App}>
    <IndexRoute component={HomePage} />
    <Route path="bots" component={BotSummaryPage} />
    <Route path="bots/:botId" component={BotPage} />
  </Route>
);
