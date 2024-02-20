import "./ListLine.css";
import React, { useState, useEffect } from "react";
import ReportService from "../../services/report/report.service";

import { FilePdfOutlined } from '@ant-design/icons';

const LineList = () => {
  const onClick = (e) => {
    ReportService();
  };

  return (
    <div className="listBody">   
      <FilePdfOutlined onClick={onClick} />
    </div>
  );
};

export default LineList;