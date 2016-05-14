import { bindActionCreators } from 'redux';
import { connect } from 'react-redux';
import BotConfig from '../components/BotConfig';
import * as BotActions from '../actions/bot';

function mapStateToProps(state, ownProps) {
    let botState = state.bot.bots[ownProps.params.botId];
    return {
        id: ownProps.params.botId,
        botChoice: botState.botChoice,
        config: botState.config,
    }
}

function mapDispatchToProps(dispatch) {
    return bindActionCreators(BotActions, dispatch);
}

export default connect(mapStateToProps, mapDispatchToProps)(BotConfig);
