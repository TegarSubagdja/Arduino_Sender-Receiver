#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(4, 5);  // CE, CSN
const byte address[6] = "00001";

// Motor pins
const int IN1 = 14;  // Motor kanan maju
const int IN2 = 27;  // Motor kanan mundur
const int IN3 = 25;  // Motor kiri maju
const int IN4 = 26;  // Motor kiri mundur

void setup() {
  Serial.begin(9600);

  pinMode(2, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);

  ledcAttach(IN1, 512, 8);  // Kanan maju
  ledcAttach(IN2, 512, 8);  // Kanan mundur
  ledcAttach(IN3, 512, 8);  // Kiri maju
  ledcAttach(IN4, 512, 8);  // Kiri mundur

  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_2MBPS);
  radio.setChannel(76);
  radio.setAutoAck(false);
  radio.disableCRC();
  radio.openReadingPipe(0, address);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    digitalWrite(2, HIGH);

    byte speeds[2];  // [kiri, kanan]
    radio.read(&speeds, sizeof(speeds));

    int kiri = speeds[0];
    int kanan = speeds[1];

    if (kiri == 0 && kanan == 0) {
      // Brake total
      ledcWrite(IN1, 255); ledcWrite(IN2, 255);
      ledcWrite(IN3, 255); ledcWrite(IN4, 255);
    } else {
      // Motor kanan
      ledcWrite(IN1, kanan);
      ledcWrite(IN2, 0);

      // Motor kiri
      ledcWrite(IN3, kiri);
      ledcWrite(IN4, 0);
    }

    digitalWrite(2, LOW);
  }
}
