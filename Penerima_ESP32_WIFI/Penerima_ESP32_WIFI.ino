#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "ESP32_AP";
const char* password = "12345678";

WebServer server(80); // Web server pada port 80

const int ledPin = 2; // Pin LED bawaan ESP32 (bisa diubah sesuai kebutuhan)

void handleRoot() {
  // Kedipkan LED saat ada permintaan
  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);

  // Kirim respon ke klien
  server.send(200, "text/plain", "LED Blinked!");
}

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);

  // Jalankan WiFi dalam mode AP
  WiFi.softAP(ssid, password);

  Serial.println("Access Point Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  // Atur handler untuk root URL
  server.on("/", handleRoot);
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient(); // Tangani permintaan HTTP
}