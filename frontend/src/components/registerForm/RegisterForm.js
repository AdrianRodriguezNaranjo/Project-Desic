import React, { useState } from 'react';
import axios from 'axios';
import './RegisterForm.css';
import { useNavigate } from "react-router-dom";

const RegisterForm = () => {
  const [name, setName] = useState('');
  const [email, setEmail] = useState('');
  const [password, setPassword] = useState('');
  const [error, setError] = useState('');
  const navigate = useNavigate()

  return (
    <div className="register-container">
      <form className="register-form">
        <h2 className="register-title">Registrarse</h2>

        <div className="form-group">
          <label className="form-label" htmlFor="name">Nombre</label>
          <input
            type="name"
            id="name"
            name="name"
            className="form-input"
            value={name}
            onChange={(e) => setName(e.target.value)}
          />
          {error && error.includes('name') && <p className="error-message">{error}</p>}
        </div>

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
        <button type="submit" className="register-button" onClick={() => navigate("/Line")}>Registrarse</button>
      </form>
      <div className="register-footer">
        <p>¿Tienes una cuenta? <a href="/login">Inicia sesión aquí</a></p>
      </div>
    </div>
  );
};

export default RegisterForm