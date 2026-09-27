# -----------------------------------------------------------------
# 🧪 Sensor Calibration & Threshold Guide
# -----------------------------------------------------------------

> Operational guidelines for calibrating the MQ-2 Gas/Smoke Sensor and IR Flame Sensor modules used in the Fire & Smoke Detection System.

---

## 📌 Calibration Overview & Hardware Setup

| Sensor Module | Target Pin | Sensing Range / Metric | Default Operational Threshold |
| :--- | :--- | :--- | :--- |
| **MQ-2 Gas / Smoke Sensor** | `Pin A1` | 0 – 1023 (Analog Gas Concentration) | `SMOKE_THRESHOLD = 110` (Triggers above) |
| **IR Flame Optical Sensor** | `Pin A0` | 0 – 1023 (Analog IR Light Intensity) | `FLAME_THRESHOLD = 500` (Triggers below) |

---

## 🛠 Calibration Procedures

### 1. MQ-2 Smoke & Gas Sensor Calibration

1. **Pre-Heating Phase:** Power on the Arduino and wait **10 seconds** for the internal heater coil to reach operating equilibrium.
2. **Baseline Ambient Reading:** Open the Serial Monitor at `9600 Baud`. Record the ambient air reading (typically reads between `40 – 80` in clean conditions).
3. **Threshold Calibration:**
   - Expose the sensor to a controlled smoke or gas source.
   - Observe the analog value increase beyond `110`.
   - Adjust the potentiometer on the physical MQ-2 module or update `SMOKE_THRESHOLD` in `src/fire_smoke_detection.ino` if ambient noise causes false positives.

### 2. IR Flame Sensor Calibration

1. **Ambient Baseline:** Measure ambient room IR light values in standard lighting (typically reads above `700 – 900`).
2. **Flame Detection Trigger:** 
   - Expose the sensor to an open flame target at a distance of 10–30 cm.
   - The value drops significantly (typically below `300`).
3. **Threshold Tuning:**
   - Adjust the onboard trimpot on the IR sensor module until the digital LED indicator turns ON when a flame is present.
   - Fine-tune `FLAME_THRESHOLD = 500` in code if environment IR interference requires higher sensitivity.

---

## 📡 Serial Telemetry Calibration Command

Send the `s` or `S` command over Bluetooth / Serial Terminal to review active sensor values in real-time during calibration routines:

```text
==================================
       SYSTEM HEALTH REPORT       
==================================
System Uptime      : 45 seconds
Fire Latch State   : STANDBY (CLEAR)
Fire Incidents     : 0
Smoke Incidents    : 0
Audio Mute Status  : INACTIVE
==================================