const http = require('http');
const fs = require('fs');
const path = require('path');
const WebSocket = require('ws');

const PORT = process.env.PORT || 8080;

// ---- HTTP server (serves controller page) ----
const server = http.createServer((req, res) => {
  console.log("Request:", req.url);
  
  // Add CORS headers
  res.setHeader('Access-Control-Allow-Origin', '*');
  res.setHeader('Access-Control-Allow-Methods', 'GET, POST, OPTIONS');
  res.setHeader('Access-Control-Allow-Headers', 'Content-Type');

  if (req.url === '/') {
    fs.readFile('./controller.html', (err, data) => {
      if (err) {
        res.writeHead(500);
        return res.end("Error loading controller");
      }
      res.writeHead(200, { 'Content-Type': 'text/html' });
      res.end(data);
    });

  } else if (req.url === '/chest') {
    fs.readFile('./player.html', (err, data) => {
      if (err) {
        console.error(err);
        res.writeHead(500);
        return res.end("Error loading player");
      }
      res.writeHead(200, { 'Content-Type': 'text/html' });
      res.end(data);
    });

  } else if (req.url.startsWith('/audio/')) {
    const filePath = path.join(__dirname, req.url);
    fs.readFile(filePath, (err, data) => {
      if (err) {
        res.writeHead(404);
        return res.end("Audio not found");
      }
      res.writeHead(200, { 'Content-Type': 'audio/mpeg' });
      res.end(data);
    });

  } else if (req.url === '/favicon.ico') {
    res.writeHead(200, { 'Content-Type': 'image/x-icon' });
    res.end('');
  } else {
    res.writeHead(404);
    res.end("Not found");
  }
});

// ---- WebSocket server ----
const wss = new WebSocket.Server({ 
  server,
  perMessageDeflate: false // Disable compression for better compatibility
});

// Heartbeat interval for all clients
const HEARTBEAT_INTERVAL = 30000; // 30 seconds

function heartbeat() {
  wss.clients.forEach(client => {
    if (client.readyState === WebSocket.OPEN) {
      client.ping();
    }
  });
}

// Start heartbeat interval
setInterval(heartbeat, HEARTBEAT_INTERVAL);

wss.on('connection', (ws, req) => {
  console.log(`Client connected from: ${req.socket.remoteAddress}`);
  ws.isAlive = true;

  // Handle pong responses
  ws.on('pong', () => {
    ws.isAlive = true;
  });

  ws.on('message', (msg) => {
    const message = msg.toString().trim();
    
    // Handle ping messages from client
    if (message === 'ping') {
      ws.send('pong');
      return;
    }
    
    console.log('Received command:', message);
    
    // broadcast to all clients (player phones)
    wss.clients.forEach(client => {
      if (client.readyState === WebSocket.OPEN) {
        try {
          client.send(message);
        } catch (error) {
          console.error('Failed to send message to client:', error);
        }
      }
    });
  });

  ws.on('close', (code, reason) => {
    console.log(`Client disconnected, code: ${code}, reason: ${reason}`);
    ws.isAlive = false;
  });

  ws.on('error', (error) => {
    console.error('WebSocket error:', error);
  });
});

// Clean up dead connections
setInterval(() => {
  wss.clients.forEach(client => {
    if (!client.isAlive) {
      console.log('Terminating dead connection');
      client.terminate();
      return;
    }
    client.isAlive = false;
    client.ping();
  });
}, HEARTBEAT_INTERVAL);

server.listen(PORT, () => {
  console.log(`Server running: http://localhost:${PORT}`);
});