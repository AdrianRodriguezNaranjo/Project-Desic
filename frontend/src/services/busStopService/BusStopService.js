import http from "../../http-common";

const getAll = async (idLine) => {
  try {
    const response = await http.get("/v2/line/" + idLine + "/bus_stop");
    return response.data;
  } catch (error) {
    console.error("Error get all bus_stop", error);
    return null;
  }
};

const get = async (idLine, id) => {
  try {
    const response = await http.get("/v2/line/" + idLine + "/bus_stop/" + id);
    return response.data;
  } catch (error) {
    console.error("Error get a bus_stop", error);
    return null;
  }
};

const create = async (idLine, bus_stop) => {
  try {
    const response = await http.post("/v2/line/" + idLine + "/bus_stop", bus_stop,
      {
        headers: {
          "Content-type": "application/json"
        },
      });
    return response.data;
  } catch (error) {
    console.error("Error post bus_stop", error);
    return null;
  }
};

const update = async (id, idLine, updatedData) => {
  try {
    const response = await http.put("/v2/line/" + idLine + "/bus_stop/" + id, updatedData, {
      headers: {
        "Content-Type": "application/json",
      },
    });
    return response.data;
  } catch (error) {
    console.error("Error updating bus_stop", error);
    return null;
  }
};

const remove = async (id, idLine) => {
  try {
    const response = await http.delete("/v2/line/" + idLine + "/bus_stop/" + id);
    return response.data;
  } catch (error) {
    console.error("Error delete bus_stop", error);
    return null;
  }
};

const BusStopService = {
  getAll,
  get,
  create,
  update,
  remove
};

export default BusStopService;