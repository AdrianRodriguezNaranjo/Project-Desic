import BicycleCreate from "../../components/bicycleCreate/bicycleCreate";
import BicycleList from "../../components/bicycleList/bicycleList";
import Consts from "../../components/const/const";
import "./bicycle.css";
import BicycleService from "../../services/bicycle/bicycle.service";
import React, { useEffect, useState } from "react";
import { useNavigate } from "react-router-dom";

function Bicycle() {
  const [current, setCurrent] = useState('0');

  const nav = useNavigate();

  const [mode, setMode] = useState(Consts.addMode);
  const [bicycle, setBicycle] = useState("");
  const [lineList, setlineList] = useState([]);
  const headline = ['id', 'brand', 'model'];

  const getBicycles= async () => {
    try {
      const response = await BicycleService.getBicycles();
      console.log(response);
      setlineList(response);
    } catch (error) {
      console.log(error);
    }
  };

  useEffect(() => {
    getBicycles();
  }, []);

  const row = (line) => {
    return (
      <>
        <td>{line.id}</td>
        <td>{line.brand}</td>
        <td>{line.model}</td>
      </>
    );
  };

  const onDelete = async (id) => {
    await BicycleService.remove(id);
    getBicycles();
  };

  const onUpdate = ((data) => {
    setMode(Consts.editMode);
    const bicycleToEdit = lineList.find(bicy => bicy.id === data.id);
    setBicycle(bicycleToEdit);
  });

  const onCancel = (() => {
    setMode(Consts.addMode);
    setBicycle("");
  });

  return (
    <div className="busline-content">
      <BicycleCreate bicycle={bicycle} mode={mode} onCancel={onCancel} afterAction={getBicycles} />
      <BicycleList items={lineList} rows={row}
        headline={headline} onDelete={onDelete}
        onUpdate={onUpdate} />
    </div>
  );
}

export default Bicycle;