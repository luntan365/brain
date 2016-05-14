import React from 'react';
import { Route, IndexRoute } from 'react-router';
import App from './containers/App';
import HomePage from './containers/HomePage';
import BotPage from './containers/BotPage';
import BotSummaryPage from './containers/BotSummaryPage';
import BotOverviewPage from './containers/BotOverviewPage';
import BotConfigPage from './containers/BotConfigPage';
import ProfileConfigPage from './containers/ProfileConfigPage';


export default (
  <Route path="/" component={App}>
    <IndexRoute component={HomePage} />
    <Route path="bots" component={BotSummaryPage} />
    <Route path="bots/:botId" component={BotPage}>
        <IndexRoute component={BotOverviewPage} />
        <Route path="config" component={BotConfigPage} />
        <Route path="profile" component={ProfileConfigPage} />
    </Route>
  </Route>
);

