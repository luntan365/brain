import { bindActionCreators } from 'redux';
import { connect } from 'react-redux';
import BotSummary from '../components/BotSummary';
import { startNewBot } from '../actions/bot';

function mapStateToProps(state) {
    return state.bot;
}

function mapDispatchToProps(dispatch) {
  return bindActionCreators({ startNewBot }, dispatch);
}

export default connect(mapStateToProps, mapDispatchToProps)(BotSummary);
