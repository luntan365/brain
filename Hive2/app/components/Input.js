import React, { Component, PropTypes } from 'react';
import { Link } from 'react-router';

class Input extends Component {
    static propTypes = {
        label: PropTypes.string.isRequired,
        value: PropTypes.any,
        type: PropTypes.string.isRequired,
        onChange: PropTypes.func,
    };

    render() {
        const { label, value, type, onChange } = this.props;
        let newProps = {
            ref: (e) => this.domInput = e,
            type: type,
        };
        if (value !== undefined) {
            newProps.value = value
        }
        if (onChange) {
            newProps.onChange = onChange;
        }
        return (
            <div className="inputField">
                <div className="inputLabel">
                    {label}
                </div>
                <div className="input">
                    <input {...newProps} />
                </div>
            </div>
        );
    }

    getValue() {
        return this.domInput.value;
    }

    onChange(e) {
        if (this.props.onChange) {
            this.props.onChange(e);
        }
    }
}

export default Input;
