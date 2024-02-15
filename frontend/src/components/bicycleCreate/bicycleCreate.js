import "./bicycleCreate.css";
import LineService from "../../services/linesServices/LineService";
import { useState, useEffect } from 'react';
import { Button, Input } from 'antd';

function BicycleCreate({ bicycle, mode, onCancel, afterAction }) {
  const [number, setId] = useState(bicycle.number || "");
  const [firstbusstop, setBrand] = useState(bicycle.firstbusstop || "");
  const [lastbusstop, setModel] = useState(bicycle.lastbusstop || "");

  useEffect(() => {
    setId(bicycle.number || "");
    setBrand(bicycle.firstbusstop || "");
    setModel(bicycle.lastbusstop || "");
  }, [bicycle]);

  // const [voidLoginError, setVoidError] = useState("");

  // const validateInput = () => {

  // }
  const submitBicycle = async () => {
    const requestData = {
      number: number,
      firstbusstop: firstbusstop,
      lastbusstop: lastbusstop
    };
    if (mode === "Editar") {
      await LineService.update("7", requestData);
    } else if (mode === "Añadir") {     
      await LineService.create(requestData);
      setId("");
      setBrand("");
      setModel("");
    }
    console.log(requestData);
    afterAction();
  }

  return (
    <div className="container-stopcreate">
      <h2>{mode}</h2>
      <Input placeholder="number" value={number} onChange={(e) => setId(e.target.value)} />
      <Input placeholder="firstbusstop" value={firstbusstop} onChange={(e) => setBrand(e.target.value)} />
      <Input placeholder="lastbusstop" value={lastbusstop} onChange={(e) => setModel(e.target.value)} />
      {/* {voidLoginError && <div className="error-mesage">{voidLoginError}</div>} */}
      <Button className="buttonCreateStop" type="primary" onClick={submitBicycle}>{mode}</Button>
      {mode === "Editar" && (<Button className="buttonCancel" danger type="primary" onClick={onCancel}>Cancelar</Button>)}
    </div>
  );
}

export default BicycleCreate;