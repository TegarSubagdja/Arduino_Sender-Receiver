#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(4, 5);  // CE, CSN
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);

  pinMode(2, OUTPUT);

  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_2MBPS);
  radio.setRetries(0, 0);
  radio.setChannel(76);
  radio.disableCRC();
  radio.setAutoAck(false);
  radio.openWritingPipe(address);
  radio.stopListening(); // Mode pengiriman
}

void loop() {
  if (Serial.available() >= 2) {
    byte speeds[2];
    speeds[0] = Serial.read(); // Roda kanan
    speeds[1] = Serial.read(); // Roda kiri

    digitalWrite(2, HIGH);
    // Kirim langsung sebagai 2 byte
    radio.write(&speeds, sizeof(speeds));
    digitalWrite(2, LOW);
  }
}
