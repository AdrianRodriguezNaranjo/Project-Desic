import React from 'react';
import { act, render, fireEvent, waitFor } from '@testing-library/react';
import ScheduleList from './ScheduleList';
import ScheduleService from "../../services/scheduleService/ScheduleService";
import { useParams } from 'react-router-dom';
import { MemoryRouter } from 'react-router-dom';

const mockedNavigate = jest.fn();

jest.mock('react-router-dom', () => ({
  ...(jest.requireActual('react-router-dom')),
  useNavigate: () => mockedNavigate,
}));

jest.mock('react-router-dom', () => ({
  ...jest.requireActual('react-router-dom'),
  useParams: jest.fn()
}));

jest.mock("../../services/scheduleService/ScheduleService", () => ({
  getAll: jest.fn()
}));

describe('ScheduleList', () => {
  beforeEach(() => {
    useParams.mockReturnValue({ idLine: '123' });
    ScheduleService.getAll.mockResolvedValue([
      { id: 1, idLine: '123', time: '10:00' },
      { id: 2, idLine: '123', time: '12:00' }
    ]);
  });

  afterEach(() => {
    jest.clearAllMocks();
  });

  it('renders schedule list', async () => {
    const { container, getByAltText, getByText } = render(
      <MemoryRouter>
        <ScheduleList />
      </MemoryRouter>
    );

    await waitFor(() => {
      expect(container.firstChild).toBeInTheDocument();
      expect(getByAltText('Añadir')).toBeInTheDocument();
    });
  });
});
