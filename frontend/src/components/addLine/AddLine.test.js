import React from 'react';
import { act, render, fireEvent, waitFor } from '@testing-library/react';
import { MemoryRouter } from 'react-router-dom';
import AddLine from './AddLine';
import LineService from '../../services/linesServices/LineService';
import ReportService from "../../services/report/report.service";

jest.mock('../../services/linesServices/LineService', () => ({
  getAll: jest.fn(),
  create: jest.fn(),
}));


jest.mock('../../services/report/report.service', () => {
  ReportService: "../../assets/report.pdf"
});

describe('AddLine', () => {
  afterEach(() => {
    jest.clearAllMocks();
  });

  it('should render AddLine component correctly', () => {
    const { getByLabelText, getByText } = render(
      <MemoryRouter>
        <AddLine />
      </MemoryRouter>
    );

    expect(getByLabelText('Número')).toBeInTheDocument();
    expect(getByLabelText('Primera parada')).toBeInTheDocument();
    expect(getByLabelText('Última parada')).toBeInTheDocument();
    expect(getByText('Guardar')).toBeInTheDocument();
    expect(getByText('Cancelar')).toBeInTheDocument();
  });

  it('should call LineService.create when clicking the save button with valid input', async () => {
    LineService.create.mockResolvedValueOnce("");
    const { getByPlaceholderText, getByText } = render(
      <MemoryRouter>
        <AddLine />
      </MemoryRouter>
    );

    await act(async () => {
      fireEvent.change(getByPlaceholderText('Número'), { target: { value: '103' } });
      fireEvent.change(getByPlaceholderText('Primera parada'), { target: { value: 'San Telmo' } });
      fireEvent.change(getByPlaceholderText('Última parada'), { target: { value: 'Teror' } });
      fireEvent.click(getByText('Guardar'));
      await waitFor(() => {
        expect(LineService.create).toHaveBeenCalledWith({
          number: '103',
          firstbusstop: 'San Telmo',
          lastbusstop: 'Teror',
        });
      });
    });
  });

  it('should not call LineService.create when clicking the guardar button with invalid input', async () => {
    const { getByText } = render(
      <MemoryRouter>
        <AddLine />
      </MemoryRouter>
    );

    await act(async () => {
      fireEvent.click(getByText('Guardar'));
      await waitFor(() => {
        expect(LineService.create).not.toHaveBeenCalled();
      });
    });
  });
});
