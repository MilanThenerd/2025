import React from 'react';
import './Navbar.css';

const Navbar = () => {
return(
    <header className ="navbar">
        <div className='logo'>VAULT7</div>
        <div className='user-profile'>
            {/* <img src="profile-icon.png" alt="profile"/> */}
        </div>
    </header>
);

};

export default Navbar;