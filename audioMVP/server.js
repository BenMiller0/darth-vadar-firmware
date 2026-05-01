const http = require('http');
const fs = require('fs');
const path = require('path');
const WebSocket = require('ws');

const PORT = process.env.PORT || 8080;

// ---- HTTP server (serves controller page) ----
const server = http.createServer((req, res) => {
  if (req.url === '/') {
    fs.readFile('./controller.html', (err, data) => {
      res.writeHead(200, { 'Content-Type': 'text/html' });
      res.end(data);
    });
  } else {
    res.writeHead(404);
    res.end();
  }
});

// ---- WebSocket server ----
const wss = new WebSocket.Server({ server });

wss.on('connection', (ws) => {
  console.log("Client connected");

  ws.on('message', (msg) => {
    // broadcast to all clients (player phones)
    wss.clients.forEach(client => {
      if (client.readyState === WebSocket.OPEN) {
        client.send(msg.toString());
      }
    });
  });

  ws.on('close', () => console.log("Client disconnected"));
});

server.listen(PORT, () => {
  console.log(`Server running: http://localhost:${PORT}`);
});