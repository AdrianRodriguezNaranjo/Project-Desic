import React from 'react';
import { render, fireEvent, waitFor } from '@testing-library/react';
import { MemoryRouter } from 'react-router-dom';
import AddLine from './AddLine';
import LineService from '../../services/linesServices/LineService';

jest.mock('../../services/linesServices/LineService');

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

  // it('should call LineService.create when clicking the guardar button with valid input', async () => {
  //   const { getByLabelText, getByText } = render(
  //     <MemoryRouter>
  //       <AddLine />
  //     </MemoryRouter>
  //   );
  //   const numberInput = getByLabelText('Número');
  //   const firstStopInput = getByLabelText('Primera parada');
  //   const lastStopInput = getByLabelText('Última parada');
  //   const saveButton = getByText('Guardar');

  //   fireEvent.change(numberInput, { target: { value: '103' } });
  //   fireEvent.change(firstStopInput, { target: { value: 'San Telmo' } });
  //   fireEvent.change(lastStopInput, { target: { value: 'Teror' } });
  //   fireEvent.click(saveButton);

  //   await waitFor(() => {
  //     expect(LineService.create).toHaveBeenCalledWith({
  //       number: '103',
  //       firstbusstop: 'San Telmo',
  //       lastbusstop: 'Teror',
  //     });
  //   });
  // });

  it('should not call LineService.create when clicking the guardar button with invalid input', async () => {
    const { getByText } = render(
      <MemoryRouter>
        <AddLine />
      </MemoryRouter>
    );
    const saveButton = getByText('Guardar');

    fireEvent.click(saveButton);

    await waitFor(() => {
      expect(LineService.create).not.toHaveBeenCalled();
    });
  });
});
