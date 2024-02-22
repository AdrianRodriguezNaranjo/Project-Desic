import React from 'react';
import { act, render, fireEvent, waitFor } from '@testing-library/react';
import { MemoryRouter } from 'react-router-dom';
import BusStopAdd from './BusStopAdd';
import BusStopService from '../../services/busStopService/BusStopService';
import { useParams } from 'react-router-dom';

jest.mock('../../services/busStopService/BusStopService', () => ({
  getAll: jest.fn(),
  create: jest.fn(),
}));

jest.mock('react-router-dom', () => ({
  ...jest.requireActual('react-router-dom'),
  useParams: jest.fn()
}));

describe('BusStopAdd', () => {
  beforeEach(() => {
    useParams.mockReturnValue({ idLine: '123' });
  });

  afterEach(() => {
    jest.clearAllMocks();
  });

  it('should render component correctly', () => {
    const { getByLabelText, getByText } = render(
      <MemoryRouter>
        <BusStopAdd />
      </MemoryRouter>
    );

    expect(getByLabelText('Localización')).toBeInTheDocument();
    expect(getByText('Guardar')).toBeInTheDocument();
    expect(getByText('Cancelar')).toBeInTheDocument();
  });

  it('should call create when clicking the save button with valid input', async () => {  
    BusStopService.create.mockResolvedValueOnce("");
    
    const { getByPlaceholderText, getByText } = render(
      <MemoryRouter>
        <BusStopAdd />
      </MemoryRouter>
    );

    await act(async () => {
      fireEvent.change(getByPlaceholderText('Localización'), { target: { value: 'AAA' } });
      fireEvent.click(getByText('Guardar'));
      await waitFor(() => {
        expect(BusStopService.create).toHaveBeenCalledWith(
          '123',
          {location: 'AAA'}
        );
      });
    });
  });

  it('should not call create when clicking the guardar button with invalid input', async () => {
    const { getByText } = render(
      <MemoryRouter>
        <BusStopAdd />
      </MemoryRouter>
    );

    await act(async () => {
      fireEvent.click(getByText('Guardar'));
      await waitFor(() => {
        expect(BusStopService.create).not.toHaveBeenCalled();
      });
    });
  });
});
