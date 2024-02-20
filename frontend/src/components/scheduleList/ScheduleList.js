import "./ScheduleList.css";
import addbutton from "../../assets//icons/addbutton.svg";
import { useNavigate, useParams } from "react-router-dom"
import ScheduleService from "../../services/scheduleService/ScheduleService"
import React, { useState, useEffect } from "react";
import { Button, message, Popconfirm } from 'antd';
import update from '../../assets/icons/update.svg';
import deletea from '../../assets/icons/delete.svg';

const ScheduleList = () => {
  const [schedules, setSchedule] = useState([]);

  const navigate = useNavigate();
  const { idLine } = useParams();

  const retrieveSchedule = async () => {
    try {
      const response = await ScheduleService.getAll(idLine);
      setSchedule(response);
      console.log(schedules);
    } catch (error) {
      console.log(error);
    }
  };

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

  const deleteSchedule = (id) => {
    ScheduleService.remove(id,idLine).then(response => {
      retrieveSchedule();
    })
    ScheduleService.getAll(idLine);
  }

  const updateSchedule = (s) => {
    localStorage.setItem("schedule", JSON.stringify(s));
    navigate(`/Line/${idLine}/updateSchedule`);
  }

  return (
    <div className="endofthepage">
    <div className="listBodySchedule">
      {schedules.map((s, index) => {
        return (
          <div key={index} className="elementBody">
            <div className="textContainer">
            <h3>Horas de salida</h3>
            <p>{s.time}</p>
            </div>
            <div className="buttonContainer">
            <Popconfirm
              title="Eliminar Linea"
              description="Estas seguro de que quieres eliminar esta hora?"
              onConfirm={() => confirm(s.id)}
              onCancel={() => cancel()}
              okText="Si"
              cancelText="No">
              <Button className="delButton"><img src={deletea} alt="eliminar" /><br/>Eliminarr</Button>
            </Popconfirm>
            <Button onClick={() => updateSchedule(s)} className="updButton"><img src={update} alt="Actualizar" /><br/>Actualizar</Button>
          </div>
          </div>
        )
      })}
      <img src={addbutton} alt="Añadir" onClick={() => navigate(`/Line/${idLine}/addSchedule`)} className="buttonAdd" />
    </div>
    </div>
  )
}

export default ScheduleList