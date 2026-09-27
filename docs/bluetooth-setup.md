# 📡 HC-05 Bluetooth Setup & Telemetry Guide

> **Author:** Bipanpreet  
> **Hardware:** HC-05 Serial Bluetooth Transceiver  
> **System:** Arduino Uno Fire & Smoke Detection System  

---

## 1. Module Specifications & Serial Configuration

The HC-05 module provides wireless telemetry and command-line remote control over standard UART serial communication.

* **Default Baud Rate:** `9600 bps` *(Data Bits: 8, Stop Bits: 1, Parity: None)*
* **Operating Voltage:** `3.3V` to `5V` DC *(Logic Level: 3.3V)*
* **Wireless Frequency:** `2.4 GHz` ISM Band
* **Signal Range:** $\approx 10\text{ meters}$ *(Line-of-Sight)*

---

## 2. Hardware Wiring & Pin Mapping

The HC-05 communicates directly via the Arduino Uno's Hardware Serial pins (`Pin 0` and `Pin 1`).

| HC-05 Pin | Arduino Uno Pin | Description | Connection Notes |
| :---: | :---: | :--- | :--- |
| **VCC** | **5V** | Power Supply | Connect to 5V rail. |
| **GND** | **GND** | Common Ground | Connect to system GND. |
| **TXD** | **Pin 0 (RX)** | Transmit Data | Direct connection to RX. |
| **RXD** | **Pin 1 (TX)** | Receive Data | ⚠️ **Requires Voltage Divider!** |

> ⚠️ **CRITICAL HARDWARE SAFETY WARNINGS**
>
> 1. **Disconnect RX/TX During Code Uploads:**  
>    Because the HC-05 uses `Pin 0 (RX)` and `Pin 1 (TX)`, you **must disconnect** the `TXD` and `RXD` jumper wires while uploading code via USB from the Arduino IDE, or the upload will fail.
>
> 2. **Voltage Divider on RXD Pin:**  
>    The HC-05 RXD pin operates on `3.3V` logic. Install a resistor voltage divider ($1\text{k}\Omega$ and $2\text{k}\Omega$) between Arduino `TX (Pin 1)` and HC-05 `RXD` to prevent logic level overvoltage damage.

---

## 3. Remote Telemetry Command Set

Send these ASCII character commands over a Bluetooth terminal app (e.g., *Serial Bluetooth Terminal*) to interact with the system in real time.

| Command | Action Name | System Response & Execution Logic |
| :---: | :--- | :--- |
| `r` / `R` | **Reset Alarm** | Clears the active latched fire alarm state, turns off red warning LEDs, and restores normal monitoring mode. |
| `s` / `S` | **System Report** | Transmits a full status report over Bluetooth showing current Uptime (ms), total Fire incidents logged, total Smoke incidents logged, and Mute state. |
| `t` / `T` | **Hardware Self-Test** | Triggers a 1-second diagnostic sequence: cycles Green and Red LEDs, and pulses the Fire/Smoke buzzers to verify hardware integrity. |
| `m` / `M` | **Mute Audio Alarms** | Temporarily mutes both warning buzzers for `60,000 ms` (60 seconds) during active hazard states while keeping visual LED alerts active. |

---

## 4. First-Time Setup & Terminal Pairing Instructions

1. **⚡ Power On:** Power the Arduino board. The HC-05 LED will blink rapidly, indicating it is ready to pair.
2. **📲 Pair Device:** Open Bluetooth settings on your smartphone/PC, search for `HC-05`, and pair using PIN `1234` or `0000`.
3. **💻 Open Terminal:** Launch your Bluetooth Terminal app and connect to the paired `HC-05` device.
4. **⚙️ Terminal Settings:** Set line endings to **`NL + CR`** (or **`Newline`**) at a baud rate of **`9600`**.
5. **✅ Verify Link:** Send `s` in the command box to receive your first diagnostic telemetry report.