"# Arduino_Sender-Receiver
Arduino IDE 2.3.6
RF24 by TMRh20 1.4.11
" 

Wiring ESP32
| NRF24L01 Pin | ESP32 GPIO | Keterangan                                                           |
| ------------ | ---------- | -------------------------------------------------------------------- |
| `GND`        | `GND`      | Ground                                                               |
| `VCC`        | `3.3V`\*   | Tegangan stabil (*gunakan regulator + kapasitor untuk versi PA+LNA*) |
| `CE`         | `GPIO4`    | Chip Enable (CE)                                                     |
| `CSN`        | `GPIO5`    | Chip Select Not (CSN)                                                |
| `SCK`        | `GPIO18`   | SPI Clock                                                            |
| `MOSI`       | `GPIO23`   | SPI Master Out                                                       |
| `MISO`       | `GPIO19`   | SPI Master In                                                        |

Wiring Arduini UNO
| NRF24L01 Pin | Arduino Uno Pin | Keterangan                                                                                  |
| ------------ | --------------- | ------------------------------------------------------------------------------------------- |
| `GND`        | `GND`           | Ground                                                                                      |
| `VCC`        | `3.3V`          | **Tegangan stabil** — disarankan pakai **regulator eksternal** dan **kapasitor 10µF–100µF** |
| `CE`         | `Pin 9`         | Chip Enable (CE)                                                                            |
| `CSN`        | `Pin 10`        | Chip Select Not (CSN)                                                                       |
| `SCK`        | `Pin 13`        | SPI Clock                                                                                   |
| `MOSI`       | `Pin 11`        | SPI Master Out                                                                              |
| `MISO`       | `Pin 12`        | SPI Master In                                                                               |

