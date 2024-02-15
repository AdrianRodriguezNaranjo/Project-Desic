import http from "../../http-common";

const getAll = async () => {
  try {
    const response = await http.get("/v2/bus_user");
    return response.data;
  } catch (error) {
    console.error("Error get all bus_user", error);
    return null;
  }
};

const get = async (id) => {
  try {
    const response = await http.get("/v2/bus_user/" + id);
    return response.data;
  } catch (error) {
    console.error("Error get a bus_user", error);
    return null;
  }
};

const create = async (bus_user) => {
  try {
    const response = await http.post("/v2/bus_user", bus_user,
      {
        headers: {
          "Content-type": "application/json"
        },
      });
    return response.data;
  } catch (error) {
    console.error("Error post bus_user", error);
    return null;
  }
};

const update = async (id, updatedData) => {
  try {
    const response = await http.put("/v2/bus_user/" + id, updatedData, {
      headers: {
        "Content-Type": "application/json",
      },
    });
    return response.data;
  } catch (error) {
    console.error("Error updating bus_user", error);
    return null;
  }
};

const remove = async (id) => {
  try {
    const response = await http.delete("/v2/bus_user/" + id);
    return response.data;
  } catch (error) {
    console.error("Error delete bus_user", error);
    return null;
  }
};

const UserService = {
  getAll,
  get,
  create,
  update,
  remove
};

export default UserService;