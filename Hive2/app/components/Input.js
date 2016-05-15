import React, { Component, PropTypes } from 'react';
import { Link } from 'react-router';

class Input extends Component {
    static propTypes = {
        label: PropTypes.string.isRequired,
        value: PropTypes.any,
        checked: PropTypes.boolean,
        type: PropTypes.string.isRequired,
        onChange: PropTypes.func,
    };

    render() {
        let newProps = {
            ...this.props,
            ref: (e) => this.domInput = e,
        };
        delete newProps.label
        return (
            <div className="inputField">
                <div className="inputLabel">
                    {this.props.label}
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
