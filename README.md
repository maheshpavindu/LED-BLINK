
# ESP32 Web Server with Mobile-Responsive UI & Persistent Timers (PlatformIO)

An IoT project built for the **ESP32** using **PlatformIO** and the Arduino framework. It hosts a local web server via **LittleFS** featuring a modern, mobile-responsive user interface with customizable ON/OFF timers that persist even after a power reboot using ESP32's **Preferences** (Flash memory).

---

## 🚀 Features

* **Single-Chip Web Server:** Runs entirely on an ESP32 using built-in `WiFi.h` and `WebServer.h` libraries.
* **LittleFS Integration:** HTML, CSS, and JavaScript files are stored separately in the ESP32 flash file system.
* **Mobile-Responsive UI:** Optimized card layout with smooth styling tailored for smartphones and desktop browsers.
* **Configurable Timers:** Adjust LED ON and OFF durations directly from the web interface.
* **Persistent Storage (EEPROM/Preferences):** Custom timer settings are saved in the ESP32's non-volatile memory, ensuring they are retained after restarts or power outages.
* **Non-Blocking Execution:** Uses `millis()` for timing intervals alongside `server.handleClient()` to keep the server responsive during delays.

---

## 📂 Project Structure

```text
YourProject/
├── data/                  
│   ├── index.html    # Web interface frontend
│   ├── style.css     # Mobile-responsive CSS styles
│   └── script.js     # JavaScript logic and AJAX requests
├── src/
│   └── main.cpp      # C++ code for ESP32 Web Server & Preferences
└── platformio.ini    # PlatformIO project configuration


## platformio.ini Configuration

[env:esp32doit-devkit-v1]
platform = espressif32
board = esp32doit-devkit-v1
framework = arduino
monitor_speed = 115200
board_build.filesystem = littlefs


📥 Installation & Flashing Instructions
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

Upload File System Image (LittleFS):
pio run --target uploadfs
