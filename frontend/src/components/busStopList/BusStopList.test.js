import React from 'react';
import { act, render, fireEvent, waitFor } from '@testing-library/react';
import BusStopList from './BusStopList';
import BusStopService from "../../services/busStopService/BusStopService";
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

jest.mock("../../services/busStopService/BusStopService", () => ({
  getAll: jest.fn()
}));

describe('ScheduleList', () => {
  beforeEach(() => {
    useParams.mockReturnValue({ idLine: '123' });
    BusStopService.getAll.mockResolvedValue([
      { id: 1, idLine: '123', location: 'aaa' },
      { id: 2, idLine: '123', location: 'bbb' }
    ]);
  });

  afterEach(() => {
    jest.clearAllMocks();
  });

  it('renders schedule list', async () => {
    const { container, getByAltText, getByText } = render(
      <MemoryRouter>
        <BusStopList />
      </MemoryRouter>
    );

    await waitFor(() => {
      expect(container.firstChild).toBeInTheDocument();
      expect(getByAltText('Añadir')).toBeInTheDocument();
    });
  });
});
