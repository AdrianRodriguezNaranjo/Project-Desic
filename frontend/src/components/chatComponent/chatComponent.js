import React, { useEffect, useState } from 'react';
import './chatComponent.css';

const WebSocket = window.WebSocket || window.MozWebSocket; // Asegurarse de tener la clase WebSocket disponible

const ChatComponent = () => {
  const [socket, setSocket] = useState(null);
  const [messages, setMessages] = useState([]);
  const [newMessage, setNewMessage] = useState('');

  useEffect(() => {
    const newSocket = new WebSocket('ws://localhost:1234');

    newSocket.onopen = () => {
      console.log('Conexión establecida');
    };

    newSocket.onmessage = (event) => {
      const receivedMessage = event.data;
      setMessages((prevMessages) => [...prevMessages, receivedMessage]);
    };

    newSocket.onclose = () => {
      console.log('Conexión cerrada');
    };

    setSocket(newSocket);

    return () => {
      newSocket.close();
    };
  }, []);

  const sendMessage = () => {
    if (newMessage.trim() !== '') {
      if (socket && socket.readyState === WebSocket.OPEN) {
        // Agrega el mensaje enviado al estado para que se muestre
        setMessages((prevMessages) => [...prevMessages, `Yo: ${newMessage}`]);
        
        socket.send(newMessage);
        setNewMessage('');
      } else {
        console.error('No se pudo enviar el mensaje. La conexión no está abierta.');
      }
    }
  };

  return (
    <div className='endofthepage'>
    <div className='listBody'>
      <div style={{ marginBottom: '10px' }}>
        <input
          type="text"
          value={newMessage}
          onChange={(e) => setNewMessage(e.target.value)}
          placeholder="Escribe un mensaje..."
        />
        <button onClick={sendMessage}>Enviar</button>
      </div>

      <div>
        <ul style={{ listStyleType: 'none', padding: 0 }}>
          {messages.map((message, index) => (
            <li key={index}>{message}</li>
          ))}
        </ul>
      </div>
    </div>
    </div>
  );
};

export default ChatComponent;
