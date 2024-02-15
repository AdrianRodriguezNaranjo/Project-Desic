import'./Home.css'
import Header from "../../components/header/Header";
import Footer from "../../components/footer/Footer";
import'../../components/registerForm/RegisterForm'
import { useNavigate } from 'react-router-dom';

function Home(){

    const navigate = useNavigate();


    return(
        <>
        <Header/>
            <p>Home</p>
            <button onClick={()=>navigate("/Line")}></button>
        <Footer/>
        </>
    );
}

export default Home