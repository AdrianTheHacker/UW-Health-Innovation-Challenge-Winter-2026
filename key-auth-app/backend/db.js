const sqlite3 = require("sqlite3").verbose();
const db = new sqlite3.Database("users.db");

db.serialize(() => {
    db.run(`
  CREATE TABLE IF NOT EXISTS users (
    key TEXT PRIMARY KEY,
    name TEXT,
    age INTEGER,
    sex TEXT,
    role TEXT,         -- 'doctor' or 'patient'
    doctor_key TEXT    -- NULL for doctors, key of assigned doctor for patients
  )
`);

  db.run(`
  CREATE TABLE IF NOT EXISTS heart_rates (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    timestamp TEXT,
    key TEXT,
    heart_rate INTEGER,
    FOREIGN KEY (key) REFERENCES users(key),
    UNIQUE(key, timestamp, heart_rate)
  )
`);

  // Insert sample user
db.run(`
  INSERT OR IGNORE INTO users VALUES
  ('abc123', 'Alice', 22, 'F', 'patient', 'doc001'),
  ('xyz789', 'Bob', 30, 'M', 'patient', 'doc001'),
  ('doc001', 'Dr. Smith', 40, 'M', 'doctor', NULL)
`);
});

    db.run(`
  INSERT OR IGNORE INTO heart_rates (timestamp, key, heart_rate) VALUES
  ('2026-02-07T10:00:00Z', 'abc123', 72),
  ('2026-02-07T10:01:00Z', 'abc123', 75),
  ('2026-02-07T10:02:00Z', 'xyz789', 65)
`);

module.exports = db;
