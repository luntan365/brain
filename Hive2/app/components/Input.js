import React, { Component, PropTypes } from 'react';
import { Link } from 'react-router';

class Input extends Component {
    static propTypes = {
        label: PropTypes.string.isRequired,
        value: PropTypes.any.isRequired,
        type: PropTypes.func.isRequired,
        onChange: PropTypes.func,
    };

    render() {
        const { label, value, type, onChange } = this.props;
        return (
            <div className="inputField">
                <div className="inputLabel">
                    {label}
                </div>
                <div className="input">
                    <input
                        type={type}
                        value={value}
                        onChange={(e) => this.onChange(e)}/>
                </div>
            </div>
        );
    }

    onChange(e) {
        if (this.props.onChange) {
            this.props.onChange(e);
        }
    }
}

export default Input;
