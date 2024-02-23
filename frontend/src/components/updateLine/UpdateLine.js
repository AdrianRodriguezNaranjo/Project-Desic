import React, { useState } from "react";
import "./UpdateLine.css";
import Header from "../../components/header/Header";
import Footer from "../../components/footer/Footer";
import LineService from "../../services/linesServices/LineService";
import { useNavigate } from "react-router-dom";
import { Button, Input, message, Popconfirm, notification } from 'antd';

const UpdateLine = () => {
  const l = JSON.parse(localStorage.getItem("linea"));
  const navigate = useNavigate();

  const [number, setNumber] = useState(l.number);
  const [firstbusstop, setFirstbusstop] = useState(l.firstbusstop);
  const [lastbusstop, setLastbusstop] = useState(l.lastbusstop);

  const confirm = (id) => {
    console.log(id);    
    UpdateLineSelected(id);
  };

  const cancel = (e) => {
    console.log(e);
    message.error('Cancelado');
  };

  const UpdateLineSelected = (id) => {
    var data = {
      number: number,
      firstbusstop: firstbusstop,
      lastbusstop: lastbusstop
    };

    if(number === '' || firstbusstop === '' || lastbusstop === ''){
      notification.error({message:'Ha ocurrido un error',description:'Comprueba que todos los campos esten rellenados', duration: 5})
    }else{
    LineService.update(id, data);
    message.success('Linea Actualizada');

    navigate("/Line");
  }
}

  return (
    <div>
      <Header />
      <div className="bodyUpdateLine">
        <div className="form-group">
          <label htmlFor="number">Número</label>
          <Input
            type="number"
            className="form-control"
            id="number"
            required
            value={number}
            onChange={(e) => setNumber(e.target.value)}
            name="number"
          />
        </div>

        <div className="form-group">
          <label htmlFor="firstStop">Primera parada</label>
          <Input
            type="text"
            className="form-control"
            id="firstStop"
            required
            value={firstbusstop}
            onChange={(e) => setFirstbusstop(e.target.value)}
            name="firstStop"
          />
        </div>

        <div className="form-group">
          <label htmlFor="lastStop">Ultima parada</label>
          <Input
            type="text"
            className="form-control"
            id="lastStop"
            required
            value={lastbusstop}
            onChange={(e) => setLastbusstop(e.target.value)}
            name="lastStop"
          />
        </div>
        <Popconfirm
              title="Actualizar Linea"
              description="Estas seguro de que quieres actualizar esta linea?"
              onConfirm={() => confirm(l.id)}
              onCancel={() => cancel()}
              okText="Si"
              cancelText="No">
        <Button className="btn btn-success">
          Actualizar
        </Button>
        </Popconfirm>

        <Button onClick={() => navigate("/Line")} className="btn-denegade">
          Cancelar
        </Button>
      </div>
      <Footer />
    </div>
  )
}

export default UpdateLine;