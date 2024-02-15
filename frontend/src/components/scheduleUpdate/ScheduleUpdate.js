import ScheduleService from "../../services/scheduleService/ScheduleService";
import { useNavigate, useParams } from "react-router-dom";
import React, { useRef, useState } from "react";
import { Button, message, Popconfirm, notification } from "antd";
import FooterSchedule from "../footerSchedule/FooterSchedule";
import HeaderSchedule from "../headerSchedule/HeaderSchedule";

const UpdateSchedule = () => {
  const s = JSON.parse(localStorage.getItem("schedule"))

  const navigate = useNavigate();
  const { idLine } = useParams();
  const schedulesRef = useRef(null);
  const [submitted, setSubmitted] = useState(false);

  const confirm = (id) => {
    console.log(id);
    message.success('Hora Actualizada');
    updateSchedule(id)
  };
  const cancel = (e) => {
    console.log(e);
    message.error('Cancelado');
  };

  const updateSchedule = () => {
    var data = {
      HourAndMinutes: schedulesRef.current.value,
      idLine: idLine
    };
    const time = schedulesRef.current.value

    if(time === null|| time === ''){
      notification.error({message:'Ha ocurrido un error',description:'Comprueba que todos los campos esten rellenados', duration: 5})
    }else{
    ScheduleService.update(s.id, data)
      .then(response => {
        navigate(`/Line/${idLine}/schedule`)
      })
    }
  };
  return (
    <div>
      <HeaderSchedule/>
      <div className="bodyAddLine">
        <form className="form-group" >
          <label htmlFor="number">Hora y minuto</label>
          <input
            type="time"
            className="form-control"
            id="HourAndMinutes"
            required
            name="number"
            ref={schedulesRef}
          />
          <Popconfirm
            title="Actualizar Linea"
            description="Estas seguro de que quieres actualizar esta hora?"
            onConfirm={() => confirm(s.id)}
            onCancel={() => cancel()}
            okText="Si"
            cancelText="No">
            <Button className="btn btn-success">
              Guardar
            </Button>
          </Popconfirm>
          <Button onClick={() => navigate(`/Line/${idLine}/schedule`)} className="btn-denegade">
            Cancelar
          </Button>
        </form>


      </div>
      <FooterSchedule/>
    </div>
  )

}
export default UpdateSchedule