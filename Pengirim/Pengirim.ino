#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// NRF24 Setup
RF24 radio(4, 5);  // CE, CSN
const byte address[6] = "00001";

// Buffer input serial
String inputData = "";

void setup() {
  Serial.begin(9600);

  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_2MBPS);
  radio.setChannel(76);
  radio.setAutoAck(false);
  radio.disableCRC();
  radio.openWritingPipe(address);
  radio.stopListening();  // Mode pemancar

  Serial.println("ESP32 NRF24 Transmitter Siap");
}

void loop() {
  if (Serial.available()) {
    inputData = Serial.readStringUntil('\n');
    inputData.trim();  // Hilangkan newline atau spasi

    int separatorIndex = inputData.indexOf(',');
    if (separatorIndex > 0 && separatorIndex < inputData.length() - 1) {
      String kiriStr = inputData.substring(0, separatorIndex);
      String kananStr = inputData.substring(separatorIndex + 1);

      int kiri = kiriStr.toInt();
      int kanan = kananStr.toInt();

      // Clamp nilai agar tetap dalam 0-255
      kiri = constrain(kiri, 0, 255);
      kanan = constrain(kanan, 0, 255);

      byte dataToSend[2] = { (byte)kiri, (byte)kanan };
      bool success = radio.write(&dataToSend, sizeof(dataToSend));
    }
  }
}
