#include <esp_now.h>
#include <WiFi.h>
// Adres slave testowego
uint8_t slave_test_mac[] = { 0xF4, 0x65, 0x0B, 0xE8, 0x4A, 0x7C };
unsigned long czas_start = 0;
bool czekam_na_odpowiedz = false;
// Funkcja odbierajaca odpowiedz
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  // Otrzymanie sygnalu 'R'
  if (len > 0 && data[0] == 'R') {
    if (czekam_na_odpowiedz) {
      unsigned long czas_stop = micros();  // Zatrzymanie stopera (w mikrosekundach)

      // Obliczanie roznicy
      unsigned long rtt = czas_stop - czas_start;

      // Wyswietlanie wyniku
      Serial.print("RTT: ");
      Serial.print(rtt);
      Serial.println(" us");

      czekam_na_odpowiedz = false;  // Gotowy na nastepny test
    }
  }
}
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Blad ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
  // Dodanie slave do peerow
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, slave_test_mac, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Blad dodawania peera");
    return;
  }

  Serial.println("--- START TESTU RTT ---");
  delay(1000);
}
void loop() {
  // Wysylanie probki co sekunde
  if (!czekam_na_odpowiedz) {
    delay(1000);

    // Zapisujemy czas tuz przed wyslaniem
    czas_start = micros();

    // Wysylamy "T"
    esp_err_t wynik = esp_now_send(slave_test_mac, (uint8_t *)"T", 1);

    if (wynik == ESP_OK) {
      czekam_na_odpowiedz = true;
    } else {
      Serial.println("Blad wysylania");
    }
  }

  // Jesli odpowiedz nie przyszla w 1 sekunde, resetujemy
  if (czekam_na_odpowiedz && (micros() - czas_start > 1000000)) {
    Serial.println("Pakiet zginal");
    czekam_na_odpowiedz = false;
  }
}