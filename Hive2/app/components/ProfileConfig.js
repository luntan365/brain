import React, { Component, PropTypes } from 'react';
import { Link } from 'react-router';
import styles from './ProfileConfig.css';
import Input from './Input';
import NpcConfig from './NpcConfig';
import { openJsonFile, saveJsonFile } from '../utils/Dialog';

class ProfileConfig extends Component {
    static propTypes = {
        botId: PropTypes.string.isRequired,
        gameState: PropTypes.object.isRequired,
        positions: PropTypes.array.isRequired,
        addX: PropTypes.number,
        addY: PropTypes.number,
        addZ: PropTypes.number,
        addProfilePosition: PropTypes.func.isRequired,
        removeProfilePositions: PropTypes.func.isRequired,
        setProfile: PropTypes.func.isRequired,
    };

    render() {
        const { addX, addY, addZ, positions } = this.props;
        const positionOptions = positions.map((p, i) => {
            return (
                <option value={i}>
                    {p.x}, {p.y}, {p.z}
                </option>
            );
        });
        return (
            <div className={styles.profileContainer}>
                <div className={styles.profileHeader}>
                    PROFILE
                </div>
                <div className={`row ${styles.profileRow}`}>
                    <div className={styles.profileList}>
                        <select 
                            multiple="multiple" 
                            size="10"
                            ref={(c) => this.profileList = c}
                            >
                            {positionOptions}
                        </select>
                    </div>
                    <div className={styles.profileAdd}>
                        <Input label="x" type="text" ref={(e) => this.addX = e}/>
                        <Input label="y" type="text" ref={(e) => this.addY = e}/>
                        <Input label="z" type="text" ref={(e) => this.addZ = e}/>
                    </div>
                </div>
                <div className={styles.npcSection}>
                    <NpcConfig prefix="Repair"/>
                    <NpcConfig prefix="Sell"/>
                    <NpcConfig prefix="Restock"/>
                </div>
                <div className={styles.buttonGroup}>
                    <button
                        type="submit"
                        className="flexButton"
                        onClick={e => this.onAdd(e)}>
                        ADD
                    </button>
                    <button
                        type="submit"
                        className="flexButton"
                        onClick={e => this.onAddCurrentPosition(e)}>
                        ADD CURRENT POSITION
                    </button>
                    <button
                        type="submit"
                        className="flexButton"
                        onClick={e => this.onRemove(e)}>
                        REMOVE
                    </button>
                    <button
                        type="submit"
                        className="flexButton"
                        onClick={e => this.onSave(e)}>
                        SAVE
                    </button>
                    <button
                        type="submit"
                        className="flexButton"
                        onClick={e => this.onLoad(e)}>
                        LOAD
                    </button>
                </div>
            </div>
        );
    }

    onAdd(e) {
        const { botId, addProfilePosition } = this.props;
        const x = Number(this.addX.getValue());
        const y = Number(this.addY.getValue());
        const z = Number(this.addZ.getValue());
        if (x && y && z) {
            addProfilePosition(botId, x, y, z);
        }
    }

    onAddCurrentPosition(e) {
        const { botId, gameState, addProfilePosition } = this.props;
        if (gameState.in_game) {
            const { x, y, z } = gameState.player_position;
            addProfilePosition(botId, x, y, z);
        }
    }

    onRemove(e) {
        let indicies = [];
        const options = this.profileList.options;
        for (let i = 0; i < options.length; i++) {
            if (options[i].selected) {
                indicies.push(i);
            }
        }
        this.props.removeProfilePositions(this.props.botId, indicies);
    }

    onLoad(e) {
        openJsonFile().then(data => {
            this.props.setProfile(this.props.botId, data);
        });
    }

    onSave(e) {
        saveJsonFile({
            positions: this.props.positions
        });
    }
}

export default ProfileConfig;
