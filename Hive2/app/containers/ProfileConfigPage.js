import { bindActionCreators } from 'redux';
import { connect } from 'react-redux';
import ProfileConfig from '../components/ProfileConfig';
import * as ProfileActions from '../actions/profile';

function mapStateToProps(state, ownProps) {
    let botState = state.bot.bots[ownProps.params.botId];
    return {
        botId: ownProps.params.botId,
        ...botState.profile,
        gameState: botState.gameState,
    }
}

function mapDispatchToProps(dispatch) {
    return bindActionCreators(ProfileActions, dispatch);
}

export default connect(mapStateToProps, mapDispatchToProps)(ProfileConfig);
