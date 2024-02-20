import BusStopList from "../../components/busStopList/BusStopList";
import FooterBusStop from "../../components/footerBusStop/FooterBusStop";
import HeaderBusStop from "../../components/headerBusStop/HeaderBusStop";


function BusStop(){

    return(
        <div className='menuBody'>
        <HeaderBusStop/>
        <BusStopList/>
        <FooterBusStop/>
        </div>
    );
}

export default BusStop