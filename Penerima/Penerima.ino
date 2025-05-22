#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(4, 5);  // CE, CSN

const byte address[6] = "00001";  // alamat komunikasi

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);  // speed tinggi
  radio.openReadingPipe(0, address);
  radio.startListening();  // mode terima
}

void loop() {
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.print("Received: ");
    Serial.println(text);
  }
}
