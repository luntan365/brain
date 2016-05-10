import React, { Component, PropTypes } from 'react';
import { Link } from 'react-router';
import styles from './BotSummary.css';
import Bot from '../containers/BotPage';

class BotLink extends Component {
    static propTypes = {
        state: PropTypes.object.isRequired
    };

    render() {
        const { state } = this.props;
        return (
            <Link to={`/bots/${state.id}`}>
                <div className={styles.botlink}>
                    {state.id}: {state.runState}
                </div>
            </Link>
        );
    }
}


class BotSummary extends Component {
    static propTypes = {
        startNewBot: PropTypes.func.isRequired,
        bots: PropTypes.object.isRequired,
        launchingCount: PropTypes.number.isRequired
    };

    render() {
        const { startNewBot, launchingCount, bots } = this.props;
        let launchingPlacholders = Array.apply(null, Array(launchingCount))
            .map((_, i) => {
                return (
                    <div key={i} className={styles.botlink}>
                        Launching...
                    </div>
                );
            });
        let botLinks = Object.keys(bots).map(key => {
            let bot = bots[key];
            console.log(bot);
            return (
                <BotLink
                    key={bot.id}
                    state={bot}
                    />
            );
        });
        return (
            <div>
                <div className={styles.backButton}>
                    <Link to="/">
                        <i className="fa fa-arrow-left fa-3x" />
                    </Link>
                </div>
                <div className={styles.summary}>
                    <div className={styles.summaryHeader}>
                        BOTS
                    </div>
                    <div className={styles.buttonGroup}>
                        <div className={styles.button} onClick={startNewBot}>
                            LAUNCH
                        </div>
                        <div className={styles.button}>TEST</div>
                        <div className={styles.button}>TEST</div>
                        <div className={styles.button}>TEST</div>
                    </div>
                    {botLinks}
                    {launchingPlacholders}
                </div>
            </div>
        );
    }
}

export default BotSummary;
