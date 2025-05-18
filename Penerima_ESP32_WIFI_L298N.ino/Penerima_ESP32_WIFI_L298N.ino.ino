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
const uint8_t maxTurnStrength = 128;
uint8_t turnStrength = 0;

bool forwardPressed = false;
bool backwardPressed = false;
bool leftPressed = false;
bool rightPressed = false;

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
}

void updateTurnStrength() {
  if (millis() - lastUpdateTime > 100) {
    if (leftPressed || rightPressed) {
      if (turnStrength < maxTurnStrength) turnStrength++;
    } else {
      if (turnStrength > 0) turnStrength--;
    }
    lastUpdateTime = millis();
  }
}

void updateMotors() {
  int leftSpeed = baseSpeed;
  int rightSpeed = baseSpeed;
  int reduce = sqrt(turnStrength) * 20;  // Adjust sensitivity here

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
  } else if (backwardPressed) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  } else {
    stopMotors();
    return;
  }

  ledcWrite(ENA, leftSpeed);
  ledcWrite(ENB, rightSpeed);
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
          -webkit-user-select: none;
          -moz-user-select: none;
          -ms-user-select: none;
          user-select: none;
          -webkit-touch-callout: none;
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
          touch-action: manipulation;
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

        // Optional: prevent long-press context menu completely
        document.addEventListener("contextmenu", function(e) {
          e.preventDefault();
        }, false);
      </script>
    </body>
    </html>
  )rawliteral";

  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(9600);
  setupMotorPins();
  stopMotors();

  WiFi.softAP(ssid, password);
  Serial.println("Access Point started");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);

  server.on("/press", []() {
    String dir = server.arg("dir");
    if (dir == "forward") forwardPressed = true;
    else if (dir == "backward") backwardPressed = true;
    else if (dir == "left") leftPressed = true;
    else if (dir == "right") rightPressed = true;
    updateMotors();
    server.send(200, "text/plain", "Pressed " + dir);
  });

  server.on("/release", []() {
    String dir = server.arg("dir");
    if (dir == "forward") forwardPressed = false;
    else if (dir == "backward") backwardPressed = false;
    else if (dir == "left") leftPressed = false;
    else if (dir == "right") rightPressed = false;
    updateMotors();
    server.send(200, "text/plain", "Released " + dir);
  });

  server.on("/stop", []() {
    forwardPressed = backwardPressed = leftPressed = rightPressed = false;
    turnStrength = 0;
    stopMotors();
    server.send(200, "text/plain", "Stopped");
  });

  server.begin();
  Serial.println("Web server started");
}

void loop() {
  updateTurnStrength();
  updateMotors();
  server.handleClient();
}
