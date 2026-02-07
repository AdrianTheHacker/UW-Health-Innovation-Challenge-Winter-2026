// server.js
const express = require("express");
const cors = require("cors");
const db = require("./db");
const importCSV = require("./import_csv");
const http = require("http");
const { Server } = require("socket.io");

const app = express();
app.use(cors());
app.use(express.json());

// ===== CREATE HTTP SERVER FOR SOCKET.IO =====
const server = http.createServer(app);

// ===== ATTACH SOCKET.IO =====
const io = new Server(server, { cors: { origin: "*" } });

io.on("connection", (socket) => {
  console.log("Frontend connected via WebSocket");

  socket.on("disconnect", () => {
    console.log("Frontend disconnected");
  });
});

// ===== AUTO IMPORT CSV ON SERVER START =====
importCSV();

// ===== LOGIN ENDPOINT =====
app.post("/login", (req, res) => {
  const { key } = req.body;

  db.get("SELECT * FROM users WHERE key = ?", [key], (err, user) => {
    if (err) return res.status(500).json({ message: err.message });
    if (!user) return res.status(401).json({ message: "Invalid key" });

    if (user.role === "patient") {
      db.all(
        "SELECT timestamp, heart_rate FROM heart_rates WHERE key=? ORDER BY timestamp DESC",
        [key],
        (err, rows) => {
          if (err) return res.status(500).json({ message: err.message });
          user.heart_rates = rows;
          res.json({ message: "Login success", user });
        }
      );
    } else if (user.role === "doctor") {
      // Get only patients of this doctor
      db.all(
        `
        SELECT u.key, u.name, u.age, u.sex, hr.heart_rate
        FROM users u
        LEFT JOIN heart_rates hr
          ON u.key = hr.key
          AND hr.timestamp = (
            SELECT MAX(timestamp)
            FROM heart_rates
            WHERE key = u.key
          )
        WHERE u.role='patient' AND u.doctor_key=?
        ORDER BY u.name
        `,
        [user.key],
        (err, rows) => {
          if (err) return res.status(500).json({ message: err.message });
          user.patients = rows;
          res.json({ message: "Login success", user });
        }
      );
    }
  });
});

// ===== DEVICE DATA ENDPOINT =====
app.post("/device-data", (req, res) => {
  const { key, timestamp, heart_rate } = req.body;

  if (!key || !timestamp || heart_rate === undefined) {
    return res.status(400).json({ message: "Missing parameters" });
  }

  // Insert into database with deduplication
  const sql = `
    INSERT OR IGNORE INTO heart_rates (key, timestamp, heart_rate)
    VALUES (?, ?, ?)
  `;
  db.run(sql, [key, timestamp, heart_rate], function (err) {
    if (err) return res.status(500).json({ message: err.message });

    // ===== EMIT REAL-TIME EVENT =====
    io.emit("new-heart-rate", { key, timestamp, heart_rate });

    res.json({ message: "Data received", id: this.lastID });
  });
});

// ===== START SERVER =====
const PORT = 5000;
server.listen(PORT, () => console.log(`Server running on http://localhost:${PORT}`));
