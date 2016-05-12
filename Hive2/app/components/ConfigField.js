import React, { Component, PropTypes } from 'react';
import { Link } from 'react-router';
import styles from './BotConfig.css';
import Input from './Input';

String.prototype.capitalize = function() {
    return this.replace(/(?:^|\s)\S/g, function(a) { return a.toUpperCase(); });
};

class ConfigField extends Component {
    static propTypes = {
        botId: PropTypes.string.isRequired,
        configKey: PropTypes.string.isRequired,
        value: PropTypes.any.isRequired,
        updateConfigField: PropTypes.func.isRequired
    };

    render() {
        const { configKey, value } = this.props;
        let formattedKey = configKey.replace(/_/g, " ").capitalize();
        let inputType = 'text';
        switch (typeof value) {
            case 'string':
                inputType = 'text';
                break;
            case 'bool':
                inputType = 'checkbox';
                break;
            case 'number':
                inputType = 'number';
                break;
        }
        return (
            <Input
                label={formattedKey}
                value={value}
                type={inputType}
                onChange={(e) => this.onChange(e)}
                />
        );
    }

    onChange(e) {
        let newValue = this.props.value.constructor(e.target.value);
        this.props.updateConfigField(
            this.props.botId,
            this.props.configKey,
            newValue
        );
    }
}

export default ConfigField;
