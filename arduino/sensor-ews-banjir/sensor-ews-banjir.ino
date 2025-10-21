#include <WiFi.h>
#include <HTTPClient.h>
#include <NewPing.h>

// ================== KONFIGURASI ==================
const char* ssid = "rrj";              
const char* password = "gogo123987";   
const char* serverName = "https://parkir.udnp4mtechno.com/api/inputTinggiAir.php"; // API endpoint

// Lokasi sensor
String lokasi_ews = "Batang Merao 1";
String status;

// Ultrasonic
#define TRIG_PIN  5
#define ECHO_PIN  18
#define MAX_DISTANCE 300
NewPing sensor(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

int kedalamanTotal = 400; // cm

// ================== SETUP ==================
void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\nMenyambungkan ke WiFi...");
  WiFi.begin(ssid, password);

  int retry = 0;
  while (WiFi.status() != WL_CONNECTED && retry < 20) {
    delay(500);
    Serial.print(".");
    retry++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi tersambung!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nGagal tersambung ke WiFi. Cek SSID/Password.");
  }
}

// ================== LOOP ==================
void loop() {
  // ======= Baca sensor ultrasonic =======
  int jarak = sensor.ping_cm();
  int tinggiAir = kedalamanTotal - jarak;
  if (tinggiAir < 0) tinggiAir = 0;
  float meter = tinggiAir / 100.0;

  // ======= Tentukan status =======
  if (meter < 1.0) {
    status = "AMAN";
  } else if (meter >= 1.0 && meter < 1.5) {
    status = "SIAGA";
  } else if (meter >= 1.5 && meter < 3.0) {
    status = "WASPADA";
  } else {
    status = "AWAS";
  }

  // ======= Debug ke Serial =======
  Serial.print("Tinggi Air = ");
  Serial.print(meter, 2);
  Serial.println(" Meter");
  Serial.print("Status = ");
  Serial.println(status);

  // ======= Kirim data ke server =======
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // ✅ Perbaikan di sini (String(meter,2) yang benar)
    String postData = "tinggi_air=" + String(meter, 2) +
                      "&lokasi_ews=" + lokasi_ews +
                      "&status=" + status;

    http.begin(serverName);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    int httpResponseCode = http.POST(postData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Response: " + response);
    } else {
      Serial.println("Gagal kirim data! Error code: " + String(httpResponseCode));
    }

    http.end();
  } else {
    Serial.println("WiFi terputus, mencoba reconnect...");
    WiFi.disconnect(true);
    delay(1000);
    WiFi.begin(ssid, password);

    int retry = 0;
    while (WiFi.status() != WL_CONNECTED && retry < 20) {
      delay(500);
      Serial.print(".");
      retry++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nReconnected!");
    } else {
      Serial.println("\nGagal reconnect WiFi.");
    }
  }

  delay(5000); // jeda 5 detik sebelum pembacaan berikutnya
}
