import './BusStopList.css';
import addbutton from "../../assets//icons/addbutton.svg";
import { useNavigate, useParams } from "react-router-dom"
import BusStopService from "../../services/busStopService/BusStopService"
import React, { useState, useEffect } from "react";
import { Button, message, Popconfirm } from 'antd';

const BusStopList = () => {
  const [busStop, setBusStop] = useState([]);

  const navigate = useNavigate();
  const { idLine } = useParams();


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

  const retrieveBusStop = () => {
    BusStopService.getAll(idLine)
      .then(response => {
        setBusStop(response.data);
      })
      .catch(e => {
        console.log(e);
      });
  };

  const deleteBusStop = (id) => {
    BusStopService.remove(id).then(response => {
      retrieveBusStop();
    })
    BusStopService.getAll()
  }

  const updateBusStop = (b) => {
    localStorage.setItem("busStop", JSON.stringify(b))
    navigate(`/Line/${idLine}/updateBusStop`)
  }

  return (
    <div className="container">
      {busStop.map((b, index) => {
        return (
          <div key={index} className="elementBody">
            <h3>Paradas</h3>
            <p>Localizacion: {b.Location}</p>
            <img src={`http://localhost:8080/images/${b.filename}`}></img>
            <Popconfirm
              title="Eliminar Linea"
              description="Estas seguro de que quieres eliminar esta parada?"
              onConfirm={() => confirm(b.id)}
              onCancel={() => cancel()}
              okText="Si"
              cancelText="No">
              <Button className="delButton">Eliminar</Button>
            </Popconfirm>
            <Button onClick={() => updateBusStop(b)} className="updButton">Actualizar</Button>
          </div>
        )
      })}
      <img src={addbutton} alt="Añadir" onClick={() => navigate(`/Line/${idLine}/addBusStop`)} className="buttonAdd" />
    </div>
  )
}

export default BusStopList