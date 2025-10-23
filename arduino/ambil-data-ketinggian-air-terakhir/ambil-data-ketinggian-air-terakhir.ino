// #include <WiFi.h>
// #include <HTTPClient.h>
// #include <ArduinoJson.h>

// const char* ssid = "rrj";
// const char* password = "gogo123987";
// const char* serverName = "https://parkir.udnp4mtechno.com/api/getTinggiAirTerakhir.php";

// // LED internal ESP32 biasanya di pin GPIO 2
// #define LED_PIN 2

// void setup() {
//   Serial.begin(115200);
//   pinMode(LED_PIN, OUTPUT);
//   digitalWrite(LED_PIN, LOW);

//   Serial.println("\nMenyambungkan ke WiFi...");
//   WiFi.begin(ssid, password);

//   int retry = 0;
//   while (WiFi.status() != WL_CONNECTED && retry < 20) {
//     delay(500);
//     Serial.print(".");
//     retry++;
//   }

//   if (WiFi.status() == WL_CONNECTED) {
//     Serial.println("\nWiFi tersambung!");
//     Serial.print("IP Address: ");
//     Serial.println(WiFi.localIP());
//   } else {
//     Serial.println("\nGagal tersambung ke WiFi. Cek SSID/Password!");
//   }
// }

// void loop() {
//   if (WiFi.status() == WL_CONNECTED) {
//     HTTPClient http;
//     http.begin(serverName);
//     int httpResponseCode = http.GET();

//     if (httpResponseCode > 0) {
//       String payload = http.getString();

//       // Parsing JSON
//       DynamicJsonDocument doc(1024);
//       DeserializationError error = deserializeJson(doc, payload);

//       if (!error) {
//         int kode = doc["kode"];
//         if (kode == 1) {
//           JsonObject data = doc["data"][0];
//           String tinggi = data["tinggi_air"].as<String>();
//           String status = data["status"].as<String>();
//           String lokasi = data["lokasi_ews"].as<String>();

//           Serial.println("================================");
//           Serial.print("Ketinggian air sekarang : ");
//           Serial.println(tinggi);
//           Serial.print("Status : ");
//           Serial.println(status);
//           Serial.print("Lokasi EWS : ");
//           Serial.println(lokasi);
//           Serial.println("================================");

//           // === Kontrol LED berdasarkan status ===
//           if (status == "AMAN") {
//             digitalWrite(LED_PIN, LOW); // LED mati
//           }
//           else if (status == "SIAGA") {
//             digitalWrite(LED_PIN, HIGH); // LED nyala stabil
//           }
//           else if (status == "WASPADA") {
//             // Blink pelan (nyala 500ms, mati 500ms)
//             for (int i = 0; i < 60; i++) {
//               digitalWrite(LED_PIN, HIGH);
//               delay(1000);
//               digitalWrite(LED_PIN, LOW);
//               delay(1000);
//             }
//           }
//           else if (status == "AWAS") {
//             // Blink cepat (nyala 200ms, mati 200ms)
//             for (int i = 0; i < 60; i++) {
//               digitalWrite(LED_PIN, HIGH);
//               delay(200);
//               digitalWrite(LED_PIN, LOW);
//               delay(200);
//             }

//           }
//         } else {
//           Serial.println("Data tidak tersedia!");
//         }
//       } else {
//         Serial.println("Gagal parsing JSON!");
//       }
//     } else {
//       Serial.print("HTTP Error: ");
//       Serial.println(httpResponseCode);
//     }

//     http.end();
//   } else {
//     Serial.println("WiFi tidak tersambung!");
//   }

//   delay(10000); // ambil data setiap 10 detik
// }


#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const char* ssid = "Mahat";
const char* password = "mahat123459";
const char* serverName = "https://parkir.udnp4mtechno.com/api/getTinggiAirTerakhir.php";

// LED internal ESP32
#define LED_PIN 2

// Inisialisasi LCD (alamat 0x27, ukuran 16x2)
LiquidCrystal_I2C lcd(0x27, 16, 2);

int lampuSiaga = 13;
int lampuWaspada = 12;
int lampuAwas = 14;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(lampuSiaga, OUTPUT);
  pinMode(lampuWaspada, OUTPUT);
  pinMode(lampuAwas, OUTPUT);

  digitalWrite(LED_PIN, LOW);
  digitalWrite(lampuSiaga, LOW);
  digitalWrite(lampuWaspada, LOW);
  digitalWrite(lampuAwas,LOW);

  lcd.init();
  lcd.backlight();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Menyambung WiFi");



  WiFi.begin(ssid, password);
  int retry = 0;
  while (WiFi.status() != WL_CONNECTED && retry < 20) {
    delay(500);
    lcd.setCursor(0, 1);
    lcd.print(".");
    retry++;
  }

  lcd.clear();
  if (WiFi.status() == WL_CONNECTED) {
    lcd.print("WiFi Tersambung");
    delay(1000);
  } else {
    lcd.print("WiFi Gagal!");
    delay(2000);
  }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String payload = http.getString();

      DynamicJsonDocument doc(1024);
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
        int kode = doc["kode"];
        if (kode == 1) {
          JsonObject data = doc["data"][0];
          String tinggi = data["tinggi_air"].as<String>();
          String status = data["status"].as<String>();
          String lokasi = data["lokasi_ews"].as<String>();

          // === Tampilkan ke LCD ===
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("TINGGI: ");
          lcd.print(tinggi);
          lcd.print(" Mtr");

          lcd.setCursor(0, 1);
          lcd.print("STATUS: ");
          lcd.print(status);

          // === Kontrol LED berdasarkan status ===
          if (status == "AMAN") {
            digitalWrite(LED_PIN, LOW);
          } else if (status == "SIAGA") {
            digitalWrite(LED_PIN, HIGH);
            digitalWrite(lampuSiaga, HIGH);
            delay(10000);
            digitalWrite(LED_PIN, LOW);
            digitalWrite(lampuSiaga,LOW);
          } else if (status == "WASPADA") {
            for (int i = 0; i < 10; i++) {
              digitalWrite(LED_PIN, HIGH);
              digitalWrite(lampuWaspada, HIGH);
              delay(1000);
              digitalWrite(LED_PIN, LOW);
              digitalWrite(lampuWaspada, LOW);
              delay(1000);
            }
          } else if (status == "AWAS") {
            for (int i = 0; i < 60; i++) {
              digitalWrite(LED_PIN, HIGH);
              digitalWrite(lampuAwas, HIGH);
              delay(200);
              digitalWrite(LED_PIN, LOW);
              digitalWrite(lampuAwas, LOW);
              delay(200);
            }
          }

        } else {
          lcd.clear();
          lcd.print("Data tidak ada");
          delay(2000);
        }
      } else {
        lcd.clear();
        lcd.print("JSON Error");
        delay(2000);
      }
    } else {
      lcd.clear();
      lcd.print("HTTP Error");
      delay(2000);
    }

    http.end();
  } else {
    lcd.clear();
    lcd.print("WiFi Putus!");
  }

  delay(10000);  // Ambil data setiap 10 detik
}
