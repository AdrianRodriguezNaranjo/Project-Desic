import { Route, Routes, BrowserRouter } from "react-router-dom";
import './App.css';
import Bicycle from "./interfaces/bicycle/bicycle";
import Chat from "./interfaces/chat/chat";


function App() {
  return (
    <BrowserRouter>
      <Routes>
        <Route path="/" element={<Bicycle />} />
        <Route path="/Chat" element={<Chat />} />
      </Routes>
    </BrowserRouter>
  );
}

export default App;
