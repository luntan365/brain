import React, { Component, PropTypes } from 'react';
import { Link } from 'react-router';
import Input from './Input'
import styles from './NpcConfig.css'

class NpcConfig extends Component {

    static propTypes = {
        botId: PropTypes.string.isRequired,
        npcKey: PropTypes.string.isRequired,
        gameState: PropTypes.object.isRequired,
        enabled: PropTypes.bool.isRequired,
        npcId: PropTypes.number.isRequired,
        x: PropTypes.number.isRequired,
        y: PropTypes.number.isRequired,
        z: PropTypes.number.isRequired,
    };

    render() {
        const { enabled, npcKey, npcId, x, y, z } = this.props;
        const prefix = npcKey.charAt(0).toUpperCase() + npcKey.slice(1);
        return (
            <div className={styles.npcGroup}>
                <div className={styles.npcHeader}>
                    {prefix}
                </div>
                <div className={styles.npcFields}>
                    <Input
                        label="Enabled?"
                        type="checkbox"
                        checked={enabled}
                        onChange={e => this.updateEnabled(e)}
                        />
                    <Input 
                        label={`${prefix} Id`} 
                        type="text" 
                        value={npcId}
                        onChange={e => this.updateField(e, 'npcId')}
                        />
                    <Input 
                        label={`${prefix} X`} 
                        type="text" 
                        value={x}
                        onChange={e => this.updateField(e, 'x')}
                        />
                    <Input 
                        label={`${prefix} Y`} 
                        type="text" 
                        value={y}
                        onChange={e => this.updateField(e, 'y')}
                        />
                    <Input 
                        label={`${prefix} Z`} 
                        type="text" 
                        value={z}
                        onChange={e => this.updateField(e, 'z')}
                        />
                </div>
                <div className={styles.npcButtons}>
                    <button
                        type="submit"
                        className="flexButton"
                        onClick={e => this.onSetFromTarget(e)}>
                        USE TARGET
                    </button>
                </div>
            </div>
        );
    }

    updateEnabled(e) {
        const value = e.target.checked;
        this.props.updateNpc(
            this.props.botId,
            this.props.npcKey,
            {
                enabled: value,
                npcId: this.props.npcId,
                x: this.props.x,
                y: this.props.y,
                z: this.props.z,
            }
        );
    }

    updateField(e, field) {
        const value = Number(e.target.value);
        if (value) {
            this.props.updateNpc(
                this.props.botId, 
                this.props.npcKey, 
                {
                    enabled: this.props.enabled,
                    npcId: this.props.npcId,
                    x: this.props.x,
                    y: this.props.y,
                    z: this.props.z,
                    [field]: value,
                }
            );
        }
    }

    onSetFromTarget(e) {
        let target = this.props.gameState.target;
        if (target) {
            this.props.updateNpc(
                this.props.botId,
                this.props.npcKey,
                {
                    enabled: this.props.enabled,
                    npcId: target.id,
                    x: target.position.x,
                    y: target.position.y,
                    z: target.position.z,
                }
            );
        }
    }

}

export default NpcConfig;
