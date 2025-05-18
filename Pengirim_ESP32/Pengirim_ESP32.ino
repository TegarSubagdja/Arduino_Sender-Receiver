#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 4
#define CSN_PIN 5

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);

  // SPI pin manual (optional tapi disarankan)
  SPI.begin(18, 19, 23, CSN_PIN);

  if (!radio.begin()) {
    Serial.println("Gagal inisialisasi radio. Periksa wiring atau power.");
    while (1)
      ;
  }

  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_1MBPS);
  radio.setRetries(3, 5);
  radio.openWritingPipe(address);
  radio.stopListening();
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    if (input.length() > 0 && input.length() < 32) {
      char text[input.length() + 1];
      input.toCharArray(text, input.length() + 1);
      radio.write(&text, input.length() + 1);
      Serial.print("Terkirim: ");
      Serial.println(text);
    } else {
      Serial.println("Input terlalu panjang atau kosong.");
    }
  }
}
