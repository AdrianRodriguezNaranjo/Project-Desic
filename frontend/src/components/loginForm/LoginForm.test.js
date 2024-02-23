import React from 'react';
import { act, render, fireEvent, waitFor } from '@testing-library/react';
import LoginForm from './LoginForm';
import { MemoryRouter } from 'react-router-dom';

const mockedNavigate = jest.fn();

jest.mock('react-router-dom', () => ({
  ...(jest.requireActual('react-router-dom')),
  useNavigate: () => mockedNavigate,
}));

describe('ScheduleList', () => {
  afterEach(() => {
    jest.clearAllMocks();
  });

  it('renders schedule list', async () => {
    const { container, getByAltText, getByText } = render(
      <MemoryRouter>
        <LoginForm />
      </MemoryRouter>
    );

    await waitFor(() => {
      expect(container.firstChild).toBeInTheDocument();
      expect(getByText('Email')).toBeInTheDocument();
      expect(getByText('Contraseña')).toBeInTheDocument();
      expect(getByText('¿No tienes una cuenta?')).toBeInTheDocument();
      expect(getByText('Regístrate aquí')).toBeInTheDocument();
    });
  });
});
