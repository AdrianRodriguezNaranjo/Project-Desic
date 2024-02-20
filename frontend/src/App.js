import { Route, Routes, BrowserRouter } from "react-router-dom";
import './App.css';
import Chat from "./pages/chat/chat";
import UpdateLine from './components/updateLine/UpdateLine';
import AddLine from './components/addLine/AddLine';
import Lines from "./pages/lines/Lines";
import Schedule from "./pages/schedule/Schedule";
import AddSchedule from "./components/scheduleAdd/ScheduleAdd";
import UpdateSchedule from "./components/scheduleUpdate/ScheduleUpdate";
import AddBusStop from "./components/busStopAdd/BusStopAdd";
import UpdateBusStop from "./components/busStopUpdate/BusStopUpdate";
import BusStop from "./pages/busStop/BusStop";
import LoginForm from "./components/loginForm/LoginForm";
import RegisterForm from "./components/registerForm/RegisterForm";
// import WelcomePage from "./components/welcomePage/WelcomePage";

function App() {
  return (
    <BrowserRouter>
      <Routes>
        <Route path="/" element={<LoginForm />} />
        {/* <Route path="/register" element={<Register />} />
        <Route path="/login" element={<Login />} /> */}
        <Route path="/chat" element={<Chat />} />
        <Route path="/Line" element={<Lines />} />
        <Route path='/Line/:idLine/schedule' element={<Schedule />} />
        <Route path="/Line/:idLine/addSchedule" element={<AddSchedule />} />
        <Route path="/Line/:idLine/updateSchedule" element={<UpdateSchedule />} />
        <Route path="/Line/:idLine/BusStop" element={<BusStop />} />
        <Route path="/Line/:idLine/addBusStop" element={<AddBusStop />} />
        <Route path="/Line/:idLine/updateBusStop" element={<UpdateBusStop />} />
        <Route path="/addLine" element={<AddLine />} />
        <Route path="/update" element={<UpdateLine />} />
        {/* <Route path="/welcome" element={<WelcomePage />} /> */}
        <Route path="/register" element={<RegisterForm/>}/>
        <Route path="/login" element={<LoginForm/>}/>
      </Routes>
    </BrowserRouter>
  );
}

export default App;
