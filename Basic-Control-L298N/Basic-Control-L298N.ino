// Motor A
#define ENA 32
#define IN1 33
#define IN2 25

// Motor B
#define ENB 14
#define IN3 26
#define IN4 27

// PWM settings
const int pwmFreq = 5000;
const int pwmResolution = 8;
const int motorSpeed = 200; // 0 - 255

void setup() {
  Serial.begin(9600);

  ledcAttach(ENA, pwmFreq, pwmResolution);
  ledcAttach(ENB, pwmFreq, pwmResolution);

  // Motor pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();

    if (command == '1') {
      // Maju
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      ledcWrite(ENA, motorSpeed);
      ledcWrite(ENB, motorSpeed);
      delay(500);
      stopMotors();
      Serial.print("maju");
    }

    if (command == '2') {
      // Mundur
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      ledcWrite(ENA, motorSpeed);
      ledcWrite(ENB, motorSpeed);
      delay(500);
      stopMotors();
      Serial.print("mundur");
    }
  }
}

void stopMotors() {
  ledcWrite(ENA, 0);
  ledcWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
