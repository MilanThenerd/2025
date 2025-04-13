import React, { useState } from "react";
import "./LoginRegister.css";

const LoginRegister = ({ onLogin, onRegister }) => {
    const [isRegistering, setIsRegistering] = useState(false);
    const [username, setUsername] = useState("");
    const [email, setEmail] = useState("");
    const [password, setPassword] = useState("");
    const [error, setError] = useState("");

    const handleLoginSubmit = async (e) => {
        e.preventDefault();
        try {
            await onLogin(username, password);
        } catch (err) {
            setError(err.message || "Login failed");
        }
    };

    const handleRegisterSubmit = async (e) => {
        e.preventDefault();
        try {
            await onRegister(username, email, password);
            setIsRegistering(false);
        } catch (err) {
            setError(err.message || "Registration failed");
        }
    };

    return (
        <div className={`wrapper ${isRegistering ? "active" : ""}`}>
            {error && <div className="error-message">{error}</div>}
            
            {/* LOGIN FORM */}
            <div className={`form-box login ${isRegistering ? "hidden" : ""}`}>
                <form onSubmit={handleLoginSubmit}>
                    <h1>Login</h1>
                    <div className="input-box">
                        <input
                            type="text"
                            placeholder="Username"
                            value={username}
                            onChange={(e) => setUsername(e.target.value)}
                            required
                            className="inputField"
                        />
                    </div>

                    <div className="input-box">
                        <input
                            type="password"
                            placeholder="Password"
                            value={password}
                            onChange={(e) => setPassword(e.target.value)}
                            required
                            className="inputField"
                        />
                    </div>

                    <div className="remember-forgot">
                        <label>
                            <input type="checkbox" /> Remember me
                        </label>
                        <a href="#">Forgot password?</a>
                    </div>

                    <button type="submit" className="actionButton">Login</button>

                    <div className="register-link">
                        <p>
                            Don't have an account?{' '}
                            <a href="#" onClick={(e) => {
                                e.preventDefault();
                                setIsRegistering(true);
                                setError("");
                            }}>
                                Register
                            </a>
                        </p>
                    </div>
                </form>
            </div>

            {/* REGISTER FORM */}
            <div className={`form-box register ${isRegistering ? "" : "hidden"}`}>
                <form onSubmit={handleRegisterSubmit}>
                    <h1>Registration</h1>
                    <div className="input-box">
                        <input
                            type="text"
                            placeholder="Username"
                            value={username}
                            onChange={(e) => setUsername(e.target.value)}
                            required
                            className="inputField"
                        />
                    </div>

                    <div className="input-box">
                        <input
                            type="email"
                            placeholder="Email"
                            value={email}
                            onChange={(e) => setEmail(e.target.value)}
                            required
                            className="inputField"
                        />
                    </div>

                    <div className="input-box">
                        <input
                            type="password"
                            placeholder="Password"
                            value={password}
                            onChange={(e) => setPassword(e.target.value)}
                            required
                            className="inputField"
                        />
                    </div>

                    <div className="remember-forgot">
                        <label>
                            <input type="checkbox" required /> I agree to the terms and conditions
                        </label>
                    </div>

                    <button type="submit" className="actionButton">Register</button>

                    <div className="register-link">
                        <p>
                            Already have an account?{' '}
                            <a href="#" onClick={(e) => {
                                e.preventDefault();
                                setIsRegistering(false);
                                setError("");
                            }}>
                                Login
                            </a>
                        </p>
                    </div>
                </form>
            </div>
        </div>
    );
};

export default LoginRegister;