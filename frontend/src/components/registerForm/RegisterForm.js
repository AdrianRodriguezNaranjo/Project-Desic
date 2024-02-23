import React, { useState } from 'react';
import './RegisterForm.css';
import { useNavigate } from "react-router-dom";
import UserService from '../../services/usersService/UsersService';
import { notification, Input } from 'antd';

const RegisterForm = () => {
  const [api, contextHolder] = notification.useNotification();
  const [name, setName] = useState('');
  const [email, setEmail] = useState('');
  const [password, setPassword] = useState('');
  const [error, setError] = useState('');
  const navigate = useNavigate()

  const handleRegister = async (e) => {
    e.preventDefault();
    if (!name || !email || !password) {
      notification.error({ message: 'Ha ocurrido un error', description: 'Comprueba que todos los campos esten rellenados', duration: 5 })
    } else {
      try {
        const combinedData = `${email}:${password}`;

        const newUser = {
          name: name,
          credentials: combinedData
        };

        await UserService.create(newUser);
        navigate("/Line"); // Navegar solo si la creación del usuario fue exitosa
      } catch (error) {
        setError('Error al registrar usuario');
      }
    }
  };

  return (
    <div className="register-container">
      <form className="register-form" onSubmit={handleRegister}>
        <h2 className="register-title">Registrarse</h2>
        <div className="form-group">
          <label className="form-label" htmlFor="name">Nombre</label>
          <Input
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
        <button type="submit" className="register-button">Registrarse</button>
      </form>
      <div className="register-footer">
        <p>¿Tienes una cuenta? <a href="/login">Inicia sesión aquí</a></p>
      </div>
    </div>
  );
};

export default RegisterForm;
