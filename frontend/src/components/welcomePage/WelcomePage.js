import './WelcomePage.css'
import { useNavigate } from "react-router-dom";

const WelcomePage = () => {
  const navigate = useNavigate()

  return (
    <div className="bodyRC">
      <div className='textRL'>
        <h1>Bienvenido</h1>
        <h3>Que deseas hacer</h3>
      </div>
      <div className="loginButtonRL" onClick={() => navigate("/login")}>
        <p>Iniciar sesion</p>
      </div>
      <div className="registerButtonRL" onClick={() => navigate("/register")}>
        <p>Registrarse</p>
      </div>
      <div className='helplink'>
        <p>
        <a href="/Documentacion_de_ayuda/Aplicacion Guaguas usuario.html" target="Blank">
          ¿Necesitas ayuda?
        </a>
        </p>
      </div>
    </div>
  );
}

export default WelcomePage