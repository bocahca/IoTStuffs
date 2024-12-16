#include <Servo.h> // Library untuk mengontrol motor servo

// Pin tempat sensor proximity dihubungkan
const int sensorProximityPin = D1; // Gunakan pin sesuai dengan koneksi Anda

// Pin untuk sensor ultrasonik
const int trigPin = D2;
const int echoPin = D3;

// Pin untuk motor servo
const int servoPin = D5;

// Objek Servo
Servo myServo;

void setup() {
  // Inisialisasi komunikasi Serial
  Serial.begin(115200);

  // Konfigurasi pin sensor proximity sebagai input
  pinMode(sensorProximityPin, INPUT);

  // Konfigurasi pin untuk sensor ultrasonik
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Inisialisasi motor servo
  myServo.attach(servoPin);
  myServo.write(90); // Posisi netral awal (90 derajat)

  Serial.println("Mulai membaca sensor...");
}

long readUltrasonicDistance() {
  // Kirim sinyal trigger
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Baca durasi pantulan sinyal
  long duration = pulseIn(echoPin, HIGH);

  // Hitung jarak dalam cm (kecepatan suara = 34300 cm/s)
  long distance = duration * 0.034 / 2;
  return distance;
}

void loop() {
  // Membaca nilai dari sensor proximity
  int proximityValue = digitalRead(sensorProximityPin);

  // Membaca jarak dari sensor ultrasonik
  long distance = readUltrasonicDistance();

  // Logik deteksi gabungan
  if (distance < 5) { // Misalnya jarak < 5 cm dianggap ada sampah
    if (proximityValue == LOW) {
      Serial.println("Sampah logam terdeteksi!");
      myServo.write(180); // Gerakkan servo ke kiri (180 derajat)
    } else {
      Serial.println("Sampah non-logam terdeteksi!");
      myServo.write(0); // Gerakkan servo ke kanan (0 derajat)
    }

    delay(2000); // Tunggu selama 2 detik saat servo bergerak
    myServo.write(90); // Kembalikan servo ke posisi netral
    delay(2000); // Tunggu 2 detik sebelum sensor kembali melakukan scan

  } else {
    Serial.print("Jarak: ");
    Serial.print(distance);
    Serial.println(" cm (Tidak ada sampah terdeteksi)");
  }

  // Tunggu sebelum membaca kembali
  delay(2000);
}
