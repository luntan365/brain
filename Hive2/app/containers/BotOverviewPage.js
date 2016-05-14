import { bindActionCreators } from 'redux';
import { connect } from 'react-redux';
import BotOverview from '../components/BotOverview';
import * as BotActions from '../actions/bot';

function mapStateToProps(state, ownProps) {
    let botState = state.bot.bots[ownProps.params.botId];
    return {
        runState: botState.runState,
        gameState: botState.gameState,
    }
}

function mapDispatchToProps(dispatch) {
    return bindActionCreators(BotActions, dispatch);
}

export default connect(mapStateToProps, mapDispatchToProps)(BotOverview);
