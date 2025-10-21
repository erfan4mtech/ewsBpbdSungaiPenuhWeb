#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "rrj";
const char* password = "gogo123987";
const char* serverName = "https://parkir.udnp4mtechno.com/api/getTinggiAirTerakhir.php";

// LED internal ESP32 biasanya di pin GPIO 2
#define LED_PIN 2

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

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
    Serial.println("\nGagal tersambung ke WiFi. Cek SSID/Password!");
  }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String payload = http.getString();

      // Parsing JSON
      DynamicJsonDocument doc(1024);
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
        int kode = doc["kode"];
        if (kode == 1) {
          JsonObject data = doc["data"][0];
          String tinggi = data["tinggi_air"].as<String>();
          String status = data["status"].as<String>();
          String lokasi = data["lokasi_ews"].as<String>();

          Serial.println("================================");
          Serial.print("Ketinggian air sekarang : ");
          Serial.println(tinggi);
          Serial.print("Status : ");
          Serial.println(status);
          Serial.print("Lokasi EWS : ");
          Serial.println(lokasi);
          Serial.println("================================");

          // === Kontrol LED berdasarkan status ===
          if (status == "AMAN") {
            digitalWrite(LED_PIN, LOW); // LED mati
          }
          else if (status == "SIAGA") {
            digitalWrite(LED_PIN, HIGH); // LED nyala stabil
          }
          else if (status == "WASPADA") {
            // Blink pelan (nyala 500ms, mati 500ms)
            for (int i = 0; i < 60; i++) {
              digitalWrite(LED_PIN, HIGH);
              delay(1000);
              digitalWrite(LED_PIN, LOW);
              delay(1000);
            }
          }
          else if (status == "AWAS") {
            // Blink cepat (nyala 200ms, mati 200ms)
            for (int i = 0; i < 60; i++) {
              digitalWrite(LED_PIN, HIGH);
              delay(200);
              digitalWrite(LED_PIN, LOW);
              delay(200);
            }
            
          }
        } else {
          Serial.println("Data tidak tersedia!");
        }
      } else {
        Serial.println("Gagal parsing JSON!");
      }
    } else {
      Serial.print("HTTP Error: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi tidak tersambung!");
  }

  delay(10000); // ambil data setiap 10 detik
}
