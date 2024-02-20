import './Lines.css'
import Header from "../../components/header/Header";
import Footer from "../../components/footer/Footer";
import LineList from '../../components/listLine/ListLine';

function Lines(){

    return(
        <div className='menuBody'>
        <Header/>
        <LineList/>
        <Footer/>
        </div>
    );
}

export default Lines