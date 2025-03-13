import { useState } from 'react';
import { BrowserRouter as Router, Routes, Route, Navigate } from 'react-router-dom';
import LoginRegister from './Components/LoginRegister/LoginRegister';
import HomePage from './Components/HomePage/HomePage';
import './App.css';

function App() {
  const [isLoggedIn, setIsLoggedIn] = useState(false);

  // Function to handle login
  const handleLogin = () => {
    setIsLoggedIn(true);
  };

  // Function to handle logout
  const handleLogout = () => {
    setIsLoggedIn(false);
  };

  return (
    <Router>
      <Routes>
        {/* Route for Login/Register */}
        <Route
          path="/login"
          element={
            isLoggedIn ? (
              <Navigate to="/" /> // Redirect to homepage if already logged in
            ) : (
              <LoginRegister onLogin={handleLogin} />
            )
          }
        />

        {/* Route for HomePage */}
        <Route
          path="/"
          element={
            isLoggedIn ? (
              <HomePage onLogout={handleLogout} />
            ) : (
              <Navigate to="/login" /> // Redirect to login if not logged in
            )
          }
        />
      </Routes>
    </Router>
  );
}

export default App;