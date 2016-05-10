import { bindActionCreators } from 'redux';
import { connect } from 'react-redux';
import Bot from '../components/Bot';
import * as BotActions from '../actions/bot';

function mapStateToProps(state, ownProps) {
    return state.bot.bots[ownProps.params.id];
}

function mapDispatchToProps(dispatch) {
    return bindActionCreators(BotActions, dispatch);
}

export default connect(mapStateToProps, mapDispatchToProps)(Bot);
