#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(4, 5);  // CE, CSN
const byte address[6] = "00001";
const int ledPin = 2;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

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

    // Tahan motor dan LED nyala tanpa blocking lama
    unsigned long t0 = millis();
    digitalWrite(ledPin, HIGH);

    while (millis() - t0 < 200) {
      // Tetap dalam loop, tapi tidak blocking penuh
      // Bisa ditambahkan pengecekan radio.available() di sini kalau mau menerima terus
    }
    digitalWrite(ledPin, LOW);
  }
}
