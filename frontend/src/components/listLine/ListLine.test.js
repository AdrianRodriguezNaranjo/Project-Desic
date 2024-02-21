import React from 'react';
import { act, render, fireEvent, waitFor, getByTestId } from '@testing-library/react';
import { MemoryRouter } from 'react-router-dom';
import ListLine from './ListLine';
import LineService from '../../services/linesServices/LineService';
import ReportService from "../../services/report/report.service";

jest.mock('../../services/linesServices/LineService', () => ({
  getAll: jest.fn(),
  remove: jest.fn()
}));

jest.mock('../../services/report/report.service', () => {
  ReportService: "../../assets/report.pdf"
});

const mockedNavigate = jest.fn();

jest.mock('react-router-dom', () => ({
  ...(jest.requireActual('react-router-dom')),
  useNavigate: () => mockedNavigate,
}));

describe('ListLine', () => {
  beforeEach(() => {
    LineService.getAll.mockResolvedValue([
      { id: 1, number: '1', firstbusstop: 'primera', lastbusstop: 'segunda' },
      { id: 2, number: '2', firstbusstop: 'primera', lastbusstop: 'tercera' },
    ]);
  });

  afterEach(() => {
    jest.clearAllMocks();
  });

  it('renders ListLine component with lines', async () => {
    const { container } = render(
      <MemoryRouter>
        <ListLine />
      </MemoryRouter>
    );

    await waitFor(() => {
      expect(container.firstChild).toBeInTheDocument();
    });
  });

  // it('deletes a line when delete button is clicked', async () => {
  //   const { getByText, getAllByText } = render(
  //     <MemoryRouter>
  //       <ListLine />
  //     </MemoryRouter>
  //   );

  //   await act(async () => {
  //   const deleteButtons = getByText('Eliminar');
  //   fireEvent.click(deleteButtons);
  //   await waitFor(() => {
  //     expect(LineService.remove).toHaveBeenCalledWith(1);
  //   });
  // });
  // });

  it('navigates to update page when update button is clicked', async () => {

    const { getByAltText } = render(
      <MemoryRouter>
        <ListLine />
      </MemoryRouter>);

    await act(async () => {
      fireEvent.click(getByAltText('Añadir')); // Cambiar el texto del botón a 'Añadir'

      await waitFor(() => {
        expect(mockedNavigate).toHaveBeenCalledWith('/addLine'); // Verificar la navegación a la página de actualización
      });
    });
  });
});

