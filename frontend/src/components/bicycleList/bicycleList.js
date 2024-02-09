import "./bicycleList.css";
import React from "react";
import { DeleteOutlined, EditOutlined } from '@ant-design/icons';

const BicycleList = ({ items, rows, headline, onDelete, onUpdate }) => {

  return (
    <div className="container-stoplist">
      <h2>Lista de bicicletas</h2>
      <table>
        <thead>
          <tr>
            {headline.map((headline, index) => (
              <th key={index}>{headline}</th>
            ))}
            <th>Acciones</th>
          </tr>
        </thead>
        <tbody>
          {items && items.length > 0 ? (
            items.map((item) => (
              <tr key={item.id}>
                {rows(item)}
                <td>
                  <DeleteOutlined onClick={() => onDelete(item.id)} />
                  <EditOutlined onClick={() => onUpdate(item)} />
                </td>
              </tr>
            ))
          ) : (
            <tr>
              <td colSpan={headline.length + 1}>No hay bicicletas</td>
            </tr>
          )}
        </tbody>
      </table>
    </div>
  );
}

export default BicycleList;