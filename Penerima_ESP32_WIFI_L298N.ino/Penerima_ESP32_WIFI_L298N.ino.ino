#include <WiFi.h>
#include <WebServer.h>
#include <math.h>

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

const int baseSpeed = 255;
const uint8_t maxTurnStrength = 64;
uint8_t turnStrength = 0;

volatile bool forwardPressed = false;
volatile bool backwardPressed = false;
volatile bool leftPressed = false;
volatile bool rightPressed = false;
volatile bool buttonStateChanged = false; // Flag untuk menandakan perubahan status tombol

unsigned long lastUpdateTime = 0;

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
  Serial.println("Motor STOP");
}

void updateTurnStrength() {
  if (millis() - lastUpdateTime > 100) {
    if (leftPressed || rightPressed) {
      if (turnStrength < maxTurnStrength) {
        turnStrength++;
        Serial.print("TurnStrength meningkat: ");
        Serial.println(turnStrength);
      }
    } else {
      if (turnStrength > 0) {
        turnStrength--;
        Serial.print("TurnStrength menurun: ");
        Serial.println(turnStrength);
      }
    }
    lastUpdateTime = millis();
  }
}

void updateMotors() {
  int leftSpeed = baseSpeed;
  int rightSpeed = baseSpeed;
  int reduce = sqrt(turnStrength) * 20;

  if (leftPressed && !rightPressed) {
    leftSpeed = max(baseSpeed - reduce, 0);
    rightSpeed = baseSpeed;
  } else if (rightPressed && !leftPressed) {
    rightSpeed = max(baseSpeed - reduce, 0);
    leftSpeed = baseSpeed;
  }

  if (forwardPressed) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    Serial.println("Gerak MAJU");
  } else if (backwardPressed) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    Serial.println("Gerak MUNDUR");
  } else {
    stopMotors();
    return;
  }

  ledcWrite(ENA, leftSpeed);
  ledcWrite(ENB, rightSpeed);

  Serial.print("Left Speed: ");
  Serial.print(leftSpeed);
  Serial.print(" | Right Speed: ");
  Serial.print(rightSpeed);
  Serial.print(" | TurnStrength: ");
  Serial.println(turnStrength);
}

void handleRoot() {
  String html = R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <title>ESP32 Motor Control</title>
      <style>
        * {
          user-select: none;
          -webkit-user-select: none;
        }
        body {
          font-family: sans-serif;
          text-align: center;
          margin-top: 40px;
        }
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
      <div>
        <button ontouchstart="press('forward')" ontouchend="release('forward')"
                onmousedown="press('forward')" onmouseup="release('forward')">Forward</button><br>
        <button ontouchstart="press('left')" ontouchend="release('left')"
                onmousedown="press('left')" onmouseup="release('left')">Left</button>
        <button onclick="stop()">Stop</button>
        <button ontouchstart="press('right')" ontouchend="release('right')"
                onmousedown="press('right')" onmouseup="release('right')">Right</button><br>
        <button ontouchstart="press('backward')" ontouchend="release('backward')"
                onmousedown="press('backward')" onmouseup="release('backward')">Backward</button>
      </div>

      <script>
        function press(dir) {
          fetch('/press?dir=' + dir);
        }
        function release(dir) {
          fetch('/release?dir=' + dir);
        }
        function stop() {
          fetch('/stop');
        }
      </script>
    </body>
    </html>
  )rawliteral";

  server.send(200, "text/html", html);
}

void handlePress() {
  String dir = server.arg("dir");
  Serial.print("Tombol ditekan: ");
  Serial.println(dir);

  if (dir == "forward") forwardPressed = true;
  else if (dir == "backward") backwardPressed = true;
  else if (dir == "left") leftPressed = true;
  else if (dir == "right") rightPressed = true;

  buttonStateChanged = true;
  server.send(200, "text/plain", "Pressed " + dir);
}

void handleRelease() {
  String dir = server.arg("dir");
  Serial.print("Tombol dilepas: ");
  Serial.println(dir);

  if (dir == "forward") forwardPressed = false;
  else if (dir == "backward") backwardPressed = false;
  else if (dir == "left") leftPressed = false;
  else if (dir == "right") rightPressed = false;

  buttonStateChanged = true;
  server.send(200, "text/plain", "Released " + dir);
}

void handleStop() {
  forwardPressed = backwardPressed = leftPressed = rightPressed = false;
  turnStrength = 0;
  stopMotors();
  buttonStateChanged = true;
  Serial.println("Semua tombol dilepas dan motor dihentikan");
  server.send(200, "text/plain", "Stopped");
}

void setup() {
  Serial.begin(9600);
  Serial.println("Memulai setup...");

  setupMotorPins();
  stopMotors();

  WiFi.softAP(ssid, password);
  Serial.print("Access Point started. IP Address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/press", handlePress);
  server.on("/release", handleRelease);
  server.on("/stop", handleStop);

  server.begin();
  Serial.println("Web server dimulai");
}

void loop() {
  updateTurnStrength();
  if (buttonStateChanged || forwardPressed || backwardPressed || leftPressed || rightPressed) {
    updateMotors();
    buttonStateChanged = false;
  }
  server.handleClient();
}