import React from 'react';
import './DatabaseCard.css';

const DatabaseCard = ({name, onOpen}) => {
    return (
        <div className='database-card' onClick={onOpen}>
            <h3>{name}</h3>
            <p>Last opened: 2 days ago</p>
        </div>
    );
};

export default DatabaseCard;