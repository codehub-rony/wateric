const express = require("express");
const webSocket = require("ws");
let http = require("http");

const app = express();
const server = http.createServer(app);

const s = new webSocket.Server({ server });

s.on("connection", function(ws, req) {
  const ip = req.socket.remoteAddress;
  ws.on("message", function(message) {
    console.log(JSON.parse(message));
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
