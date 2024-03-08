import React, { useState } from 'react';
import './FooterBusStop.css';
import user from '../../assets/icons/user.svg';
import bus from '../../assets/icons/bus.svg';
import hour from '../../assets/icons/hour.svg';
import chat from '../../assets/icons/chat.svg';
import { useNavigate, useParams, useLocation } from 'react-router-dom';
import { message, Popconfirm } from 'antd';

function FooterBusStop() {
  const navigate = useNavigate();
  const { idLine } = useParams();
  const location = useLocation();
  // const [selectedIcon, setSelectedIcon] = useState('');

  const confirm = () => {
    message.success('Se ha cerrado sesion');
    navigate('/');
  };

  const cancel = (e) => {
    console.log(e);
    message.error('Cancelado');
  };

  const handleIconClick = (icon) => {
    const { pathname } = location;
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

  const getMenuActiveItem = () => {
    const { pathname } = location;

    if (pathname === `/Line/${idLine}/schedule`) {
      return 'hour';
    } else if (pathname === `/Line/${idLine}/BusStop`) {
      return 'bus';
    } else if (pathname === `/Line/${idLine}/chat`) {
      return 'chat';
    }
    return 'hour';
  };

  return (
    <div className="bodyFooterSB">
      <div className={`icon`} onClick={() => handleIconClick('hour')}>
        <div className={getMenuActiveItem() === 'hour' ? 'active' : ''}>
          <img src={hour} alt="Horas" />
        </div>
        <p>Horario</p>
      </div>
      <div className={`icon`} onClick={() => handleIconClick('bus')}>
        <div className={getMenuActiveItem() === 'bus' ? 'active' : ''}>
          <img src={bus} alt="Paradas" />
        </div>
        <p>Paradas</p>

      </div>
      <div className={`icon`} onClick={() => handleIconClick('chat')}>
        <div className={getMenuActiveItem() === 'chat' ? 'active' : ''}>
          <img src={chat} alt="chat" />
        </div>
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
        <div className="iconuser">
          <img src={user} alt="Usuario" />
          <p>Usuario</p>
        </div>
      </Popconfirm>
    </div>
  );
}

export default FooterBusStop;


