import React, { Component, PropTypes } from 'react';
import { Link } from 'react-router';
import styles from './Bot.css';

class Bot extends Component {
  static propTypes = {
    start: PropTypes.func.isRequired,
    started: PropTypes.func.isRequired,
    stop: PropTypes.func.isRequired,
    stopped: PropTypes.func.isRequired,
    runState: PropTypes.string.isRequired
  };

  render() {
    const { start, stop, runState } = this.props;
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
          <button className={styles.btn} onClick={start}>
            Start
          </button>
          <button className={styles.btn} onClick={stop}>
            Stop
          </button>
        </div>
      </div>
    );
  }
}

export default Bot;
