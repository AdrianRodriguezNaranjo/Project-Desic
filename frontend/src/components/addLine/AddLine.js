import "./AddLine.css";
import Header from "../../components/header/Header";
import Footer from "../../components/footer/Footer";
import React, { useState } from "react";
import LineService from "../../services/linesServices/LineService";
import { useNavigate } from "react-router-dom";
import { Button, Input, notification, Space } from "antd";

const AddLine = () => {
  const [api, contextHolder] = notification.useNotification();
  const navigate = useNavigate();
  const [number, setNumber] = useState("");
  const [firstbusstop, setFirstbusstop] = useState("");
  const [lastbusstop, setLastbusstop] = useState("");

  const saveLine = () => {
    var data = {
      number: number,
      firstbusstop: firstbusstop,
      lastbusstop: lastbusstop,
    };

    if (number === '' || firstbusstop === '' || lastbusstop === '') {
      notification.error({ message: 'Ha ocurrido un error', description: 'Comprueba que todos los campos esten rellenados', duration: 5 })
    } else {
      LineService.create(data)
        .then(() => {
          navigate("/Line")
    })
    }
  };

  return (
    <div>
      <Header />
      <div className="bodyAddLine">
      <div className="form-group">
          <label htmlFor="number">Número</label>
          <Input
            type="number"
            id="number"
            value={number}
            onChange={(e) => setNumber(e.target.value)}
          />
        </div>

        <div className="form-group">
          <label htmlFor="firstStop">Primera parada</label>
          <Input
            type="text"
            id="firstStop"
            value={firstbusstop}
            onChange={(e) => setFirstbusstop(e.target.value)}
          />
        </div>

        <div className="form-group">
          <label htmlFor="lastStop">Última parada</label>
          <Input
            type="text"
            id="lastStop"
            value={lastbusstop}
            onChange={(e) => setLastbusstop(e.target.value)}
          />
        </div>

        <Button onClick={saveLine} className="btn btn-success">
          Guardar
        </Button>
        <Button onClick={() => navigate("/Line")} className="btn-denegade">
          Cancelar
        </Button>
      </div>
      <Footer />
    </div>
  )
}

export default AddLine;