# 🔋 DJI Mavic Air 1 Battery Unlocker (Arduino Mega 2560)

This project provides the full Arduino code to unlock (unseal) and clear the Permanent Failure (PF) flags from Mavic Air 1 smart batteries. It is designed to run on an Arduino Mega 2560, but it may also be portable to other Arduino-compatible boards with I2C capabilities.

---

## 🚨 Why Do Mavic Air 1 Batteries Lock Themselves?

DJI smart batteries contain internal protection logic, including:
* Cycle count limits
* Over-discharge or over-temperature events
* Firmware-triggered permanent failure (PF) flags
Once certain thresholds are met, the battery enters a locked or sealed state, making it unusable unless reset via software. This project allows you to reverse that state by issuing Manufacturer Block Access (MBA) commands directly over I2C (SMBus) to the battery.

---

## ✅ Features
* Automatically unseals the battery
* Temporarily disables the Permanent Failure logic
* Clears DJI-specific PF2 registers
* Prints detailed battery state and status flags
* Does not require commercial software or a CP2112 USB-to-I2C adapter
* Free and open-source

---

## 🛠️ Requirements

* ✅ Arduino Mega 2560 (tested and recommended)
* ✅ Battery from DJI Mavic Air 1
* ✅ Connect battery’s SDA/SCL lines to Arduino (usually via JST or breakout connector)
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

* This tool was developed for the Arduino Mega 2560, but may work on other boards. You might need to:
  * Adjust I2C pins
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


Made with ❤️
