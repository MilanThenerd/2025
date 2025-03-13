import React from 'react';
import Navbar from '../Shared/Navbar/Navbar';
import Button from '../Shared/Button/Button';
import DatabaseCard from '../Shared/DatabaseCard/DatabaseCard';
import './HomePage.css';

const HomePage = ({onLogout}) => {
    const handleLogout = () =>{
        //API call here
        onLogout();
    };

    const recentlyOpenedDatabases = [
        {id: 1, name: 'example database 1'},
        {id: 2, name: 'example database 2'},
        {id: 3, name: 'example database 3'}
    ];

const handleCreateNewDatabase = () => {
    console.log('Create New Database clicked');
};

const handleOpenDatabase = (database) => {
    console.log('Open Database: ', database.name);
};

return (
    <div className='homepage'>
        <Navbar />
        <main className='main-content'>
            <div className='quick-actions'>
                <Button onClick={handleCreateNewDatabase}>Create New Database</Button>
                <Button>Open Collection</Button>
                <Button>Create New Collection</Button>
                <Button>Open Database</Button>
            </div>
            <div className='recent-databases'>
                <h2>Recently Opened Databases</h2>
                <div className='database-list'>
                    {recentlyOpenedDatabases.map((db) => (
                        <DatabaseCard
                        key = {db.id}
                        name = {db.name}
                        onOpen={() => handleOpenDatabase(db)}
                        />
                    ))}
                </div>
            </div>
        </main>
    </div>
);

};

export default HomePage;