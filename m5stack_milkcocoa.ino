#include <Milkcocoa.h>
#include <M5Stack.h>
#include <WiFi.h>

#define WIFI_SSID "Wi-Fi のSSID"
#define WIFI_PASSWORD "SSID のパスワード"

#define MILKCOCOA_APP_ID      "Milkcocoa のアプリケーションID"
#define MILKCOCOA_SERVERPORT  1883
#define MILKCOCOA_DATASTORE   "Milkcocoa のデータストア名"

const char MQTT_SERVER[] PROGMEM    = MILKCOCOA_APP_ID ".mlkcca.com";
const char MQTT_CLIENTID[] PROGMEM  = __TIME__ MILKCOCOA_APP_ID;

WiFiClient client;
Milkcocoa milkcocoa = Milkcocoa(&client, MQTT_SERVER, MILKCOCOA_SERVERPORT, MILKCOCOA_APP_ID, MQTT_CLIENTID);

void clearScreen(){
    uint8_t font_buf[20][16];
    uint16_t sj_length;
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.print("========== Milkcocoa subscriber ==========\n\n");
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  delay(500);

  // subscribe
  milkcocoa.on(MILKCOCOA_DATASTORE, "push", on_push);

  M5.begin();
  clearScreen();
}

void loop() {
  milkcocoa.loop();
  milkcocoa.ping();
  delay(5000);
}

void on_push(DataElement *elem) {
  // 登録されたデータを画面に表示する（データ項目名[type, message]は適宜変更してください）
  M5.Lcd.print(elem->getString("type"));
  M5.Lcd.print(": ");
  M5.Lcd.print(elem->getString("message"));
  M5.Lcd.print("\n");
};
