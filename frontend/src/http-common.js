import axios from "axios";

export default axios.create({
  baseURL: "http://localhost:49080/",
  headers: {
    "Content-type": "application/json"
  }
});