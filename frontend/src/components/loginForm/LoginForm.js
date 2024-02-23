import React, { useState } from 'react';
import './LoginForm.css';
import { useNavigate } from "react-router-dom";
import { notification, Input } from 'antd';

const LoginForm = () => {
  const [api, contextHolder] = notification.useNotification();
  const [email, setEmail] = useState('');
  const [password, setPassword] = useState('');
  const [error, setError] = useState('');
  const navigate = useNavigate()

  const handleLogin = async (e) => {
    e.preventDefault();
    if (!email || !password) {
      notification.error({ message: 'Ha ocurrido un error', description: 'Comprueba que todos los campos esten rellenados', duration: 5 })
    } else {
      navigate("/Line");
    }
  };

  return (
    <div className="login-container">
      <form className="login-form" onSubmit={handleLogin}>
        <h2 className="login-title">Iniciar sesión</h2>
        <div className="form-group">
          <label className="form-label" htmlFor="email">Email</label>
          <Input
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
          <Input
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
        <p>¿No tienes una cuenta? <a href="/register">Regístrate aquí</a></p>
      </div>
    </div>
  );
};

export default LoginForm;