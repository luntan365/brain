import React, { Component, PropTypes } from 'react';
import { Link } from 'react-router';
import styles from './Bot.css';
import BotSummary from '../containers/BotSummaryPage';

class Bot extends Component {
  static propTypes = {
    requestStart: PropTypes.func.isRequired,
    requestStop: PropTypes.func.isRequired,
    runState: PropTypes.string.isRequired,
    notFound: PropTypes.bool.isRequired
  };

  render() {
    const {
        id,
        requestStart,
        requestStop,
        runState,
        notFound,
    } = this.props;
    if (notFound || notFound === undefined) {
        return <BotSummary />
    }
    var tabs = [
        {
            title: "Overview",
            page: ""
        },
        {
            title: "Config",
            page: "config"
        },
        {
            title: "Profile",
            page: "profile"
        }
    ].map(obj => {
        let url = `/bots/${id}/${obj.page}`;
        return (
            <Link to={url}>
                <div className={styles.tab}>
                    {obj.title}
                </div>
            </Link>
        );
    });
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

            <div className={styles.tabGroup}>
                {tabs}
            </div>

            <div className={styles.paneContainer}>
                {this.props.children}
            </div>

            <div className={styles.btnGroup}>
              <div className="button" onClick={() => requestStart(id)}>
                START
              </div>
              <div className="button" onClick={() => requestStop(id)}>
                STOP
              </div>
            </div>
        </div>
      </div>
    );
  }
}

export default Bot;
