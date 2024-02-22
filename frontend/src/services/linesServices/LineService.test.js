import http from "../../http-common";
import LineService from "./LineService";

// Mockear el módulo http-common
jest.mock("../../http-common");

console.error = jest.fn();

describe('LineService', () => {
  afterEach(() => {
    jest.clearAllMocks();
  });

  // Prueba para getAll
  describe('getAll', () => {
    it('should fetch all lines', async () => {
      const mockResponseData = [{ id: 1, name: 'Line 1' }, { id: 2, name: 'Line 2' }];
      http.get.mockResolvedValueOnce({ data: mockResponseData });

      const result = await LineService.getAll();

      expect(http.get).toHaveBeenCalledWith('/v2/line');
      expect(result).toEqual(mockResponseData);
    });

    it('should handle errors when fetching all lines', async () => {
      const errorMessage = 'Failed to fetch all lines';
      http.get.mockRejectedValueOnce(new Error(errorMessage));

      const result = await LineService.getAll();

      expect(http.get).toHaveBeenCalledWith('/v2/line');
      expect(result).toBeNull();
      expect(console.error).toHaveBeenCalledWith('Error get all line', expect.any(Error));
    });
  });

  // Prueba para get
  describe('get', () => {
    it('should fetch a single line', async () => {
      const mockLineId = 1;
      const mockResponseData = { id: mockLineId, name: 'Line 1' };
      http.get.mockResolvedValueOnce({ data: mockResponseData });

      const result = await LineService.get(mockLineId);

      expect(http.get).toHaveBeenCalledWith(`/v2/line/${mockLineId}`);
      expect(result).toEqual(mockResponseData);
    });

    it('should handle errors when fetching a single line', async () => {
      const mockLineId = 1;
      const errorMessage = 'Failed to fetch a line';
      http.get.mockRejectedValueOnce(new Error(errorMessage));

      const result = await LineService.get(mockLineId);

      expect(http.get).toHaveBeenCalledWith(`/v2/line/${mockLineId}`);
      expect(result).toBeNull();
      expect(console.error).toHaveBeenCalledWith('Error get a line', expect.any(Error));
    });
  });

  it('should create a new line', async () => {
    // Mock de los datos de la nueva línea
    const newLine = { name: 'New Line' };
    // Mock de la respuesta exitosa del servidor
    const mockResponseData = { id: 1, ...newLine };
    http.post.mockResolvedValueOnce({ data: mockResponseData });

    // Llamar al método create
    const result = await LineService.create(newLine);

    // Verificar que se realizó la llamada POST con los datos correctos
    expect(http.post).toHaveBeenCalledWith('/v2/line', newLine, {
      headers: {
        "Content-type": "application/json"
      },
    });
    // Verificar que el resultado es el esperado
    expect(result).toEqual(mockResponseData);
  });

  it('should handle errors when creating a line', async () => {
    // Mock de los datos de la nueva línea
    const newLine = { name: 'New Line' };
    // Mock del mensaje de error
    const errorMessage = 'Failed to create a new line';
    // Mock de la respuesta de error del servidor
    http.post.mockRejectedValueOnce(new Error(errorMessage));

    // Llamar al método create
    const result = await LineService.create(newLine);

    // Verificar que se realizó la llamada POST con los datos correctos
    expect(http.post).toHaveBeenCalledWith('/v2/line', newLine, {
      headers: {
        "Content-type": "application/json"
      },
    });
    // Verificar que el resultado es null
    expect(result).toBeNull();
    // Verificar que se registró el error en la consola
    expect(console.error).toHaveBeenCalledWith('Error post line', expect.any(Error));
  });

  it('should update an existing line', async () => {
    // Mock del ID de la línea existente y los datos actualizados
    const lineId = 1;
    const updatedData = { name: 'Updated Line' };
    // Mock de la respuesta exitosa del servidor
    const mockResponseData = { id: lineId, ...updatedData };
    http.put.mockResolvedValueOnce({ data: mockResponseData });

    // Llamar al método update
    const result = await LineService.update(lineId, updatedData);

    // Verificar que se realizó la llamada PUT con los datos correctos
    expect(http.put).toHaveBeenCalledWith(`/v2/line/${lineId}`, updatedData, {
      headers: {
        "Content-Type": "application/json",
      },
    });
    // Verificar que el resultado es el esperado
    expect(result).toEqual(mockResponseData);
  });

  it('should handle errors when updating a line', async () => {
    // Mock del ID de la línea existente y los datos actualizados
    const lineId = 1;
    const updatedData = { name: 'Updated Line' };
    // Mock del mensaje de error
    const errorMessage = 'Failed to update the line';
    // Mock de la respuesta de error del servidor
    http.put.mockRejectedValueOnce(new Error(errorMessage));

    // Llamar al método update
    const result = await LineService.update(lineId, updatedData);

    // Verificar que se realizó la llamada PUT con los datos correctos
    expect(http.put).toHaveBeenCalledWith(`/v2/line/${lineId}`, updatedData, {
      headers: {
        "Content-Type": "application/json",
      },
    });
    // Verificar que el resultado es null
    expect(result).toBeNull();
    // Verificar que se registró el error en la consola
    expect(console.error).toHaveBeenCalledWith('Error updating line', expect.any(Error));
  });

  it('should remove an existing line', async () => {
    // Mock del ID de la línea existente
    const lineId = 1;
    // Mock de la respuesta exitosa del servidor
    const mockResponseData = { message: 'Line deleted successfully' };
    http.delete.mockResolvedValueOnce({ data: mockResponseData });

    // Llamar al método remove
    const result = await LineService.remove(lineId);

    // Verificar que se realizó la llamada DELETE con el ID correcto
    expect(http.delete).toHaveBeenCalledWith(`/v2/line/${lineId}`);
    // Verificar que el resultado es el esperado
    expect(result).toEqual(mockResponseData);
  });

  it('should handle errors when removing a line', async () => {
    // Mock del ID de la línea existente
    const lineId = 1;
    // Mock del mensaje de error
    const errorMessage = 'Failed to delete the line';
    // Mock de la respuesta de error del servidor
    http.delete.mockRejectedValueOnce(new Error(errorMessage));

    // Llamar al método remove
    const result = await LineService.remove(lineId);

    // Verificar que se realizó la llamada DELETE con el ID correcto
    expect(http.delete).toHaveBeenCalledWith(`/v2/line/${lineId}`);
    // Verificar que el resultado es null
    expect(result).toBeNull();
    // Verificar que se registró el error en la consola
    expect(console.error).toHaveBeenCalledWith('Error delete line', expect.any(Error));
  });
});