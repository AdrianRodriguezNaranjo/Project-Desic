import "./bicycleCreate.css";
import BicycleService from "../../services/bicycle/bicycle.service";
import { useState, useEffect } from 'react';
import { Button, Input } from 'antd';

function BicycleCreate({ bicycle, mode, onCancel, afterAction }) {
  const [id, setId] = useState(bicycle.id || "");
  const [brand, setBrand] = useState(bicycle.brand || "");
  const [model, setModel] = useState(bicycle.model || "");

  useEffect(() => {
    setId(bicycle.id || "");
    setBrand(bicycle.brand || "");
    setModel(bicycle.model || "");
  }, [bicycle]);

  // const [voidLoginError, setVoidError] = useState("");

  // const validateInput = () => {

  // }
  const submitBicycle = async () => {
    const requestData = {
      id: id,
      brand: brand,
      model: model
    };
    if (mode === "Editar") {
      await BicycleService.update(id, requestData);
    } else if (mode === "Añadir") {     
      await BicycleService.create(requestData);
      setId("");
      setBrand("");
      setModel("");
    }
    afterAction();
  }

  return (
    <div className="container-stopcreate">
      <h2>{mode}</h2>
      <Input placeholder="Id" value={id} onChange={(e) => setId(e.target.value)} />
      <Input placeholder="Brand" value={brand} onChange={(e) => setBrand(e.target.value)} />
      <Input placeholder="Model" value={model} onChange={(e) => setModel(e.target.value)} />
      {/* {voidLoginError && <div className="error-mesage">{voidLoginError}</div>} */}
      <Button className="buttonCreateStop" type="primary" onClick={submitBicycle}>{mode}</Button>
      {mode === "Editar" && (<Button className="buttonCancel" danger type="primary" onClick={onCancel}>Cancelar</Button>)}
    </div>
  );
}

export default BicycleCreate;