import React, { Component, PropTypes } from 'react';
import { Link } from 'react-router';
import styles from './Bot.css';
import BotConfig from './BotConfig';
import BotSelector from './BotSelector';
import BotSummary from '../containers/BotSummaryPage';

class Bot extends Component {
  static propTypes = {
    requestStart: PropTypes.func.isRequired,
    requestStop: PropTypes.func.isRequired,
    getConfig: PropTypes.func.isRequired,
    runState: PropTypes.string.isRequired,
    config: PropTypes.object.isRequired,
    notFound: PropTypes.bool.isRequired
  };

  render() {
    const {
        botChoice,
        id,
        requestStart,
        requestStop,
        config,
        runState,
        requestBotChoices,
        selectBot,
        notFound,
        updateConfigField,
        requestConfigUpdate
    } = this.props;
    let configSection;
    if (notFound || notFound === undefined) {
        return <BotSummary />
    }
    if (config.config) {
        configSection = (
            <BotConfig
                config={config}
                id={id}
                updateConfigField={updateConfigField}
                requestConfigUpdate={requestConfigUpdate}
                />
        );
    }
    return (
      <div>
        <div className={styles.backButton}>
          <Link to="/bots">
            <i className="fa fa-arrow-left fa-3x" />
          </Link>
        </div>
        <div className={styles.container}>
            <div className={styles.header}>
                BOT
            </div>
            <div className={styles.data}>
                State: {runState} <br/>
                Pid: 12345 <br/>
            </div>

            <div className={styles.botSelector}>
                <BotSelector
                    {...botChoice}
                    botId={id}
                    requestBotChoices={requestBotChoices}
                    selectBot={selectBot}
                    />
            </div>

            {configSection}

            <div className={styles.btnGroup}>
              <div className="button" onClick={requestStart}>
                START
              </div>
              <div className="button" onClick={requestStop}>
                STOP
              </div>
            </div>
        </div>
      </div>
    );
  }
}

export default Bot;
