# Watchdog Timer (WDT) Case Study — Arduino Nano 33 BLE Sense

![Arduino](https://img.shields.io/badge/Arduino-Nano%2033%20BLE%20Sense-00979D?logo=arduino&logoColor=white)
![SoC](https://img.shields.io/badge/SoC-nRF52840-0077C8?logo=nordic-semiconductor&logoColor=white)
![License](https://img.shields.io/badge/License-MIT-green.svg)

> **Author:** Akanksh Batra  
> **Platform:** Arduino Nano 33 BLE Sense (Nordic Semiconductor nRF52840)  
> **Core:** Mbed OS Nano Boards (Arduino IDE 2.x)

---

## 📌 Objective

Demonstrate the nRF52840 **hardware Watchdog Timer (WDT)** using bare-metal register access in an Arduino sketch.  
The sketch:
1. Configures a 3-second WDT timeout.
2. Feeds (pets) the WDT every 1 second for 10 iterations.
3. **Simulates a firmware hang** by entering a deliberate infinite loop.
4. Lets the WDT fire — triggering an automatic system reset.
5. On reboot, detects the WDT reset via the `POWER->RESETREAS` register.
6. Signals recovery with a rapid LED blink pattern.

---

## 🛠️ Requirements

### Hardware
| Component | Details |
|-----------|---------|
| Board | Arduino Nano 33 BLE Sense (Rev1 or Rev2) |
| Connection | USB-A to Micro-USB cable |

### Software
| Tool | Version |
|------|---------|
| Arduino IDE | 2.x (recommended: 2.3+) |
| Board core | **Arduino Mbed OS Nano Boards** (≥ 4.0.0) |
| Serial Monitor | 115 200 baud, Newline line ending |

> **Note:** No external libraries are required. The sketch uses the `<nrf.h>` header
> shipped with the Mbed OS core for direct register access.

---

## ⚙️ How It Works

The nRF52840 WDT peripheral uses a dedicated **32 768 Hz (LFCLK)** oscillator, independent of the CPU clock, making it robust against firmware lockups.

```
Counter Reload Value (CRV) = (timeout_ms × 32 768) / 1000
  → 3000 ms  →  CRV = 98 304
```

### Step-by-Step Flow

```
Step 1  ─ setup() configures WDT: CONFIG, CRV, RREN, TASKS_START
Step 2  ─ loop() calls feedWDT() → writes WDT_RR_RR_Reload to RR[0]
           (resets the internal counter; repeat every ~1 s)
Step 3  ─ After 10 successful feeds, sketch enters while(1) — no more feeds
Step 4  ─ WDT counter reaches CRV (≈ 3 s later) → hardware reset fires
Step 5  ─ On reboot, setup() reads POWER->RESETREAS
           DOG bit set? → WDT reset confirmed
Step 6  ─ Rapid LED blink (10× @ 100 ms) signals successful auto-recovery
```

### Key Registers

| Register | Purpose |
|----------|---------|
| `NRF_WDT->CONFIG` | HALT / SLEEP behaviour |
| `NRF_WDT->CRV` | Timeout counter reload value |
| `NRF_WDT->RREN` | Enable reload registers (RR0–RR7) |
| `NRF_WDT->TASKS_START` | Start the WDT (write 1; irreversible) |
| `NRF_WDT->RR[0]` | Feed register — write `0x6E524635` to pet |
| `NRF_POWER->RESETREAS` | Reset reason flags (DOG bit = WDT reset) |

---

## 🚀 How to Run

1. **Clone / download** this repository.
2. Open `WDT_Nano33BLE/WDT_Nano33BLE.ino` in **Arduino IDE 2.x**.
3. Select **Board → Arduino Mbed OS Nano Boards → Arduino Nano 33 BLE**.
4. Select the correct **Port**.
5. Click **Upload** (⬆️).
6. Open **Serial Monitor** at **115 200 baud**.

### Expected Behaviour Timeline

```
t = 0 s   → Sketch starts; "Normal power-on" message printed.
t = 0–10 s → LED blinks once per second; "WDT fed. Count = N" printed.
t ≈ 10 s  → "Simulating firmware hang…" printed; LED freezes.
t ≈ 13 s  → WDT fires; board auto-resets (no user intervention).
t ≈ 13 s+ → LED rapid-blinks ×10; "SYSTEM RECOVERED FROM A WATCHDOG RESET!" printed.
```

---

## 📟 Sample Serial Output

```
=========================================
 Nano 33 BLE Sense - WDT Case Study Demo
=========================================
Normal power-on / manual reset detected.
WDT configured with 3000 ms timeout.
Feeding WDT every 1000 ms...
-----------------------------------------
WDT fed. Count = 1
WDT fed. Count = 2
WDT fed. Count = 3
WDT fed. Count = 4
WDT fed. Count = 5
WDT fed. Count = 6
WDT fed. Count = 7
WDT fed. Count = 8
WDT fed. Count = 9
WDT fed. Count = 10
-----------------------------------------
Simulating firmware hang (infinite loop)...
WDT will NOT be fed from this point onward.
System will auto-reset in ~3 seconds.

--- [~3 second pause — WDT fires, board resets] ---

=========================================
 Nano 33 BLE Sense - WDT Case Study Demo
=========================================
>>> SYSTEM RECOVERED FROM A WATCHDOG RESET! <<<
>>> WDT functionality CONFIRMED working.    <<<
WDT configured with 3000 ms timeout.
Feeding WDT every 1000 ms...
-----------------------------------------
WDT fed. Count = 1
...
```

---

## 📁 Repository Structure

```
nano33ble-wdt-case-study/
├── WDT_Nano33BLE/
│   └── WDT_Nano33BLE.ino   ← Arduino sketch (main demo)
├── docs/
│   └── case_study.tex       ← LaTeX write-up (compile with pdfLaTeX / Overleaf)
├── media/                   ← Screenshots & demo video (add your own)
│   └── .gitkeep
├── README.md
└── LICENSE                  ← MIT License
```

---

## 📄 Documentation & Media

📄 **Full write-up:** [`docs/case_study.pdf`](docs/case_study.tex) *(compile the `.tex` file with pdfLaTeX or upload to [Overleaf](https://overleaf.com))*  
🎥 **Demo video:** [ADD LINK HERE]

---

## 🔧 Customisation

| Macro | Default | Description |
|-------|---------|-------------|
| `WDT_TIMEOUT_MS` | `3000` | WDT timeout in milliseconds |
| `FEED_INTERVAL_MS` | `1000` | Feed interval in milliseconds |
| `HANG_AFTER_FEEDS` | `10` | Number of feeds before simulated hang |

---

## 💡 Real-World Applications

- Embedded medical devices requiring guaranteed recovery
- Remote IoT sensors with no manual reset capability
- Industrial automation controllers in unattended environments
- Any safety-critical firmware requiring fault tolerance

---

## 📜 License

This project is licensed under the **MIT License** — see the [LICENSE](LICENSE) file for details.

© 2026 Akanksh Batra
