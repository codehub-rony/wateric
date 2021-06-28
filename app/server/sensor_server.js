const express = require("express");
const webSocket = require("ws");
const db = require('./db/db.js')
let http = require("http");

require('dotenv').config();

const app = express();
const server = http.createServer(app);

const s = new webSocket.Server({ server });

s.on("connection", function(ws, req) {
  const ip = req.socket.remoteAddress;
  ws.on("message", function(message) {
    console.log(JSON.parse(message));
    let data = JSON.parse(message)

    const text = 'INSERT INTO soil_measurements(sensor_id, temp, moisture) VALUES($1, $2, $3) returning id'
    const values = [1, int(data['temp']), data['moisture']]

    db.query(text, values).then(res => {
      console.log(res.rows[0])
    })
    .catch(e => console.error(e.stack))
    // s.clients.forEach(function(client) {
    //   if (client.readyState) {
    //     client.send("broadcast: " + message);
    //   }
    // });
  });
  ws.on("close", function() {
    console.log("client disconnected");
  });
  console.log("client connected");
});
server.listen(3000);
