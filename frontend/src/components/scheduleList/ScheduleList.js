import addbutton from "../../assets//icons/addbutton.svg";
import { useNavigate, useParams } from "react-router-dom"
import ScheduleService from "../../services/scheduleService/ScheduleService"
import React, { useState, useEffect } from "react";
import { Button, message, Popconfirm } from 'antd';

const ScheduleList = () => {
  const [schedules, setSchedule] = useState([]);

  const navigate = useNavigate();
  const { idLine } = useParams();


  const confirm = (id) => {
    console.log(id);
    message.success('Se ha eliminado');
    deleteSchedule(id)
  };
  const cancel = (e) => {
    console.log(e);
    message.error('Cancelado');
  };

  useEffect(() => {
    retrieveSchedule();
  }, []);

  const retrieveSchedule = () => {
    ScheduleService.getAll(idLine)
      .then(response => {
        setSchedule(response.data);
      })
      .catch(e => {
        console.log(e);
      });
  };

  const deleteSchedule = (id) => {
    ScheduleService.remove(id).then(response => {
      retrieveSchedule();
    })
    ScheduleService.getAll()
  }

  const updateSchedule = (s) => {
    localStorage.setItem("schedule", JSON.stringify(s))
    navigate(`/Line/${idLine}/updateSchedule`)
  }

  return (
    <div className="container">
      {schedules.map((s, index) => {
        return (
          <div key={index} className="elementBody">
            <h3>Horas de salida</h3>
            <p>{s.HourAndMinutes}</p>

            <Popconfirm
              title="Eliminar Linea"
              description="Estas seguro de que quieres eliminar esta hora?"
              onConfirm={() => confirm(s.id)}
              onCancel={() => cancel()}
              okText="Si"
              cancelText="No">
              <Button className="delButton">Eliminar</Button>
            </Popconfirm>
            <Button onClick={() => updateSchedule(s)} className="updButton">Actualizar</Button>
          </div>
        )
      })}
      <img src={addbutton} alt="Añadir" onClick={() => navigate(`/Line/${idLine}/addSchedule`)} className="buttonAdd" />
    </div>
  )
}

export default ScheduleList