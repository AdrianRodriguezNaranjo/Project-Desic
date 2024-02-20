import "./ListLine.css";
import addbutton from "../../assets//icons/addbutton.svg";
import React, { useState, useEffect } from "react";
import LineService from "../../services/linesServices/LineService";
import ReportService from "../../services/report/report.service";
import { useNavigate } from "react-router-dom";
import { Button, message, Popconfirm } from 'antd';
import { FilePdfOutlined } from '@ant-design/icons';
import update from '../../assets/icons/update.svg';
import deletea from '../../assets/icons/delete.svg';

const LineList = () => {
  const [lines, setLines] = useState([]);
  const [currentLine, setCurrentLine] = useState(null);
  const [currentIndex, setCurrentIndex] = useState(-1);
  const [searchTitle, setSearchTitle] = useState("");
  const navigate = useNavigate();

  const retrieveLines = async () => {
    try {
      const response = await LineService.getAll();
      setLines(response);
      console.log(lines);
    } catch (error) {
      console.log(error);
    }
  };

  useEffect(() => {
    retrieveLines();
  }, []);

  const confirm = (id) => {
    console.log(id);
    message.success('Se ha eliminado');
    deleteLine(id)
  };

  const cancel = (e) => {
    console.log(e);
    message.error('Cancelado');
  };

  const onClick = (e) => {
    ReportService();
  };

  const deleteLine = (id) => {
    LineService.remove(id).then(response => {
      retrieveLines();
    })
    LineService.getAll();
  }

  const updateLine = (l) => {
    localStorage.setItem("linea", JSON.stringify(l));
    navigate("/update");
  }

  return (
    <div className="endofthepage">
    <div className="listBodyLines">
      {lines.map((l, index) => {
        return (
          <div key={index} className="elementBody">
            <div className="textContainer" onClick={()=>navigate(`/Line/${l.id}/schedule`)}>
              <h3>Línea: {l.number}</h3>
              <p>{l.firstbusstop} - {l.lastbusstop}</p>
            </div>
            <div className="buttonContainer">
              <Popconfirm
                title="Eliminar Linea"
                description="Estas seguro de que quieres eliminar esta linea?"
                onConfirm={() => confirm(l.id)}
                onCancel={() => cancel()}
                okText="Si"
                cancelText="No"
              >
              <Button className="delButton"><img src={deletea} alt="eliminar" /><br/>Eliminar</Button>
              </Popconfirm>
              <Button onClick={() => updateLine(l)} className="updButton"><img src={update} alt="Actualizar" /><br/>Actualizar</Button>
            </div>
          </div>
        )
      })}
      <FilePdfOutlined onClick={onClick} />
      <img src={addbutton} alt="Añadir" onClick={() => navigate("/addLine")} className="buttonAdd" />
    </div>
    </div>
  );
};

export default LineList;