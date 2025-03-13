import React, { useState } from "react";
import './LoginRegister.css'
import { FaUser, FaLock} from "react-icons/fa";
import { MdEmail } from "react-icons/md";

//added by Tambi
//import apiLibrary from the jsLibrary branch
import APIWrapper from "../client";
//create api object with all APIWrapper functions
const api = new APIWrapper("api-url");



const LoginRegister =({onLogin}) => {
    // const handleLogin = () =>{
    //     //call API validation
    //     onLogin();
    // };

    const [action,setAction] = useState('');
    const [username, setUsername] = useState('');
    const [email, setEmail] = useState('');
    const [password, setPassword] = useState('');

    //added by tambi
    // hold error messages
    const [errorMessage, setErrorMessage] = useState('');
    
    //added by tambi
    //try connect to api
    //made async to allow await
    const handleLoginSubmit = async (e) => {
        e.preventDefault(); // Prevent page reload
        setErrorMessage("");// clear prev errors

        try{
            const response = await api.loginUser({username, password});
            if(response){
                console.log('Logging in with:', username, password);
                onLogin(); // Call the onLogin function passed from App.jsx
            }
            else{
                console.error("Invalid username or password");
                setErrorMessage("Invalid username or password");
            }
        }catch (error){
            console.error("Login error: ", error);
            setErrorMessage("An error occurred while logging in.");
        }   
    };

    //added by tambi
    //try connect to api
    //made async to allow await
    const handleRegisterSubmit = async (e) => {
        e.preventDefault(); // Prevent page reload
        setErrorMessage("");// clear prev errors

        try{
            const response = await api.registerUser(username, email, password);
            if(response){
                console.log('Registering with:', username, email, password);
                alert('Registration successful! Please log in.');
                setAction(''); // Switch to login view after registration
            }
            else{
                console.error("Invalid username, password or email");
                setErrorMessage("Invalid username, password or email");
            }
        }catch(error){
            console.error("Registration error: ", error);
            setErrorMessage("An error occurred while registering in.");
        }
    };

    // const registerLink = () => {
    //     setAction(' active');
    // };

    // const loginLink = () => {
    //     setAction('');
    // };

    return (
        <div className={`wrapper${action}`}>

            {/* LOGIN FORM */}
            <div className="form-box login">
                <form onSubmit={handleLoginSubmit}>
                    <h1>Login</h1>
                    <div className = "input-box">
                        <input type="text" placeholder="Username" value ={username} onChange={(e) => setUsername(e.target.value)} required></input>
                        <FaUser className="icon"/>
                    </div>

                    <div className = "input-box">
                        <input type="password" placeholder="Password" value={password} onChange={(e) => setPassword(e.target.value)} required></input>
                        <FaLock className="icon"/>
                    </div>

                    <div className="remember-forgot">
                        <label>
                            <input type="checkbox" />Remember me
                        </label>
                        <a href="#">Forgot password?</a>
                    </div>

                    <button type = "submit">Login</button>

                    <div className="register-link">
                        <p>Dont have an account?
                            <a href="#" onClick={() => setAction('active')}>Register</a>
                        </p>
                    </div>
                </form>
            </div>

    {/* REGISTER FORM */}

            <div className="form-box register">
                <form onSubmit={handleRegisterSubmit}>
                    <h1>Registration</h1>
                    <div className = "input-box">
                        <input type="text" placeholder="Username" value={username} onChange={(e) => setUsername(e.target.value)} required></input>
                        <FaUser className="icon"/>
                    </div>

                    <div className = "input-box">
                        <input type="email" placeholder="Email" value={email} onChange={(e) => setEmail(e.target.value)} required></input>
                        <MdEmail className="icon"/>
                    </div>

                    <div className = "input-box">
                        <input type="password" placeholder="Password" value={password} onChange={(e) => setPassword(e.target.value)} required></input>
                        <FaLock className="icon"/>
                    </div>

                    <div className="remember-forgot">
                        <label>
                            <input type="checkbox" />I agree to the terms and conditions
                        </label>
                    </div>

                    <button type = "submit">Register</button>

                    <div className="register-link">
                        <p>Already have an account?
                            <a href="#" onClick={() => setAction('')}>Login</a>
                        </p>
                    </div>
                </form>
            </div>
        </div>
    );
};

export default LoginRegister;