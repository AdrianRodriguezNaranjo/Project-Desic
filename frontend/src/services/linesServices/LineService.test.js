import http from "../../http-common";
import LineService from "./LineService";

jest.mock("../../http-common");

console.error = jest.fn();

describe('LineService', () => {
  afterEach(() => {
    jest.clearAllMocks();
  });

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
    const newLine = { name: 'New Line' };
    const mockResponseData = { id: 1, ...newLine };
    http.post.mockResolvedValueOnce({ data: mockResponseData });

    const result = await LineService.create(newLine);

    expect(http.post).toHaveBeenCalledWith('/v2/line', newLine, {
      headers: {
        "Content-type": "application/json"
      },
    });
    expect(result).toEqual(mockResponseData);
  });

  it('should handle errors when creating a line', async () => {
    const newLine = { name: 'New Line' };
    const errorMessage = 'Failed to create a new line';
    http.post.mockRejectedValueOnce(new Error(errorMessage));

    const result = await LineService.create(newLine);

    expect(http.post).toHaveBeenCalledWith('/v2/line', newLine, {
      headers: {
        "Content-type": "application/json"
      },
    });
    expect(result).toBeNull();
    expect(console.error).toHaveBeenCalledWith('Error post line', expect.any(Error));
  });

  it('should update an existing line', async () => {
    const lineId = 1;
    const updatedData = { name: 'Updated Line' };
    const mockResponseData = { id: lineId, ...updatedData };
    http.put.mockResolvedValueOnce({ data: mockResponseData });

    const result = await LineService.update(lineId, updatedData);

    expect(http.put).toHaveBeenCalledWith(`/v2/line/${lineId}`, updatedData, {
      headers: {
        "Content-Type": "application/json",
      },
    });
    expect(result).toEqual(mockResponseData);
  });

  it('should handle errors when updating a line', async () => {
    const lineId = 1;
    const updatedData = { name: 'Updated Line' };
    const errorMessage = 'Failed to update the line';
    http.put.mockRejectedValueOnce(new Error(errorMessage));

    const result = await LineService.update(lineId, updatedData);

    expect(http.put).toHaveBeenCalledWith(`/v2/line/${lineId}`, updatedData, {
      headers: {
        "Content-Type": "application/json",
      },
    });
    expect(result).toBeNull();
    expect(console.error).toHaveBeenCalledWith('Error updating line', expect.any(Error));
  });

  it('should remove an existing line', async () => {
    const lineId = 1;
    const mockResponseData = { message: 'Line deleted successfully' };
    http.delete.mockResolvedValueOnce({ data: mockResponseData });

    const result = await LineService.remove(lineId);

    expect(http.delete).toHaveBeenCalledWith(`/v2/line/${lineId}`);
    expect(result).toEqual(mockResponseData);
  });

  it('should handle errors when removing a line', async () => {
    const lineId = 1;
    const errorMessage = 'Failed to delete the line';
    http.delete.mockRejectedValueOnce(new Error(errorMessage));

    const result = await LineService.remove(lineId);

    expect(http.delete).toHaveBeenCalledWith(`/v2/line/${lineId}`);
    expect(result).toBeNull();
    expect(console.error).toHaveBeenCalledWith('Error delete line', expect.any(Error));
  });
});