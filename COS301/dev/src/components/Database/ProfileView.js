import React, { useState, useEffect } from 'react';

const ProfileView = ({ onCancel, onUpdateUser, onGetUser, username }) => {
  const [userData, setUserData] = useState({
    username: '',
    email: ''
  });
  const [newUsername, setNewUsername] = useState('');
  const [newEmail, setNewEmail] = useState('');
  const [isEditing, setIsEditing] = useState(false);
  const [error, setError] = useState(null);
  const [success, setSuccess] = useState(false);
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    const fetchUserData = async () => {
      try {
        setLoading(true);
        const response = await onGetUser(username);
        console.log("User data:", response);
        
        const user = response.user || response;
        
        setUserData({
          username: user.username || user.name,
          email: user.email
        });
        setNewUsername(user.username || user.name);
        setNewEmail(user.email);
      } catch (err) {
        setError(err.message || 'Failed to load user data');
      } finally {
        setLoading(false);
      }
    };

    if (username) {
      fetchUserData();
    }
  }, [username]);

  const handleSave = async () => {
    if (newUsername.trim() === '') {
      setError('Username cannot be empty');
      return;
    }

    if (newEmail.trim() === '') {
      setError('Email cannot be empty');
      return;
    }

    // Simple email validation
    if (!/^[^\s@]+@[^\s@]+\.[^\s@]+$/.test(newEmail)) {
      setError('Please enter a valid email address');
      return;
    }

    if (newUsername === userData.username && newEmail === userData.email) {
      setIsEditing(false);
      return;
    }

    try {
      await onUpdateUser(username, { 
        username: newUsername,
        email: newEmail
      });
      setSuccess(true);
      setUserData({
        username: newUsername,
        email: newEmail
      });
      setTimeout(() => {
        setSuccess(false);
        setIsEditing(false);
      }, 1500);
    } catch (err) {
      setError(err.message || 'Failed to update profile');
    }
  };

  const handleCancel = () => {
    setNewUsername(userData.username);
    setNewEmail(userData.email);
    setIsEditing(false);
    setError(null);
    setSuccess(false);
    onCancel();
  };

  if (loading) {
    return <div className="tableTitle">Loading user data...</div>;
  }

  if (error && !isEditing) {
    return (
      <div className="profile-view">
        <div className="error-message">{error}</div>
        <button onClick={onCancel} className="toggleButton">
          Close
        </button>
      </div>
    );
  }

  return (
    <div className="profile-view">
      {isEditing ? (
        <div className="edit-mode">
          <h2 className='tableTitle'>Edit Profile</h2>
          {error && <div className="error-message">{error}</div>}
          {success && <div className="tableTitle">Profile updated successfully!</div>}
          <div className='tableTitle'>Username:</div>
          <div className="form-group">
            <input
              id="username"
              type="text"
              value={newUsername}
              onChange={(e) => {
                setNewUsername(e.target.value);
                setError(null);
              }}
              placeholder="Enter new username"
              className='inputField'
            />
          </div>
          <div className='tableTitle'>Email:</div>
          <div className="form-group">
            <input
              id="email"
              type="email"
              value={newEmail}
              onChange={(e) => {
                setNewEmail(e.target.value);
                setError(null);
              }}
              placeholder="Enter new email"
              className='inputField'
            />
          </div>
          <div className="button-group">
            <button onClick={handleSave} className="toggleButton">
              Save
            </button>
            <button onClick={handleCancel} className="toggleButton">
              Cancel
            </button>
          </div>
        </div>
      ) : (
        <div>
          <h2 className='tableTitle'>Profile</h2>
          <div className="tableTitle">
            <div>Username: {userData.username}</div>
            <div>Email: {userData.email}</div>
          </div>
          <div className="button-group">
            <button onClick={() => setIsEditing(true)} className="toggleButton">
              Edit
            </button>
            <button onClick={onCancel} className="toggleButton">
              Close
            </button>
          </div>
        </div>
      )}
    </div>
  );
};

export default ProfileView;