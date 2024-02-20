import React from 'react';
import { act, render, fireEvent, waitFor } from '@testing-library/react';
import { MemoryRouter } from 'react-router-dom';
import ListLine from './ListLine';
import LineService from '../../services/linesServices/LineService';
import ReportService from "../../services/report/report.service";

jest.mock('../../services/linesServices/LineService', () => ({
  getAll: jest.fn()
}));

jest.mock('../../services/report/report.service', () => {
  ReportService: "../../assets/report.pdf"
});

describe('ListLine', () => {
  // beforeEach(() => {
 
  // });
  LineService.getAll.mockResolvedValue([
    { id: 1, number: '1', firstbusstop: 'primera', lastbusstop: 'segunda' },
    { id: 2, number: '2', firstbusstop: 'primera', lastbusstop: 'tercera' },
  ]);
  afterEach(() => {
    jest.clearAllMocks();
  });

  it('renders ListLine component with lines', async () => {

    const { getByText } = render(
      <MemoryRouter>
        <ListLine />
      </MemoryRouter>
    );
    await waitFor(() => {
      expect(getByText('Línea: 1')).toBeInTheDocument();
      expect(getByText('Línea: 2')).toBeInTheDocument();
    });
  });

  it('renders delete and update buttons for each line', async () => {
    const { getByText, getAllByText } = render(
      <MemoryRouter>
        <ListLine />
      </MemoryRouter>
    );
    await waitFor(() => {
      const deleteButtons = getAllByText('Eliminar');
      const updateButtons = getAllByText('Actualizar');
      expect(deleteButtons.length).toBe(2);
      expect(updateButtons.length).toBe(2);
    });
  });

  // it('deletes a line when delete button is clicked', async () => {
  //   const { getByText, getAllByText } = render(
  //     <MemoryRouter>
  //       <ListLine />
  //     </MemoryRouter>
  //   );
  //   const deleteButtons = getAllByText('Eliminar');
  //   fireEvent.click(deleteButtons[0]);
  //   await waitFor(() => {
  //     expect(LineService.remove(1)).toHaveBeenCalledWith(1);
  //   });
  //   expect(getByText('Se ha eliminado')).toBeInTheDocument();
  // });

  // it('navigates to update page when update button is clicked', async () => {
  //   const mockNavigate = jest.fn();
  //   jest.mock('react-router-dom', () => ({
  //     ...jest.requireActual('react-router-dom'),
  //     useNavigate: () => mockNavigate,
  //   }));

  //   const { getAllByText } = render(
  //     <MemoryRouter>
  //       <ListLine />
  //     </MemoryRouter>);
  //   await waitFor(() => {
  //     const updateButtons = getAllByText('Actualizar');
  //     fireEvent.click(updateButtons[0]);
  //     expect(mockNavigate).toHaveBeenCalledWith('/update');
  //   });
  // });
});