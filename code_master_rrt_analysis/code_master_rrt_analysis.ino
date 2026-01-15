#include <esp_now.h>
#include <WiFi.h>
// Adres mastera
uint8_t master_mac[] = { 0xF4, 0x65, 0x0B, 0x55, 0x42, 0x74 };
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  // Jesli Master przyslal "T" (Test)
  if (len > 0 && data[0] == 'T') {
    // Natychmiast odsylamy "R"
    esp_now_send(info->src_addr, (uint8_t *)"R", 1);
  }
}
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
  // Dodawanie mastera do peerow
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, master_mac, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  esp_now_add_peer(&peerInfo);
}
void loop() {
}