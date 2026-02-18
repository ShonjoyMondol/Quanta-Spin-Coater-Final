Here’s a clean GitHub-ready README you can paste directly into `README.md` 😊

---

# 🧪✨ BLDC Spin Coater with Nextion Touch UI (No Signal Cuts)

A fully stable **BLHeli ESC controlled spin coater** powered by Arduino and a beautiful **Nextion touchscreen interface**.
This project solves the common ESC signal-cut issue and delivers **smooth, continuous motor control** with a professional UI.

---

## 🎯 Project Highlights

✔️ No PWM signal drops
✔️ Smooth BLDC rotation even during display updates
✔️ Real-time countdown & progress bar
✔️ Clean and responsive Nextion UI
✔️ ESC calibrated automatically on boot
✔️ Designed for lab / coating / robotics projects

This build is perfect for:

* Thin film coating
* DIY lab equipment
* Robotics and motor control learning
* Embedded systems practice

---

## ⚠️ The BIG Problem This Project Solves

Most Arduino + ESC + Nextion projects suffer from:

❌ Motor stopping every second
❌ ESC losing signal when display updates
❌ Unstable low-RPM rotation

Cause:
`SoftwareSerial` blocks interrupts → ESC loses PWM → motor cuts.

💡 **Solution used in this project:**
We use **Hardware Serial** for the Nextion display so the Servo library can generate continuous PWM for the ESC.

Result → **Perfect uninterrupted motor signal** 🎉

---

# 🖥️ User Interface Features

The Nextion touchscreen provides a simple and beautiful control panel:

### Page 1 – Setup

* Set coating duration ⏱️
* Set RPM ⚙️
* Navigate to run page

### Page 2 – Run Screen

* Reverse countdown timer
* Real-time progress bar
* Start / Cancel control
* Status text:

  * *Coating…*
  * *Coating Done!*

---

# 🔌 Hardware Required

| Component                 | Description         |
| ------------------------- | ------------------- |
| Arduino UNO / Nano        | Main controller     |
| BLHeli_32 ESC (35A)       | Motor driver        |
| BLDC Motor                | Spin motor          |
| Nextion Touch Display     | User interface      |
| Power Supply / LiPo       | ESC power           |
| 1000–2200µF 35V Capacitor | ESC power smoothing |
| 0.1µF Ceramic Capacitor   | Noise filtering     |

---

# 🔧 Wiring Connections

## 🎛️ ESC Connection

| ESC Wire | Arduino   |
| -------- | --------- |
| Signal   | **Pin 9** |
| GND      | GND       |

---

## 📺 Nextion Display (Hardware Serial)

| Nextion Wire | Arduino Pin    |
| ------------ | -------------- |
| TX (Blue)    | **RX – Pin 0** |
| RX (Yellow)  | **TX – Pin 1** |

⚠️ Disconnect RX/TX while uploading code.

---

## 🔋 Power Stabilization (Important)

Add capacitors across ESC power input:

* **1000–2200 µF 35V Low-ESR electrolytic**
* **0.1 µF ceramic capacitor**

Place them **close to ESC power pads**.

This improves:

* Low RPM smoothness
* Voltage stability
* Noise reduction

---

# 🧠 How It Works

### 1️⃣ ESC Calibration on Boot

On startup the ESC receives:

1. Maximum throttle
2. Minimum throttle

This ensures correct throttle range detection.

---

### 2️⃣ RPM Control

The RPM is mapped to ESC pulse width:

| RPM  | PWM Signal |
| ---- | ---------- |
| 3000 | 875 µs     |
| 7000 | 2200 µs    |

Arduino continuously sends PWM using the Servo library.

---

### 3️⃣ Non-Blocking Countdown Timer

The project uses `millis()` instead of `delay()`.

This allows:

* Continuous PWM output
* Real-time UI updates
* No motor interruptions

---

### 4️⃣ Hardware Serial Communication

The Nextion communicates using the Arduino UART.

Because interrupts remain active:

* PWM never stops
* ESC never loses signal
* Motor spins smoothly

---

# 🚀 How To Upload & Run

1️⃣ Disconnect Nextion RX/TX wires
2️⃣ Upload the Arduino sketch
3️⃣ Reconnect Nextion wires
4️⃣ Power the ESC
5️⃣ Enjoy your spin coater 🎉

---

# 📈 Future Improvements Ideas

* Smooth acceleration ramp (soft start)
* Multi-step spin profiles
* EEPROM preset saving
* Emergency stop button
* RPM closed-loop control (tachometer)

---

# ❤️ Acknowledgment

This project was built to create a **reliable and affordable spin coater** using accessible components and clean embedded design.

If this helped you, consider ⭐ starring the repo!

---

Happy Building 🛠️✨
