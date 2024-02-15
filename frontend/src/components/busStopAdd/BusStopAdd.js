import BusStopService from "../../services/busStopService/BusStopService";
import { useNavigate, useParams } from "react-router-dom";
import React, { useRef, useState } from "react";
import { Button, notification, Space } from "antd";
import HeaderBusStop from "../headerBusStop/HeaderBusStop";
import FooterBusStop from "../footerBusStop/FooterBusStop";

const AddBusStop = () => {

  const [api, contextHolder] = notification.useNotification();

  const navigate = useNavigate();
  const { idLine } = useParams();
  const busStopRef = useRef(null);
  const fileInputRef = useRef(null);
  const [submitted, setSubmitted] = useState(false);

  

  const saveBusStop = () => {
    var data = {
      Location: busStopRef.current.value === '' ? null : busStopRef.current.value,
      idLine: idLine,
      file:  fileInputRef.current.value === '' ? null : fileInputRef.current.files[0],
    };

   const locationD = busStopRef.current.value
   const fileD = fileInputRef.current.value

    if(locationD === null|| locationD === '' || fileD === null || fileD === ''){
      notification.error({message:'Ha ocurrido un error',description:'Comprueba que todos los campos esten rellenados', duration: 5})
    }else{
      BusStopService.create(data)
      .then(response => {
        navigate(`/Line/${idLine}/listBusStop`)
      })
    };

  };
  return (
    <div>
      <HeaderBusStop/>
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

          <label htmlFor="filename">Imagen</label>
          <input
            type="file"
            className="form-control"
            id="filename"
            required
            name="filename"
            ref={fileInputRef}
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
      <FooterBusStop/>
    </div>
  )

}
export default AddBusStop