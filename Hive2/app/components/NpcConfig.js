import React, { Component, PropTypes } from 'react';
import { Link } from 'react-router';
import Input from './Input'
import styles from './NpcConfig.css'

class NpcConfig extends Component {

    static propTypes = {
        npcKey: PropTypes.string.isRequired,
        gameState: PropTypes.objects.isRequired,
    };

    render() {
        const { npcKey } = this.props;
        const prefix = npcKey.charAt(0).toUpperCase() + npcKey.slice(1);
        return (
            <div className={styles.npcGroup}>
                <div className={styles.npcHeader}>
                    {prefix}
                </div>
                <div className={styles.npcFields}>
                    <Input 
                        label={`${prefix} Id`} 
                        type="text" 
                        ref={e => this.npcId = e} 
                        />
                    <Input 
                        label={`${prefix} X`} 
                        type="text" 
                        ref={e => this.x = e} 
                        />
                    <Input 
                        label={`${prefix} Y`} 
                        type="text" 
                        ref={e => this.y = e} 
                        />
                    <Input 
                        label={`${prefix} Z`} 
                        type="text" 
                        ref={e => this.z = e} 
                        />
                </div>
                <div className={styles.npcButtons}>
                    <button
                        type="submit"
                        className="flexButton"
                        onClick={e => this.onSave(npcKey)}>
                        SAVE
                    </button>
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

    getValue() {
        return {
            id: this.npcId.value,
            x: this.npcId.x,
            y: this.npcId.y,
            z: this.npcId.z,
        };
    }

    onSave(key) {
        this.props.updateNpc(this.props.botId, key, this.getValue())
    }

    onSetFromTarget(e) {

    }

}

export default NpcConfig;
