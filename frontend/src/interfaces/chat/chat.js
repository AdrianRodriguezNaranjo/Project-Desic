import "./chat.css";
import React from "react";
import ChatComponent from "../../components/chatComponent/chatComponent";
import Header from "../../components/header/header";
import Footer from "../../components/footer/footer";

function Chat() {
  return (
    <div className="busline-content">
      <Header />
      <ChatComponent />
      <Footer />
    </div>
  );
}

export default Chat;