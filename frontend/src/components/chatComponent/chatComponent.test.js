import React from 'react';
import { act, render, fireEvent, waitFor } from '@testing-library/react';
import { MemoryRouter } from 'react-router-dom';
import ChatComponent from './chatComponent';
import { WebSocketMock } from 'jest-websocket-mock';

describe('ChatComponent', () => {
  jest.spyOn(global, 'WebSocket');

  beforeEach(() => {
    global.WebSocket.mockClear();
  });

  it('should render component correctly', () => {
    const { getByPlaceholderText, getByText } = render(
      <MemoryRouter>
        <ChatComponent />
      </MemoryRouter>
    );

    expect(getByPlaceholderText('Escribe un mensaje...')).toBeInTheDocument();
    expect(getByText('Enviar')).toBeInTheDocument();
  });

  it('should render received messages', async () => {
    const { getByPlaceholderText, getByText, getByTestId } = render(
      <MemoryRouter>
        <ChatComponent />
      </MemoryRouter>
    );

    const input = getByPlaceholderText('Escribe un mensaje...');
    const sendButton = getByText('Enviar');

    await act(async () => {
      fireEvent.change(input, { target: { value: 'Hola mundo' } });
      fireEvent.click(sendButton);
      await waitFor(() => {
        expect(input.value).toBe('Hola mundo');
      });
    });
  });


  // it('should not call create when clicking the guardar button with invalid input', async () => {
  //   const { getByText } = render(
  //     <MemoryRouter>
  //       <ChatComponent />
  //     </MemoryRouter>
  //   );

  //   await act(async () => {
  //     fireEvent.click(getByText('Guardar'));
  //     await waitFor(() => {
  //       expect(BusStopService.create).not.toHaveBeenCalled();
  //     });
  //   });
  // });
});

