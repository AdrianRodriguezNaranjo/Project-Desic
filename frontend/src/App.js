import { Route, Routes, BrowserRouter } from "react-router-dom";
import './App.css';
import Bicycle from "./interfaces/bicycle/bicycle";


function App() {
  return (
    <BrowserRouter>
      <Routes>
        <Route path="/" element={<Bicycle />} />
      </Routes>
    </BrowserRouter>
  );
}

export default App;
