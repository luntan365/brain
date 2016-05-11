import { bindActionCreators } from 'redux';
import { connect } from 'react-redux';
import Bot from '../components/Bot';
import * as BotActions from '../actions/bot';

function mapStateToProps(state, ownProps) {
    let botState = state.bot.bots[ownProps.params.botId];
    if (botState === undefined) {
        return {
            notFound: true
        }
    } else {
        return {
            ...botState,
            notFound: false
        }
    }
}

function mapDispatchToProps(dispatch) {
    return bindActionCreators(BotActions, dispatch);
}

export default connect(mapStateToProps, mapDispatchToProps)(Bot);
