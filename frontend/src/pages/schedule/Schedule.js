import FooterBusStop from "../../components/footerBusStop/FooterBusStop";
import HeaderSchedule from "../../components/headerSchedule/HeaderSchedule";
import ScheduleList from "../../components/scheduleList/ScheduleList";

function Schedule(){

    return(
        <div className='menuBody'>
        <HeaderSchedule/>
        <ScheduleList/>
        <FooterBusStop/>
        </div>
    );
}

export default Schedule