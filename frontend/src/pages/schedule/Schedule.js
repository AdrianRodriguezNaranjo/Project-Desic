import FooterSchedule from "../../components/footerSchedule/FooterSchedule";
import HeaderSchedule from "../../components/headerSchedule/HeaderSchedule";
import ScheduleList from "../../components/scheduleList/ScheduleList";

function Schedule(){

    return(
        <div className='menuBody'>
        <HeaderSchedule/>
        <ScheduleList/>
        <FooterSchedule/>
        </div>
    );
}

export default Schedule