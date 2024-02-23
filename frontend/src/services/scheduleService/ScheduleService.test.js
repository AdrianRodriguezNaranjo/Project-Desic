import http from "../../http-common";
import ScheduleService from "./ScheduleService";

jest.mock("../../http-common");

console.error = jest.fn();

describe('ScheduleService', () => {
  afterEach(() => {
    jest.clearAllMocks();
  });

  describe('getAll', () => {
    it('should fetch all schedules for a line', async () => {
      const mockLineId = 1;
      const mockResponseData = [{ id: 1, name: 'Schedule 1' }, { id: 2, name: 'Schedule 2' }];
      http.get.mockResolvedValueOnce({ data: mockResponseData });

      const result = await ScheduleService.getAll(mockLineId);

      expect(http.get).toHaveBeenCalledWith(`/v2/line/${mockLineId}/schedule`);
      expect(result).toEqual(mockResponseData);
    });

    it('should handle errors when fetching all schedules', async () => {
      const mockLineId = 1;
      const errorMessage = 'Failed to fetch all schedules';
      http.get.mockRejectedValueOnce(new Error(errorMessage));

      const result = await ScheduleService.getAll(mockLineId);

      expect(http.get).toHaveBeenCalledWith(`/v2/line/${mockLineId}/schedule`);
      expect(result).toBeNull();
      expect(console.error).toHaveBeenCalledWith('Error get all schedule', expect.any(Error));
    });
  });

  describe('get', () => {
    it('should fetch a single schedule for a line', async () => {
      const mockLineId = 1;
      const mockScheduleId = 1;
      const mockResponseData = { id: mockScheduleId, name: 'Schedule 1' };
      http.get.mockResolvedValueOnce({ data: mockResponseData });

      const result = await ScheduleService.get(mockLineId, mockScheduleId);

      expect(http.get).toHaveBeenCalledWith(`/v2/line/${mockLineId}/schedule/${mockScheduleId}`);
      expect(result).toEqual(mockResponseData);
    });

    it('should handle errors when fetching a single schedule', async () => {
      const mockLineId = 1;
      const mockScheduleId = 1;
      const errorMessage = 'Failed to fetch a schedule';
      http.get.mockRejectedValueOnce(new Error(errorMessage));

      const result = await ScheduleService.get(mockLineId, mockScheduleId);

      expect(http.get).toHaveBeenCalledWith(`/v2/line/${mockLineId}/schedule/${mockScheduleId}`);
      expect(result).toBeNull();
      expect(console.error).toHaveBeenCalledWith('Error get a schedule', expect.any(Error));
    });
  });

  describe('create', () => {
    it('should create a new schedule for a line', async () => {
      const mockLineId = 1;
      const newSchedule = { name: 'New Schedule' };
      const mockResponseData = { id: 1, ...newSchedule };
      http.post.mockResolvedValueOnce({ data: mockResponseData });

      const result = await ScheduleService.create(mockLineId, newSchedule);

      expect(http.post).toHaveBeenCalledWith(`/v2/line/${mockLineId}/schedule`, newSchedule, {
        headers: {
          "Content-type": "application/json"
        },
      });
      expect(result).toEqual(mockResponseData);
    });

    it('should handle errors when creating a schedule', async () => {
      const mockLineId = 1;
      const newSchedule = { name: 'New Schedule' };
      const errorMessage = 'Failed to create a new schedule';
      http.post.mockRejectedValueOnce(new Error(errorMessage));

      const result = await ScheduleService.create(mockLineId, newSchedule);

      expect(http.post).toHaveBeenCalledWith(`/v2/line/${mockLineId}/schedule`, newSchedule, {
        headers: {
          "Content-type": "application/json"
        },
      });
      expect(result).toBeNull();
      expect(console.error).toHaveBeenCalledWith('Error post schedule', expect.any(Error));
    });
  });

  describe('update', () => {
    it('should update an existing schedule for a line', async () => {
      const mockLineId = 1;
      const mockScheduleId = 1;
      const updatedData = { name: 'Updated Schedule' };
      const mockResponseData = { id: mockScheduleId, ...updatedData };
      http.put.mockResolvedValueOnce({ data: mockResponseData });

      const result = await ScheduleService.update(mockLineId, mockScheduleId, updatedData);

      expect(http.put).toHaveBeenCalledWith(`/v2/line/${mockLineId}/schedule/${mockScheduleId}`, updatedData, {
        headers: {
          "Content-Type": "application/json",
        },
      });
      expect(result).toEqual(mockResponseData);
    });

    it('should handle errors when updating a schedule', async () => {
      const mockLineId = 1;
      const mockScheduleId = 1;
      const updatedData = { name: 'Updated Schedule' };
      const errorMessage = 'Failed to update the schedule';
      http.put.mockRejectedValueOnce(new Error(errorMessage));

      const result = await ScheduleService.update(mockLineId, mockScheduleId, updatedData);

      expect(http.put).toHaveBeenCalledWith(`/v2/line/${mockLineId}/schedule/${mockScheduleId}`, updatedData, {
        headers: {
          "Content-Type": "application/json",
        },
      });
      expect(result).toBeNull();
      expect(console.error).toHaveBeenCalledWith('Error updating schedule', expect.any(Error));
    });
  });

  describe('remove', () => {
    it('should remove an existing schedule for a line', async () => {
      const mockLineId = 1;
      const mockScheduleId = 1;
      const mockResponseData = { message: 'Schedule deleted successfully' };
      http.delete.mockResolvedValueOnce({ data: mockResponseData });

      const result = await ScheduleService.remove(mockLineId, mockScheduleId);

      expect(http.delete).toHaveBeenCalledWith(`/v2/line/${mockLineId}/schedule/${mockScheduleId}`);
      expect(result).toEqual(mockResponseData);
    });

    it('should handle errors when removing a schedule', async () => {
      const mockLineId = 1;
      const mockScheduleId = 1;
      const errorMessage = 'Failed to delete the schedule';
      http.delete.mockRejectedValueOnce(new Error(errorMessage));

      const result = await ScheduleService.remove(mockLineId, mockScheduleId);

      expect(http.delete).toHaveBeenCalledWith(`/v2/line/${mockLineId}/schedule/${mockScheduleId}`);
      expect(result).toBeNull();
      expect(console.error).toHaveBeenCalledWith('Error delete schedule', expect.any(Error));
    });
  });
});
