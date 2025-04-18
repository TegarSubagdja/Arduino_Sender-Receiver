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
  const char text[] = "Hello NRF";
  bool report = radio.write(&text, sizeof(text));
  
  if (report) {
    Serial.println("Data sent successfully");
  } else {
    Serial.println("Failed to send");
  }

  delay(500); // delay 0.5 detik, sesuaikan kebutuhan
}
