#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN

const byte address[6] = "00001"; // alamat komunikasi

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setPALevel(RF24_PA_MAX); // daya maksimum
  radio.setDataRate(RF24_2MBPS); // kecepatan maksimum
  radio.setRetries(3, 5); // retry count dan delay
  radio.openWritingPipe(address);
  radio.stopListening(); // mode kirim
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n'); // baca sampai newline
    input.trim(); // hapus spasi jika ada
    if (input.length() > 0 && input.length() < 32) { // panjang maksimal data NRF24
      char text[input.length() + 1];
      input.toCharArray(text, input.length() + 1); // ubah ke char[]
      radio.write(&text, input.length() + 1); // +1 karena null-terminator
      Serial.print("Terkirim: ");
      Serial.println(text);
    } else {
      Serial.println("Input terlalu panjang atau kosong.");
    }
    delay(500);
  }
}