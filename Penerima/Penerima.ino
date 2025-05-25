#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(4, 5);  // CE, CSN

const byte address[6] = "00001";  // alamat komunikasi
const int ledPin = 2;             // GPIO untuk LED (bisa disesuaikan)

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);       // Atur pin LED sebagai output
  digitalWrite(ledPin, LOW);     // Pastikan LED mati di awal

  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(0, address);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.print("Received: ");
    Serial.println(text);

    // Nyalakan LED
    digitalWrite(ledPin, HIGH);
    delay(200); // LED menyala selama 1 detik
    digitalWrite(ledPin, LOW);
  }
}
