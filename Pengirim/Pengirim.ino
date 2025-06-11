#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(4, 5); // CE, CSN

const byte address[6] = "00001"; // alamat komunikasi

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);        // Gunakan daya rendah untuk waktu switching lebih cepat
  radio.setDataRate(RF24_2MBPS);        // Kecepatan maksimum
  radio.setRetries(0, 0);               // Tidak ada retry
  radio.setChannel(76);                 // Pilih channel bebas gangguan
  radio.disableCRC();                   // Hilangkan CRC
  radio.setAutoAck(false);              // Matikan auto-ACK
  radio.openWritingPipe(address);
  radio.stopListening();                // Mode TX
}

void loop() {
  const char* text = "hai";
  radio.write(&text, strlen(text) + 1); // kirim pesan kecil
  delay(300); // jeda antar pengiriman
}
