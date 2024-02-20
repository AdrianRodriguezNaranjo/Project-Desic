import BusStopService from "../../services/busStopService/BusStopService";
import { useNavigate, useParams } from "react-router-dom";
import React, { useRef, useState } from "react";
import { Button, message, Popconfirm, notification, Input } from "antd";
import HeaderBusStop from "../headerBusStop/HeaderBusStop";
import FooterBusStop from "../footerBusStop/FooterBusStop";

const UpdateBusStop = () => {
  const b = JSON.parse(localStorage.getItem("busStop"))

  const navigate = useNavigate();
  const { idLine } = useParams();
  const busStopRef = useRef(b.location);
  const [submitted, setSubmitted] = useState(false);

  const confirm = (id) => {
    console.log(id);
    message.success('Parada Actualizada');
    updateBusStop(id)
  };

  const cancel = (e) => {
    console.log(e);
    message.error('Cancelado');
  };

  const updateBusStop = () => {
    var data = {
      location: busStopRef.current.value
    };
    const locationD = busStopRef.current.value

    if (locationD === null || locationD === '') {
      notification.error({ message: 'Ha ocurrido un error', description: 'Comprueba que todos los campos esten rellenados', duration: 5 })
    } else {
      BusStopService.update(b.id, idLine, data)
        .then(response => {
          navigate(`/Line/${idLine}/BusStop`)
        })
    }
  };
  return (
    <div>
      <HeaderBusStop />
      <div className="bodyAddLine">
        <form className="form-group" >
          <label htmlFor="Location">Localizacion</label>
          <input
            type="text"
            className="form-control"
            id="Location"
            required
            name="Location"
            ref={busStopRef}
          />

          <Popconfirm
            title="Actualizar Linea"
            description="Estas seguro de que quieres actualizar esta parada?"
            onConfirm={() => confirm(b.id)}
            onCancel={() => cancel()}
            okText="Si"
            cancelText="No">
            <Button className="btn btn-success">
              Guardar
            </Button>
          </Popconfirm>
          <Button onClick={() => navigate(`/Line/${idLine}/BusStop`)} className="btn-denegade">
            Cancelar
          </Button>
        </form>


      </div>
      <FooterBusStop />
    </div>
  )

}
export default UpdateBusStop