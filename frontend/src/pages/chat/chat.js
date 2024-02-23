import "./chat.css";
import React from "react";
import ChatComponent from "../../components/chatComponent/chatComponent";
import FooterBusStop from "../../components/footerBusStop/FooterBusStop";
import HeaderChat from "../../components/headerChat/HeaderChat";

function Chat() {
  return (
    <div className="menuBody">
      <HeaderChat/>
      <ChatComponent />
      <FooterBusStop/>
    </div>
  );
}

export default Chat;