import http from "../../http-common";

const getAll = async (idLine) => {
  try {
    const response = await http.get("/v2/line/" + idLine + "/schedule");
    return response.data;
  } catch (error) {
    console.error("Error get all schedule", error);
    return null;
  }
};

const get = async (idLine, id) => {
  try {
    const response = await http.get("/v2/line/" + idLine + "/schedule/" + id);
    return response.data;
  } catch (error) {
    console.error("Error get a schedule", error);
    return null;
  }
};

const create = async (idLine, schedule) => {
  try {
    const response = await http.post("/v2/line/" + idLine + "/schedule", schedule,
      {
        headers: {
          "Content-type": "application/json"
        },
      });
    return response.data;
  } catch (error) {
    console.error("Error post schedule", error);
    return null;
  }
};

const update = async (id, idLine, updatedData) => {
  try {
    const response = await http.put("/v2/line/" + idLine + "/schedule/" + id, updatedData, {
      headers: {
        "Content-Type": "application/json",
      },
    });
    return response.data;
  } catch (error) {
    console.error("Error updating schedule", error);
    return null;
  }
};

const remove = async (id, idLine) => {
  try {
    const response = await http.delete("/v2/line/" + idLine + "/schedule/" + id);
    return response.data;
  } catch (error) {
    console.error("Error delete schedule", error);
    return null;
  }
};

export default {
  getAll,
  get,
  create,
  update,
  remove
};