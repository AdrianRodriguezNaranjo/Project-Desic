import React from 'react';
import { act, render, fireEvent, waitFor } from '@testing-library/react';
import { MemoryRouter } from 'react-router-dom';
import ScheduleAdd from './ScheduleAdd';
import ScheduleService from '../../services/scheduleService/ScheduleService';
import { useParams } from 'react-router-dom';

jest.mock('../../services/scheduleService/ScheduleService', () => ({
  getAll: jest.fn(),
  create: jest.fn(),
}));

jest.mock('react-router-dom', () => ({
  ...jest.requireActual('react-router-dom'),
  useParams: jest.fn()
}));

describe('ScheduleAdd', () => {
  beforeEach(() => {
    useParams.mockReturnValue({ idLine: '123' });
  });

  afterEach(() => {
    jest.clearAllMocks();
  });

  it('should render component correctly', () => {
    const { getByLabelText, getByText } = render(
      <MemoryRouter>
        <ScheduleAdd />
      </MemoryRouter>
    );

    expect(getByText('Hora y minuto')).toBeInTheDocument();
    expect(getByText('Guardar')).toBeInTheDocument();
    expect(getByText('Cancelar')).toBeInTheDocument();
  });

  it('should call create when clicking the save button with valid input', async () => {  
    ScheduleService.create.mockResolvedValueOnce("");
    
    const { getByTestId, getByText } = render(
      <MemoryRouter>
        <ScheduleAdd />
      </MemoryRouter>
    );

    await act(async () => {
      fireEvent.change(getByTestId('HourAndMinutes'), { target: { value: '10:30' } });
      fireEvent.click(getByText('Guardar'));
      await waitFor(() => {
        expect(ScheduleService.create).toHaveBeenCalledWith(
          '123',
          {time: '10:30'}
        );
      });
    });
  });

  it('should not call create when clicking the guardar button with invalid input', async () => {
    const { getByText } = render(
      <MemoryRouter>
        <ScheduleAdd />
      </MemoryRouter>
    );

    await act(async () => {
      fireEvent.click(getByText('Guardar'));
      await waitFor(() => {
        expect(ScheduleService.create).not.toHaveBeenCalled();
      });
    });
  });
});
