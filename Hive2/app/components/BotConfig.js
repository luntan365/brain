import React, { Component, PropTypes } from 'react';
import { Link } from 'react-router';
import styles from './BotConfig.css';
import ConfigField from './ConfigField';

class BotConfig extends Component {
    static propTypes = {
        id: PropTypes.string.isRequired,
        config: PropTypes.object.isRequired,
        updateConfigField: PropTypes.func.isRequired,
        requestConfigUpdate: PropTypes.func.isRequired,
    };

    render() {
      const { id, config, updateConfigField } = this.props;
      let configFields = Object.keys(config.config).map(key => {
          let value = config.config[key];
          return (
              <ConfigField
                botId={id}
                configKey={key}
                value={value}
                updateConfigField={updateConfigField}
                />
          );
      });
      return (
          <div className={styles.configGroup}>
              <div className={styles.configHeader}>
                SETTINGS
              </div>
              <form onSubmit={(e) => this.onSubmit(e)}>
                  {configFields}
                  <div className={styles.submitButton}>
                      <button
                          type="submit"
                          className={`button`}>
                          SUBMIT
                      </button>
                  </div>
              </form>
          </div>
      );
    }

    onSubmit(e) {
        e.preventDefault();

        this.props.requestConfigUpdate(
            this.props.id,
            this.props.config.config
        );
    }
}

export default BotConfig;
