const fs = require("fs");
const path = require("path");
const csv = require("csv-parser");
const db = require("./db");

function importCSV() {
  const filePath = path.join(__dirname, "../test_file/file.csv");

  if (!fs.existsSync(filePath)) {
    console.log("CSV file not found, skipping import.");
    return;
  }

  fs.createReadStream(filePath)
  .pipe(csv())
  .on("data", (row) => {
    const user_id = row.user_id.trim();
    const timestamp = row.timestamp.trim();
    const heart_rate = parseInt(row.heart_rate);

    db.run(
      "INSERT OR IGNORE INTO heart_rates (timestamp, key, heart_rate) VALUES (?, ?, ?)",
      [timestamp, user_id, heart_rate],
      (err) => {
        if (err) console.error("DB insert error:", err.message);
      }
    );
  })
  .on("end", () => {
    console.log("CSV auto-import finished.");
  });
}

module.exports = importCSV;
