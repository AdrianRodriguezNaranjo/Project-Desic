import React from 'react';
import { act, render, fireEvent, waitFor } from '@testing-library/react';
import RegisterForm from './RegisterForm';
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
    const { container, getByText } = render(
      <MemoryRouter>
        <RegisterForm />
      </MemoryRouter>
    );

    await waitFor(() => {
      expect(container.firstChild).toBeInTheDocument();
      expect(getByText('Nombre')).toBeInTheDocument();
      expect(getByText('Email')).toBeInTheDocument();
      expect(getByText('Contraseña')).toBeInTheDocument();    
      expect(getByText('Inicia sesión aquí')).toBeInTheDocument();
    });
  });
});