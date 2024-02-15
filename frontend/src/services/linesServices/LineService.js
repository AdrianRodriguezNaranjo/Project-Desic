import http from "../../http-common";

const getAll = async () => {
  try {
    const response = await http.get("/v2/line");
    return response.data;
  } catch (error) {
    console.error("Error get all line", error);
    return null;
  }
};

const get = async (id) => {
  try {
    const response = await http.get("/v2/line/" + id);
    return response.data;
  } catch (error) {
    console.error("Error get a line", error);
    return null;
  }
};

const create = async (line) => {
  try {
    const response = await http.post("/v2/line", line,
      {
        headers: {
          "Content-type": "application/json"
        },
      });
    return response.data;
  } catch (error) {
    console.error("Error post line", error);
    return null;
  }
};

const update = async (id, updatedData) => {
  try {
    const response = await http.put("/v2/line/" + id, updatedData, {
      headers: {
        "Content-Type": "application/json",
      },
    });
    return response.data;
  } catch (error) {
    console.error("Error updating line", error);
    return null;
  }
};

const remove = async (id) => {
  try {
    const response = await http.delete("/v2/line/" + id);
    return response.data;
  } catch (error) {
    console.error("Error delete line", error);
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