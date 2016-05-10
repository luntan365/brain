import React, { Component, PropTypes } from 'react';
import { Link } from 'react-router';
import styles from './Bot.css';

class Bot extends Component {
  static propTypes = {
    requestStart: PropTypes.func.isRequired,
    requestStop: PropTypes.func.isRequired,
    runState: PropTypes.string.isRequired
  };

  render() {
    const { requestStart, requestStop, runState } = this.props;
    return (
      <div>
        <div className={styles.backButton}>
          <Link to="/">
            <i className="fa fa-arrow-left fa-3x" />
          </Link>
        </div>
        <div className={`counter ${styles.counter}`}>
          {runState}
        </div>
        <div className={styles.btnGroup}>
          <button className={styles.btn} onClick={requestStart}>
            Start
          </button>
          <button className={styles.btn} onClick={requestStop}>
            Stop
          </button>
        </div>
      </div>
    );
  }
}

export default Bot;
