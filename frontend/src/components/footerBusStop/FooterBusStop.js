import React, { useState } from 'react';
import './FooterBusStop.css';
import user from '../../assets/icons/user.svg';
import bus from '../../assets/icons/bus.svg';
import hour from '../../assets/icons/hour.svg';
import chat from '../../assets/icons/chat.svg';
import { useNavigate, useParams } from 'react-router-dom';
import { message, Popconfirm } from 'antd';

function FooterBusStop() {
  const navigate = useNavigate();
  const { idLine } = useParams();
  // const [selectedIcon, setSelectedIcon] = useState('');

  const confirm = () => {
    message.success('Se ha cerrado sesion');
    window.location.reload();
  };

  const cancel = (e) => {
    console.log(e);
    message.error('Cancelado');
  };

  const handleIconClick = (icon) => { 
    if (icon === 'hour') {
      navigate(`/Line/${idLine}/schedule`);
      // setSelectedIcon(icon);
      // console.log(selectedIcon);
    } else if (icon === 'bus') {
      navigate(`/Line/${idLine}/BusStop`);
      // setSelectedIcon(icon);
      // console.log(selectedIcon);
    } else if (icon === 'chat') {
      navigate(`/Line/${idLine}/chat`);
      // setSelectedIcon(icon);
      // console.log(selectedIcon);
    }
  };

  return (
    <div className="bodyFooterSB">
      <div className={`icon`} onClick={() => handleIconClick('hour')}>
        <img src={hour} alt="Horas" />
        <p>Horario</p>
      </div>
      <div className={`icon`} onClick={() => handleIconClick('bus')}>
        <img src={bus} alt="Paradas" />
        <p>Paradas</p>
      </div>
      <div className={`icon`} onClick={() => handleIconClick('chat')}>
        <img src={chat} alt="chat" />
        <p>Chat</p>
      </div>
      <Popconfirm
        title="Cerrar sesion"
        description="Estas seguro que quieres Cerrar sesion?"
        onConfirm={() => confirm()}
        onCancel={() => cancel()}
        okText="Si"
        cancelText="No"
      >
        <div className="icon">
          <img src={user} alt="Usuario" />
          <p>Usuario</p>
        </div>
      </Popconfirm>
    </div>
  );
}

export default FooterBusStop;


