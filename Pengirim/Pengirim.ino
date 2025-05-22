#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(8, 9); // CE, CSN

const byte address[6] = "00001"; // alamat komunikasi

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setPALevel(RF24_PA_LOW); // daya maksimum
  radio.setDataRate(RF24_250KBPS); // kecepatan maksimum
  radio.setRetries(3, 5); // retry count dan delay
  radio.openWritingPipe(address);
  radio.stopListening(); // mode kirim
}

void loop() {
  const char* text = "hai";
  radio.write(&text, strlen(text) + 1); // +1 untuk null-terminator
  Serial.print("Terkirim: ");
  Serial.println(text);
  delay(1000); // delay 1 detik biar tidak terlalu cepat
}
