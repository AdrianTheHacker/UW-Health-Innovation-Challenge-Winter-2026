import { useState, useEffect, useRef } from "react";
import "./App.css";
import { io } from "socket.io-client";
import jsPDF from "jspdf";
import autoTable from "jspdf-autotable";

 const HARD_CODED_VALUES = {
    heartRateRange: "Comfort: 60-100, Monitor: 100-120, Stop exercise: >120",
    exerciseList: [
      "Bike (New Step)",
      "Cable row (Techno)",
      "Chest press (Hammer)",
      "Leg press (Techno)",
      "Step up (Stairs)",
      "Hip flexion (Parallel bars)",
      "Shoulder raise (Dumbells)"
    ],
    repsTime: [
      "15 min",
      "10 reps (bilateral)",
      "10 reps (bilateral)",
      "10 reps (unilateral)",
      "10 reps (unilateral)",
      "15 reps (unilateral)",
      "15 reps (unilateral)",
      "15 reps (unilateral)"
    ],
    weightIntensity: [
      "L1 - 5 min",
      "L2 - 10 min",
      "40 lbs",
      "30 lbs",
      "80 lbs",
      "NA",
      "NA",
      "5 lbs"
    ]
  };

function App() {
  const [key, setKey] = useState("");
  const [user, setUser] = useState(null);
  const [error, setError] = useState("");

  // Create a socket ref (singleton)
  const socketRef = useRef();

  useEffect(() => {
    // Initialize socket only once
    socketRef.current = io("http://localhost:5000");

    // Listen for new heart rate events
    socketRef.current.on("new-heart-rate", (data) => {
      console.log("New data from device:", data);
      // Optional: update state if current patient matches
      if (user && user.role === "patient" && user.key === data.key) {
        setUser((prev) => ({
          ...prev,
          heart_rates: [data, ...prev.heart_rates] // prepend new record
        }));
      }
    });

    return () => {
      socketRef.current.disconnect();
    };
  }, [user]); // dependency on user so we can update patient's heart rate live

  const login = async () => {
    const res = await fetch("http://localhost:5000/login", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ key }),
    });
    const data = await res.json();
    if (!res.ok) {
      setError(data.message);
      return;
    }
    setUser(data.user);
    setError("");
  };

  const exportPatientPDF = () => {
  if (!user || user.role !== "patient") return;

  const doc = new jsPDF();
  doc.setFontSize(16);
  doc.text(`Patient Report: ${user.name}`, 14, 20);
  doc.setFontSize(12);
  doc.text(`Age: ${user.age}, Sex: ${user.sex}`, 14, 28);

  const tableColumn = ["#", "Timestamp", "Heart Rate"];
  const tableRows = user.heart_rates
    .sort((a, b) => new Date(a.timestamp) - new Date(b.timestamp))
    .map((row, index) => [index + 1, row.timestamp, row.heart_rate]);

  
  autoTable(doc, {
    head: [tableColumn],
    body: tableRows,
    startY: 36,
  });

  doc.save(`${user.name}_heart_rate_report.pdf`);
};


if (!user) {
  return (
    <div className="login-container">
      <h1>Key Login</h1>
      <input
        value={key}
        onChange={(e) => setKey(e.target.value)}
        placeholder="Enter your key"
      />
      <button className="login-button" onClick={login}>
        Login
      </button>
      {error && <p className="error-message">{error}</p>}
    </div>
  );
}

// PatientDashboard.jsx
if (user && user.role === "patient") {
  return (
    <div className="dashboard-container">

      {/* Profile Card */}
      <div className="card profile-card">
        <h1>👋 Welcome, {user.name}</h1>
        <p>
          Age: <b>{user.age}</b> • Sex: <b>{user.sex}</b>
        </p>
      </div>

      {/* Heart Rate Records */}
      <div className="card">
        <h3>❤️ Heart Rate Records</h3>

        <table className="heart-table">
          <thead>
            <tr>
              <th>#</th>
              <th>Timestamp</th>
              <th>Heart Rate (BPM)</th>
            </tr>
          </thead>
          <tbody>
            {user.heart_rates
              .sort((a, b) => new Date(a.timestamp) - new Date(b.timestamp))
              .map((r, i) => (
                <tr key={i}>
                  <td>{i + 1}</td>
                  <td>{r.timestamp}</td>
                  <td className={r.heart_rate > 120 ? "bpm-high" : "bpm-normal"}>
                    {r.heart_rate}
                  </td>
                </tr>
              ))}
          </tbody>
        </table>

        <div className="export-btn-container">
          <button onClick={exportPatientPDF} className="export-btn">
            📄 Export PDF
          </button>
        </div>
      </div>

    </div>
  );
}



  if (user.role === "doctor") {
  return (
    <div className="doctor-container">
      <h1 style={{ textAlign: "center", marginBottom: "20px" }}>
        Doctor Dashboard: {user.name}
      </h1>
      <table className="doctor-table">
        <thead>
          <tr>
            <th>User Info</th>
            <th>Heart Rate / Exercise</th>
            <th>Heart Rate Range / Reps</th>
            <th>Weight/Intensity</th>
          </tr>
        </thead>
        <tbody>
          {user.patients.map((p) => (
            <>
              <tr key={p.key + "-1"}>
                <td rowSpan="2">
                  <strong>{p.name}</strong>
                  <br />
                  Age: {p.age}, Gender: {p.sex}
                </td>
                <td>{p.heart_rate || "-"}</td>
                <td>{HARD_CODED_VALUES.heartRateRange}</td>
                <td></td>
              </tr>
              <tr key={p.key + "-2"}>
                <td>
                  {HARD_CODED_VALUES.exerciseList.map((ex, i) => (
                    <span key={i}>
                      {ex}
                      <br />
                    </span>
                  ))}
                </td>
                <td>
                  {HARD_CODED_VALUES.repsTime.map((r, i) => (
                    <span key={i}>
                      {r}
                      <br />
                    </span>
                  ))}
                </td>
                <td>
                  {HARD_CODED_VALUES.weightIntensity.map((w, i) => (
                    <span key={i}>
                      {w}
                      <br />
                    </span>
                  ))}
                </td>
              </tr>
            </>
          ))}
        </tbody>
      </table>
    </div>
  );
}


  return null;
}

export default App;
