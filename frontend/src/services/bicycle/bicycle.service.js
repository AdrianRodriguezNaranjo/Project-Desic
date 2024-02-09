import http from "../../http-common";

const getBicycles = async () => {
  try {
    const response = await http.get("/v2/bicycle");
    return response.data;
  } catch (error) {
    console.error("Error get bicycle", error);
    return null;
  }
};

const getBicycle = async (id) => {
  try {
    const response = await http.get("/v2/bicycle/" + id);
    return response.data;
  } catch (error) {
    console.error("Error get a bicycle", error);
    return null;
  }
};

const create = async (bicycle) => {
  try {
    const response = await http.post("/v2/bicycle", bicycle,
      {
        headers: {
          "Content-type": "application/json"
        },
      });
      console.log(response);
    return response.data;
  } catch (error) {
    console.error("Error post bicycle", error);
    return null;
  }
};

const update = async (id, updatedData) => {
  try {
    const response = await http.put("/v2/bicycle/" + id, updatedData, {
      headers: {
        "Content-Type": "application/json",
      },
    });
    return response.data;
  } catch (error) {
    console.error("Error updating bicycle", error);
    return null;
  }
};

const remove = async (id) => {
  try {
    const response = await http.delete("/v2/bicycle/" + id);
    return response.data;
  } catch (error) {
    console.error("Error delete bicycle", error);
    return null;
  }
};

const BicycleService = {
  getBicycles,
  getBicycle,
  create,
  update,
  remove
};

export default BicycleService;