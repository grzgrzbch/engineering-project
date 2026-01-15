#include <esp_now.h>
#include <WiFi.h>
// Adres slave do testu
uint8_t slave_test_mac[] = { 0xF4, 0x65, 0x0B, 0xE8, 0x4A, 0x7C };
// Zmienne do statystyki
int wyslane = 0;
int dostarczone = 0;
int utracone = 0;
bool test_trwa = false;
// Logika odebrania sygnalu zwrotnego
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if (status == ESP_NOW_SEND_SUCCESS) {
    dostarczone++;
  } else {
    utracone++;
  }
}
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Blad ESP-NOW");
    return;
  }
  esp_now_register_send_cb((esp_now_send_cb_t)OnDataSent);
  // Dodawanie slave
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, slave_test_mac, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Blad dodawania peera");
    return;
  }

  Serial.println("--- TEST PER (1000 pakietow) ---");
  Serial.println("Wpisz cokolwiek w Serial Monitor i wcisnij Enter aby zaczac...");
}
void loop() {
  // Uruchomienie testu z Serial Monitora
  if (Serial.available() > 0 && !test_trwa) {
    // Wyczysc bufor
    while (Serial.available()) Serial.read();

    Serial.println("Rozpoczynam test 1000 pakietow...");
    wyslane = 0;
    dostarczone = 0;
    utracone = 0;
    test_trwa = true;

    unsigned long start_testu = millis();
    // Petla wysylania 1000 razy
    for (int i = 0; i < 1000; i++) {
      esp_now_send(slave_test_mac, (uint8_t *)"T", 1);
      wyslane++;

      // Male opoznienie zeby nie zatkac bufora
      delay(10);

      // Co 100 pakietow kropka zeby widziec postep
      if (i % 100 == 0) Serial.print(".");
    }

    unsigned long czas_trwania = millis() - start_testu;
    test_trwa = false;

    // Wyniki
    Serial.println("\n--- WYNIKI TESTU ---");
    Serial.print("Czas trwania: ");
    Serial.print(czas_trwania);
    Serial.println(" ms");
    Serial.print("Wyslane: ");
    Serial.println(wyslane);
    Serial.print("Dostarczone (ACK): ");
    Serial.println(dostarczone);
    Serial.print("Utracone (FAIL): ");
    Serial.println(utracone);

    float per = (float)utracone / wyslane * 100.0;
    Serial.print("PER (Stopa bledow): ");
    Serial.print(per);
    Serial.println("%");
    Serial.println("--------------------");
    Serial.println("Gotowy do kolejnego testu (wpisz cos)...");
  }
}