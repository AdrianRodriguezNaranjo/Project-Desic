import BusStopService from "../../services/busStopService/BusStopService";
import { useNavigate, useParams } from "react-router-dom";
import React, { useRef, useState } from "react";
import { Button, notification, Input, Space } from "antd";
import HeaderBusStop from "../headerBusStop/HeaderBusStop";
import FooterBusStop from "../footerBusStop/FooterBusStop";

const AddBusStop = () => {

  const [api, contextHolder] = notification.useNotification();

  const navigate = useNavigate();
  const { idLine } = useParams();
  const busStopRef = useRef(null);
  const [submitted, setSubmitted] = useState(false);

  const saveBusStop = () => {
    var data = {
      location: busStopRef.current.value === '' ? null : busStopRef.current.value
    };

    const locationD = busStopRef.current.value;

    if (locationD === null || locationD === '') {
      notification.error({ message: 'Ha ocurrido un error', description: 'Comprueba que todos los campos esten rellenados', duration: 5 })
    } else {
      console.log(data);
      BusStopService.create(idLine, data)
        .then(response => {
          navigate(`/Line/${idLine}/BusStop`);
        })
    };

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
            defaultValue={null}
          />
          <Button onClick={() => saveBusStop()} className="btn btn-success">
            Guardar
          </Button>
          <Button onClick={() => navigate(`/Line/${idLine}/BusStop`)} className="btn-denegade">
            Cancelar
          </Button>
        </form>

      </div>
      <FooterBusStop />
    </div>
  )

}
export default AddBusStop;