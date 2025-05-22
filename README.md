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
| `CE`         | `Pin 8`         | Chip Enable (CE)                                                                            |
| `CSN`        | `Pin 9`        | Chip Select Not (CSN)                                                                       |
| `SCK`        | `Pin 13`        | SPI Clock                                                                                   |
| `MOSI`       | `Pin 11`        | SPI Master Out                                                                              |
| `MISO`       | `Pin 12`        | SPI Master In                                                                               |

Wiring ESP32
| L298N Pin  | ESP32 Pin       | Keterangan                                                                                                       |
| ---------- | --------------- | ---------------------------------------------------------------------------------------------------------------- |
| `GND`      | `GND`           | Ground (harus satu ground dengan ESP32 dan sumber motor)                                                         |
| `VCC`      | —               | **Jangan dihubungkan ke ESP32!** Sambungkan ke sumber daya motor (misal 6–12V)                                   |
| `5V` (out) | —               | Output 5V dari modul L298N (bisa dipakai untuk ESP32 jika modul punya jumper `5V-EN`, tapi **tidak disarankan**) |
| `IN1`      | `GPIO 26`       | Kontrol arah motor A (ubah sesuai kebutuhan)                                                                     |
| `IN2`      | `GPIO 27`       | Kontrol arah motor A                                                                                             |
| `IN3`      | `GPIO 14`       | Kontrol arah motor B                                                                                             |
| `IN4`      | `GPIO 12`       | Kontrol arah motor B                                                                                             |
| `ENA`      | `GPIO 25` (PWM) | Enable motor A (bisa pakai PWM untuk kecepatan)                                                                  |
| `ENB`      | `GPIO 33` (PWM) | Enable motor B (bisa pakai PWM untuk kecepatan)                                                                  |

