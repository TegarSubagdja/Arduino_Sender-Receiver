#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define ENA 32
#define IN1 33
#define IN2 25
#define ENB 14
#define IN3 26
#define IN4 27

const int pwmFreq = 10000;
const int pwmResolution = 8;
const int motorSpeed = 255;

RF24 radio(4, 5);  // CE, CSN
const byte address[6] = "00001";
const int ledPin = 2;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  ledcAttach(ENA, pwmFreq, pwmResolution);
  ledcAttach(ENB, pwmFreq, pwmResolution);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  radio.begin();
  radio.setPALevel(RF24_PA_LOW);       // Latency lebih rendah di mode LOW
  radio.setDataRate(RF24_2MBPS);       // Kecepatan maksimum
  radio.setChannel(76);                // Channel minim interferensi
  radio.disableCRC();                  // Matikan CRC
  radio.setAutoAck(false);             // Matikan auto-ACK
  radio.openReadingPipe(0, address);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.print("Received: ");
    Serial.println(text);

    // Jalankan motor
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    ledcWrite(ENA, motorSpeed);
    ledcWrite(ENB, motorSpeed);

    // Tahan motor dan LED nyala tanpa blocking lama
    unsigned long t0 = millis();
    digitalWrite(ledPin, HIGH);

    while (millis() - t0 < 200) {
      // Tetap dalam loop, tapi tidak blocking penuh
      // Bisa ditambahkan pengecekan radio.available() di sini kalau mau menerima terus
    }

    digitalWrite(ledPin, LOW);
    stopMotors();
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
