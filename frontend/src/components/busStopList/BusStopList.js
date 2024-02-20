import './BusStopList.css';
import addbutton from "../../assets//icons/addbutton.svg";
import { useNavigate, useParams } from "react-router-dom"
import BusStopService from "../../services/busStopService/BusStopService"
import React, { useState, useEffect } from "react";
import { Button, message, Popconfirm } from 'antd';
import update from '../../assets/icons/update.svg';
import deletea from '../../assets/icons/delete.svg';

const BusStopList = () => {
  const [busStop, setBusStop] = useState([]);

  const navigate = useNavigate();
  const { idLine } = useParams();

  const retrieveBusStop = async () => {
    try {
      const response = await BusStopService.getAll(idLine);
      setBusStop(response);
    } catch (error) {
      console.log(error);
    }
  };

  const confirm = (id) => {
    console.log(id);
    message.success('Se ha eliminado');
    deleteBusStop(id)
  };

  const cancel = (e) => {
    console.log(e);
    message.error('Cancelado');
  };

  useEffect(() => {
    retrieveBusStop();
  }, []);

  const deleteBusStop = (id) => {
    BusStopService.remove(id,idLine).then(response => {
      retrieveBusStop();
    })
    retrieveBusStop();
  }

  const updateBusStop = (b) => {
    localStorage.setItem("busStop", JSON.stringify(b))
    navigate(`/Line/${idLine}/updateBusStop`)
  }

  return (
    <div className='endofthepage'>
    <div className="listBodyBusStop">
      {busStop.map((b, index) => {
        return (
          <div key={index} className="elementBody">
            <div className='textContainer'>
            <h3>Localización:</h3>
            <p> {b.location}</p>
            </div>
            {/* <img src={`http://localhost:8080/images/${b.filename}`}></img> */}
            <Popconfirm
              title="Eliminar Linea"
              description="Estas seguro de que quieres eliminar esta parada?"
              onConfirm={() => confirm(b.id)}
              onCancel={() => cancel()}
              okText="Si"
              cancelText="No">
              <Button className="delButton"><img src={deletea} alt="eliminar" /><br/>Eliminar</Button>
            </Popconfirm>
            <Button onClick={() => updateBusStop(b)} className="updButton"><img src={update} alt="Actualizar" /><br/>Actualizar</Button>
          </div>
        )
      })}
      <img src={addbutton} alt="Añadir" onClick={() => navigate(`/Line/${idLine}/addBusStop`)} className="buttonAdd" />
    </div>
    </div>
  )
}

export default BusStopList;