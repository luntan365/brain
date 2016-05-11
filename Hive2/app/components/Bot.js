import React, { Component, PropTypes } from 'react';
import { Link } from 'react-router';
import styles from './Bot.css';
import BotSelector from './BotSelector';

class Bot extends Component {
  static propTypes = {
    requestStart: PropTypes.func.isRequired,
    requestStop: PropTypes.func.isRequired,
    getConfig: PropTypes.func.isRequired,
    runState: PropTypes.string.isRequired,
    config: PropTypes.object.isRequired
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
        selectBot
    } = this.props;
    let configFields;
    if (config) {
        configFields = Object.keys(config).map(key => {
            let value = config[key];
            return (
                <div className={styles.configField}>
                    <div className={styles.configLabel}>
                        {key}
                    </div>
                    <div className={styles.configInput}>
                        <input type="text" value={value}/>
                    </div>
                </div>
            );
        });
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

            <div className={styles.configGroup}>
                {configFields}
            </div>

            <div className={styles.btnGroup}>
              <div className={styles.button} onClick={requestStart}>
                START
              </div>
              <div className={styles.button} onClick={requestStop}>
                STOP
              </div>
            </div>
        </div>
      </div>
    );
  }
}

export default Bot;
