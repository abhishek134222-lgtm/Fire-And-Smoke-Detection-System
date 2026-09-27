# -----------------------------------------------------------------
# ⚡ Fire & Smoke Detection System — Core Module Reference
# -----------------------------------------------------------------

> Operational documentation for `src/fire_smoke_detection.ino`. Defines pin assignments, command structures, system timing specs, and state transition behaviors.

---

## 📌 Hardware Architecture & Pin Map

The system interfaces analog environmental sensors with digital visual/audio indicators and an active serial communication link over Bluetooth.

| Component | Arduino Pin | Signal Type | Operating Threshold / State Description |
| :--- | :--- | :--- | :--- |
| **IR Flame Sensor** | `Pin A0` | Analog Input | Active LOW hazard detection below **500** analog threshold |
| **MQ-2 Gas / Smoke Sensor** | `Pin A1` | Analog Input | Active HIGH hazard detection above **110** analog threshold |
| **Status LED (Safe)** | `Pin 2` | Digital Output | Drives **Green LED** (`HIGH` when operational and clear) |
| **Alert LED (Hazard)** | `Pin 3` | Digital Output | Drives **Red LED** (`HIGH` during warm-up or active hazard) |
| **Primary Flame Alarm** | `Pin 9` | Digital Output | Piezo Buzzer with rapid **120ms** pulse cadence |
| **Secondary Smoke Alarm** | `Pin 10` | Digital Output | Piezo Buzzer with rhythmic **250ms** pulse cadence |
| **HC-05 Bluetooth Interface**| `Pins 0 (RX) / 1 (TX)` | Hardware Serial | Asynchronous UART telemetry operating @ **9600 Baud** |

---

## 🛠 System Lifecycle & Operational Logic

```text
               [ POWER ON / RESET ]
                         │
                         ▼
        ┌──────────────────────────────────┐
        │  10-Second MQ-2 Sensor Warm-Up   │  <-- Dual LEDs illuminated
        └──────────────────────────────────┘
                         │
                         ▼
        ┌──────────────────────────────────┐
        │     SYSTEM ARMED & MONITORING    │  <-- Green LED ON / Red LED OFF
        └──────────────────────────────────┘
             │                        │
  Flame < 500│                        │Smoke > 110
             ▼                        ▼
  ┌──────────────────┐      ┌──────────────────┐
  │ FIRE LATCH STATE │      │   SMOKE ALERT    │
  └──────────────────┘      └──────────────────┘
  - Red LED Active          - Red LED Active
  - 120ms Buzzer Pulse      - 250ms Buzzer Pulse
  - Latched (Requires 'r')  - Auto-clears when smoke drops









  ==================================
       SYSTEM HEALTH REPORT       
==================================
System Uptime      : 142 seconds
Fire Latch State   : STANDBY (CLEAR)
Fire Incidents     : 0
Smoke Incidents    : 1
Audio Mute Status  : ACTIVE (Muted: 42s remaining)
==================================