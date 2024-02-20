import React, { useState } from 'react';
import axios from 'axios';
import './Login.css';

const Login = () => {
  const [email, setEmail] = useState('');
  const [password, setPassword] = useState('');
  const [error, setError] = useState('');

  return (
    <div className="login-container">
      <form className="login-form">
        <h2 className="login-title">Iniciar sesión</h2>
        <div className="form-group">
          <label className="form-label" htmlFor="email">Email</label>
          <input
            type="email"
            id="email"
            name="email"
            className="form-input"
            value={email}
            onChange={(e) => setEmail(e.target.value)}
          />
          {error && error.includes('email') && <p className="error-message">{error}</p>}
        </div>
        <div className="form-group">
          <label className="form-label" htmlFor="password">Contraseña</label>
          <input
            type="password"
            id="password"
            name="password"
            className="form-input"
            value={password}
            onChange={(e) => setPassword(e.target.value)}
          />
          {error && error.includes('password') && <p className="error-message">{error}</p>}
        </div>
        <button type="submit" className="login-button">Iniciar sesión</button>
      </form>
      <div className="login-footer">
        <p>¿No tienes una cuenta? <a href="/signup">Regístrate aquí</a></p>
      </div>
    </div>
  );
};

export default Login;