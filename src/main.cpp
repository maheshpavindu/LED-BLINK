#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>
#include <Preferences.h> // ESP32 හි දත්ත Permanent Save කිරීමට භාවිතා කරන 라이බ්‍රිය

const char* ssid = "MAHESH";
const char* password = "200307055";

const int ledPin = 2; // ESP32 Built-in LED
WebServer server(80);
Preferences preferences; // Preferences object එක සෑදීම

int onDelay = 2000;  
int offDelay = 2000; 

void handleFile(String path, String contentType) {
  if (LittleFS.exists(path)) {
    File file = LittleFS.open(path, "r");
    server.streamFile(file, contentType);
    file.close();
  } else {
    server.send(404, "text/plain", "File Not Found");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Preferences (Flash Memory) ආරම්භ කර පෙර Save කළ දත්ත ලබා ගැනීම
  preferences.begin("led-store", false);
  onDelay = preferences.getInt("onTime", 2) * 1000;   // නැති නම් පෙරනිමියෙන් තත්පර 2ක් දෙයි
  offDelay = preferences.getInt("offTime", 2) * 1000; 

  if (!LittleFS.begin(true)) {
    Serial.println("LittleFS Mount දෝෂයක්!");
    return;
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Wi-Fi වෙත සම්බන්ධ වෙමින්...");
  }
  
  Serial.println("\nWi-Fi සම්බන්ධ විය!");
  Serial.print("IP Address එක: http://");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, []() { handleFile("/index.html", "text/html"); });
  server.on("/style.css", HTTP_GET, []() { handleFile("/style.css", "text/css"); });
  server.on("/script.js", HTTP_GET, []() { handleFile("/script.js", "application/javascript"); });

  // දැනට Save වී ඇති කාලයන් JSON ფორමැට් එකෙන් බ්‍රව්සරයට යැවීම
  server.on("/getTimes", HTTP_GET, []() {
    String json = "{\"on\":" + String(onDelay / 1000) + ",\"off\":" + String(offDelay / 1000) + "}";
    server.send(200, "application/json", json);
  });

  // අලුත් කාලයන් ලබාගෙන Flash මතකයේ Save කිරීම
  server.on("/setTimer", HTTP_GET, []() {
    if (server.hasArg("on") && server.hasArg("off")) {
      int newOn = server.arg("on").toInt();
      int newOff = server.arg("off").toInt();

      // Preferences වල දත්ත Save කිරීම (Power එක ගියත් මකී යන්නේ නැත)
      preferences.putInt("onTime", newOn);
      preferences.putInt("offTime", newOff);

      onDelay = newOn * 1000;
      offDelay = newOff * 1000;
      
      server.send(200, "text/plain", "Saved Successfully");
    } else {
      server.send(400, "text/plain", "Bad Request");
    }
  });

  server.begin();
  Serial.println("Web Server ආරම්භ විය!");
}

void loop() {
  server.handleClient();

  // Save වී ඇති කාලයන්ට අනුව ස්වයංක්‍රීයව LED එක ON/OFF වීම
  digitalWrite(ledPin, HIGH);
  
  // delay කාලය අතරතුර වුවද server requests බලා ගැනීමට server.handleClient() පාවිච්චි කළ යුතුය
  unsigned long startMillis = millis();
  while (millis() - startMillis < (unsigned long)onDelay) {
    server.handleClient();
  }

  digitalWrite(ledPin, LOW);
  startMillis = millis();
  while (millis() - startMillis < (unsigned long)offDelay) {
    server.handleClient();
  }
}