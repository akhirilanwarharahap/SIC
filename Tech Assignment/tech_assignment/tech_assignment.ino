#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

// Konfigurasi WiFi
const char* ssid = "Infinix";
const char* password = "112233445";

// URL server lokal
const char* serverName = "http://192.168.25.235:5000/data"; // Ganti dengan alamat IP server lokal Anda

// Konfigurasi DHT
#define DHTPIN 4        // Pin DHT22 terhubung ke GPIO4
#define DHTTYPE DHT22   // DHT 22 (AM2302)
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Memulai Serial Monitor
  Serial.begin(115200);
  
  // Menghubungkan ke WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Menghubungkan ke WiFi...");
  }
  Serial.println("Terhubung ke WiFi");

  // Memulai sensor DHT
  dht.begin();
}

void loop() {
  // Mengambil data dari sensor DHT22
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Memeriksa apakah pembacaan berhasil
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Gagal membaca dari sensor DHT");
    return;
  }

  // Mencetak data ke Serial Monitor
  Serial.print("Kelembaban: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Suhu: ");
  Serial.print(temperature);
  Serial.println(" *C");

  // Mengirim data ke server melalui HTTP POST
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    String httpRequestData = "{\"temperature\":" + String(temperature) + ",\"humidity\":" + String(humidity) + "}";
    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error pada pengiriman POST: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("Terputus dari WiFi");
  }

  // Menunggu sebelum membaca lagi
  delay(10000);
}
