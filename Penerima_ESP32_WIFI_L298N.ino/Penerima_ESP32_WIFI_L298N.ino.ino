#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "ESP32_AP";
const char* password = "12345678";

WebServer server(80);

// Motor pins
const int ENA = 14;
const int IN1 = 26;
const int IN2 = 27;
const int IN3 = 25;
const int IN4 = 33;
const int ENB = 32;

const int speedA = 200;
const int speedB = 200;

void setupMotorPins() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  ledcAttach(ENA, 1000, 8);
  ledcAttach(ENB, 1000, 8);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  ledcWrite(ENA, 0);
  ledcWrite(ENB, 0);
}

void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  ledcWrite(ENA, speedA);
  ledcWrite(ENB, speedB);
}

void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  ledcWrite(ENA, speedA);
  ledcWrite(ENB, speedB);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  ledcWrite(ENA, speedA);
  ledcWrite(ENB, speedB);
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  ledcWrite(ENA, speedA);
  ledcWrite(ENB, speedB);
}

void handleRoot() {
  String html = R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <title>ESP32 Motor Control</title>
      <style>
        button {
          width: 100px;
          height: 40px;
          font-size: 16px;
          margin: 5px;
        }
      </style>
    </head>
    <body>
      <h2>ESP32 Motor Control</h2>
      <button onclick="sendCmd('forward')">Forward</button><br>
      <button onclick="sendCmd('left')">Left</button>
      <button onclick="sendCmd('stop')">Stop</button>
      <button onclick="sendCmd('right')">Right</button><br>
      <button onclick="sendCmd('backward')">Backward</button>

      <script>
        function sendCmd(cmd) {
          fetch('/' + cmd)
            .then(response => response.text())
            .then(data => console.log(data));
        }
      </script>
    </body>
    </html>
  )rawliteral";

  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  setupMotorPins();
  stopMotors();

  WiFi.softAP(ssid, password);
  Serial.println("Access Point started");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/forward", []() {
    moveForward();
    server.send(200, "text/plain", "Moving Forward");
  });
  server.on("/backward", []() {
    moveBackward();
    server.send(200, "text/plain", "Moving Backward");
  });
  server.on("/left", []() {
    turnLeft();
    server.send(200, "text/plain", "Turning Left");
  });
  server.on("/right", []() {
    turnRight();
    server.send(200, "text/plain", "Turning Right");
  });
  server.on("/stop", []() {
    stopMotors();
    server.send(200, "text/plain", "Stopped");
  });

  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
}
