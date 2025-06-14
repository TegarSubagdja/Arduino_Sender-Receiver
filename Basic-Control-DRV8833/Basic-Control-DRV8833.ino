// Deklarasi pin motor
const int IN1 = 14;  // Motor kanan maju
const int IN2 = 27;  // Motor kanan mundur
const int IN3 = 25;  // Motor kiri maju
const int IN4 = 26;  // Motor kiri mundur

const int freq = 512;
const int res = 8;

void setup() {
  // Pastikan semua pin dalam keadaan LOW saat mulai
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  ledcAttach(IN1, freq, res);
  ledcAttach(IN2, freq, res);
  ledcAttach(IN3, freq, res);
  ledcAttach(IN4, freq, res);
}

void loop() {
  // Tes motor jalan maju dengan bertahap
  for (int i = 0; i <= 255; i += 20) {
    ledcWrite(IN1, i);
    ledcWrite(IN2, 0);  // Arah motor kanan maju

    ledcWrite(IN3, i);
    ledcWrite(IN4, 0);  // Arah motor kiri maju

    delay(2000); // jeda untuk lihat perubahan kecepatan
  }

  delay(500);
}
