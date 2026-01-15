#include <esp_now.h>
#include <WiFi.h>
#define LED 2
#define PRZYCISK 15
bool dioda_swieci = false;
// Adres MAC Mastera
byte master_mac[] = { 0xF4, 0x65, 0x0B, 0x55, 0x42, 0x74 };
// Odbieranie danych
void odebranoDane(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  // Jak przyjdzie "S" to zapalamy
  if (len > 0 && data[0] == 'S') {
    digitalWrite(LED, HIGH);
    dioda_swieci = true;
  }
}
void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  pinMode(PRZYCISK, INPUT_PULLUP);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    return;  // Blad inicjalizacji
  }
  esp_now_register_recv_cb(odebranoDane);
  // Rejestracja Mastera
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, master_mac, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Blad dodawania Mastera");
  }
}
void loop() {
  // Jak dioda swieci i wcisniemy guzik
  if (dioda_swieci && digitalRead(PRZYCISK) == LOW) {

    // Gasimy
    digitalWrite(LED, LOW);
    dioda_swieci = false;
    // Wysylamy info do Mastera
    esp_now_send(master_mac, (uint8_t *)"R", 1);

    // Debouncing
    delay(200);
  }
}