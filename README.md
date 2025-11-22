# 🌡️ DHT22 Sensor Data Acquisition & Calibration Suite

![Project Status](https://img.shields.io/badge/Status-Active-success)
![Language](https://img.shields.io/badge/Language-C%2B%2B%20%7C%20Arduino-blue)

## 🚀 About The Project

This project is a comprehensive end-to-end solution for acquiring, logging, and calibrating environmental data using the **DHT22** sensor. It encompasses the entire data pipeline: from embedded firmware reading raw sensor values, to a C++ desktop application for UART data logging, and finally, statistical analysis for sensor calibration.

The system aims to minimize sensor error using **Linear Regression** and **Standard Deviation** analysis, providing a corrected and reliable temperature output...

---

## 📂 Project Architecture

The repository is organized into three main modules:

### 1️⃣ 🔌 Firmware (Arduino)
`📂 sketch_nov21a`
* **Role:** Embedded Firmware.
* **Description:** Runs on the Arduino microcontroller. It reads raw temperature and humidity data from the DHT22 sensor and transmits it via **UART** (Serial Communication) to the host machine.

### 2️⃣ 📡 Data Logger (Serial to CSV)
`📂 serialPort2CSV_DHT22`
* **Role:** Desktop Data Logger.
* **Technology:** C++ / Serial API.
* **Description:** Listens to the serial port, parses incoming data packets, and logs them into structured **.csv** files with precise timestamps for further analysis.

### 3️⃣ 📊 Data Analysis & Calibration
`📂 standart_dev_calculation_for_Rep`
* **Role:** Statistical Analysis & Calibration.
* **Description:** Processes the logged CSV data to calculate standard deviations.

---

## 📈 Calibration Results

Based on tests conducted at three distinct reference points (Cold, Room, Super Cold), the following calibration model was derived to correct the sensor readings:

| Reference Temp (°C) | Measured Mean (°C) | Status |
| :--- | :--- | :--- |
| **23.8** | 23.72 | Room |
| **10.0** | 10.28 | Cold |
| **3.8** | 3.77 | Super Cold |

---

## 🛠️ Getting Started

### Prerequisites
* Arduino IDE
* Visual Studio (for C++ Data Logger)

### Installation & Usage
1.  **Firmware:** Open `sketch_nov21a`, connect your Arduino with the DHT22 sensor, and upload the code.
2.  **Logging:** Open the `serialPort2CSV_DHT22` solution, build it, and run. Ensure the correct COM port is selected. The data will be saved to a CSV file.
3.  **Analysis:** Use the tools in `standart_dev_calculation_for_Rep` to process your own datasets.

---

## 📬 Contact

Developers: **[Samet Can Şahin and Murat Can Mutlu]**
Project Link: [https://github.com/SametCanSahin/Sensor_Reading_Project](https://github.com/SametCanSahin/Sensor_Reading_Project)
