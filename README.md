# 🔋 DJI Mavic Air 1 Battery Unlocker (Arduino Mega 2560 / Uno R3)

This project provides the full Arduino code to unlock (unseal) and clear the Permanent Failure (PF) flags from Mavic Air 1 smart batteries. It was originally designed to run on an Arduino Mega 2560, and now also officially supports the Arduino Uno R3 (and should be portable to other Arduino-compatible boards with I2C capabilities).

All battery command/description tables are stored in flash memory (`PROGMEM`) instead of RAM, which is what makes the sketch fit comfortably within the Uno R3's 2KB of SRAM.

---

## 🚨 Why Do Mavic Air 1 Batteries Lock Themselves?

DJI smart batteries contain internal protection logic, including:
* Cycle count limits
* Over-discharge or over-temperature events
* Firmware-triggered permanent failure (PF) flags
Once certain thresholds are met, the battery enters a locked or sealed state, making it unusable unless reset via software. This project allows you to reverse that state by issuing Manufacturer Block Access (MBA) commands directly over I2C (SMBus) to the battery.

---

## ✅ Steps
* Connect battery to arduino (some screens are availibale in the "screen" directiry if needed)
* Connect arduino to PC with arduino
* Upload code to arduino 
* Open Serial Monitor
* Check connexion
* If you cannot connect, your battery is probably under voltage
  * You need to open it and charge it a bit manualy there is many youtube video about this)
* Control battery informations
* Close Serial Monitor
* Change UNLOCK_ACTIVETED to true and send code again
* Open Serial Monitor
* Control logs
* Wait the message "You can disconnect and test your battery now."

---

## 🛠️ Requirements

* ✅ Arduino Mega 2560 or Arduino Uno R3 (both tested)
* ✅ Battery from DJI Mavic Air 1
* ✅ Connect battery’s SDA/SCL lines to Arduino (usually via JST or breakout connector)
  * On Mega 2560: SDA = pin 20, SCL = pin 21
  * On Uno R3: SDA = pin A4, SCL = pin A5
* ✅ Basic knowledge of wiring and Arduino IDE

---

## 📦 How It Works

This project uses SMBus block access commands (also known as ManufacturerBlockAccess) to communicate with the battery’s embedded controller (BQ series). The `runMBACommand()` function looks up a command by name (e.g., `UnsealKey1`, `ClearPF2`), sends it, and optionally reads the response.
Key steps performed:
1. Read battery and safety states
2. Send unlock keys (UnsealKey1 & UnsealKey2)
3. Temporarily disable PF logic
4. Clear DJI PF2 register
5. Re-enable PF logic
6. Reset battery
7. Print final status
Each command uses `Wire` (I2C) to send data, wait for a response, and parse it with endianness conversion where needed.

---

## 📌 Usage Notes

* This tool was developed for the Arduino Mega 2560 and has also been validated on the Arduino Uno R3. It may work on other boards too. You might need to:
  * Adjust I2C pins (Wire.begin() uses the board's default I2C pins automatically)
  * Replace the `Wire` library with `SoftwareWire` if needed
  * Adapt the buffer size constraints
* All output is sent to the Serial Monitor at 9600 baud.
* Be patient: some commands (especially DeviceReset) require delays

---

## ⚠️ Disclaimer

This tool is provided as-is for educational and personal repair purposes.
> You are fully responsible for how you use this code and for any damage or data loss caused by it.
> Batteries are hazardous and improper use may cause fire, explosion, or permanent damage.
Use responsibly. No warranty is provided. Always take proper safety precautions when working with LiPo batteries.

---

## 🪪 License

This project is licensed under the **MIT License** — see the [LICENSE](LICENSE) file for details.

---

## 🧠 Acknowledgments

This project was inspired by the DJI reverse-engineering and repair community. It aims to make battery servicing more accessible for users.
