#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 4
#define CSN_PIN 5

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00002";

void setup() {
  Serial.begin(9600);

  // Inisialisasi SPI khusus untuk ESP32
  SPI.begin(18, 19, 23, CSN_PIN);

  if (!radio.begin()) {
    Serial.println("Gagal inisialisasi radio. Periksa wiring atau power.");
    while (1);
  }

  radio.setPALevel(RF24_PA_MAX);     // Gunakan power maksimum untuk PA+LNA
  radio.setDataRate(RF24_1MBPS);     // Cocokkan dengan pengirim
  radio.openReadingPipe(1, address); // Pipe 1 bisa digunakan bebas
  radio.startListening();            // Masuk ke mode menerima
  Serial.println("Mode penerima siap...");
}

void loop() {
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.print("Diterima: ");
    Serial.println(text);
  }
}
