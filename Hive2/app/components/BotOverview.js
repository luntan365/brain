import React, { Component, PropTypes } from 'react';
import { Link } from 'react-router';
import styles from './BotOverview.css';

class BotOverview extends Component {
    static propTypes = {
        runState: PropTypes.string.isRequired,
        gameState: PropTypes.object.isRequired,
    };

    render() {
        const { runState, gameState } = this.props;
        return (
            <div className={styles.data}>
                State: {runState} <br/>
                Pid: 12345 <br/>
                Game: <pre>{JSON.stringify(gameState, null, 2)}</pre>
            </div>
        );
    }
}

export default BotOverview;
