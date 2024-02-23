import './Header.css'
import help from '../../assets/icons/help.svg';
import ReportService from "../../services/report/report.service";
import { FilePdfOutlined } from '@ant-design/icons';

function Header() {
  const onClick = (e) => {
    ReportService();
  };

  return (
    <div className="bodyHeader">
      <div className='contentHeader'>
      <div className="pageTitle">
        <h1>Líneas</h1>
      </div>
      <div className='report'>
      <FilePdfOutlined onClick={onClick} />
      </div>
      <div className='helpHeader'>
      <a href="/documentos_de_ayuda/Project-Desic.html" target="Blank">
        <img src={help} alt="Ayuda" />
      </a>
      </div>
    </div>
    </div>
  );
}

export default Header;