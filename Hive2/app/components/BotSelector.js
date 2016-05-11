import React, { Component, PropTypes } from 'react';
import { Link } from 'react-router';
import styles from './BotSelector.css';


export default class BotSelector extends Component {

    static propTypes = {
        requestBotChoices: PropTypes.func.isRequired,
        selectBot: PropTypes.func.isRequired,

        botId: PropTypes.string.isRequired,

        bots: PropTypes.array.isRequired,
        selected: PropTypes.string.isRequired,
        updateState: PropTypes.string.isRequired,
        errorMessage: PropTypes.string
    }

    render() {
        const { bots, selected } = this.props
        const botOptions = bots.map(bot => {
            return (
                <option value={bot}>{bot}</option>
            );
        });
        // const errorSection = ();
        return (
            <div>
                <div className={styles.container}>
                    <select onChange={this.selectBot} value={selected}>
                        {botOptions}
                    </select>
                </div>
                {this.props.updateState}
            </div>
        );
    }

    selectBot(event) {
        const { selectBot, botId } = this.props;
        selectBuild(botId, event.target.value);
    }

    componentDidMount() {
        this.props.requestBotChoices(this.props.botId);
    }
}
