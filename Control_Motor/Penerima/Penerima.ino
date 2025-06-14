#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(4, 5);  // CE, CSN
const byte address[6] = "00001";

// Motor kanan
const int IN1 = 14;
const int IN2 = 27;

// Motor kiri
const int IN3 = 26;
const int IN4 = 25;

void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT); // LED indikator

  // PWM setup
  ledcAttach(0, 10000, 8);  // IN1 - Kanan maju
  ledcAttach(1, 10000, 8);  // IN2 - Kanan mundur
  ledcAttach(2, 10000, 8);  // IN3 - Kiri maju
  ledcAttach(3, 10000, 8);  // IN4 - Kiri mundur

  // Bind channel ke pin (otomatis dengan write pertama)
  ledcWrite(0, 0);
  ledcWrite(1, 0);
  ledcWrite(2, 0);
  ledcWrite(3, 0);

  // Radio setup
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_2MBPS);
  radio.setRetries(0, 0);
  radio.setChannel(76);
  radio.disableCRC();
  radio.setAutoAck(false);
  radio.openReadingPipe(1, address);
  radio.startListening();
}

void setMotor(int pwm, int pwmPinA, int pwmPinB, int channelA, int channelB) {
  if (pwm > 0) {
    ledcWrite(channelA, pwm);
    ledcWrite(channelB, 0);
  } else if (pwm < 0) {
    ledcWrite(channelA, 0);
    ledcWrite(channelB, -pwm);
  } else {
    ledcWrite(channelA, 0);
    ledcWrite(channelB, 0); // Stop
  }
}

void loop() {
  byte speeds[2];

  if (radio.available()) {
    digitalWrite(2, HIGH); // LED nyala saat menerima

    radio.read(&speeds, sizeof(speeds));

    int speedR = (int)speeds[0] - 127;
    int speedL = (int)speeds[1] - 127;

    setMotor(speedR, IN1, IN2, 0, 1); // Kanan
    setMotor(speedL, IN3, IN4, 2, 3); // Kiri

    digitalWrite(2, LOW); // LED mati

    // Debug ke serial
    Serial.print("Kanan: ");
    Serial.print(speedR);
    Serial.print(" | Kiri: ");
    Serial.println(speedL);
  }
}
