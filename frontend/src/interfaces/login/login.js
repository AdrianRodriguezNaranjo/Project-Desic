import "./login.css";
import { useNavigate } from "react-router-dom";
import { useState } from 'react';
import { UserOutlined } from '@ant-design/icons';
import { EyeInvisibleOutlined, EyeTwoTone } from '@ant-design/icons';
import { Button, Input } from 'antd';
import UserService from "../../services/user/user.service";

function Login() {
  const nav = useNavigate();

  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");

  const [voidLoginError, setVoidError] = useState("");

  const navSingUp = () => {
    nav("/signup");
  }

  const validateInput = () => {
    if (!username && !password) {
      setVoidError("Faltan los campos");
      return false;
    } else if (!password) {
      setVoidError("Falta la contraseña");
      return false;
    } else if (!username) {
      setVoidError("Falta el usuario");
      return false;
    } else {
      setVoidError("");
      return true;
    }
  }

  const submitUser = async () => {
    if (validateInput()) {
      const response = await UserService.signin(username, password);
      if (response && response.access_token) {
        const accessToken = response.access_token;
        const isAdmin = response.isAdmin;

        localStorage.setItem("accessToken", accessToken);
        localStorage.setItem("isAdmin", isAdmin);

        nav("/busline");
      } else {
        setVoidError("Inicio de sesión fallido");
      }
    }
  }

  return (
    <div className="background-login">
      <div className="container-login">
        <h2>Log In</h2>
        <Input placeholder="Username" value={username} onChange={(e) => setUsername(e.target.value)}
          prefix={<UserOutlined />} />
        <Input.Password className="passwordInput"
          placeholder="Password"
          value={password} onChange={(e) => setPassword(e.target.value)}
          iconRender={(visible) => (visible ? <EyeTwoTone /> : <EyeInvisibleOutlined />)}
        />
        {voidLoginError && <div className="error-mesage">{voidLoginError}</div>}
        <Button className="buttonLoggin" type="primary" onClick={submitUser}>Log in</Button>
        <div className="question">¿No tienes cuenta?</div>
        <div className="signup" onClick={navSingUp}>Sign Up</div>
      </div>
    </div>
  );
}

export default Login;