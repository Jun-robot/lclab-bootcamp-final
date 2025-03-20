#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#include "mywifi.h" //gitignoreしてあるので、各自作ってね。
// const char* ssid = "********";
// const char* password = "*******";

const int localPort = 8000;

WiFiUDP udp;

// UART（Serial2）の設定（ESP32のピン配置に合わせて変更）
#define UART_TX_PIN 17
#define UART_RX_PIN 16
#define UART_BAUD   115200  // Picoとの通信速度

const int PACKET_SIZE = 3;  // 角度X, 角度Y, スピードの3バイト

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 UDP -> UART Binary Gateway starting...");

  // STAモードでWiFiに接続
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());
  
  // UDP受信用ポートの開始
  udp.begin(localPort);
  Serial.print("Listening on UDP port ");
  Serial.println(localPort);
  
  // UART（Serial2）の初期化（RX, TXピンを指定して開始）
  Serial2.begin(UART_BAUD, SERIAL_8N1, UART_RX_PIN, UART_TX_PIN);
  Serial.println("UART (Serial2) started.");
}

void loop() {
  // UDPパケットの到着をチェック
  int packetSize = udp.parsePacket();
  if (packetSize >= PACKET_SIZE) { // 最低3バイト受信していれば
    uint8_t buffer[PACKET_SIZE];
    int len = udp.read(buffer, PACKET_SIZE);
    if (len == PACKET_SIZE) {
      Serial.print("Received binary data: ");
      for (int i = 0; i < PACKET_SIZE; i++) {
        Serial.print(buffer[i], DEC);
        Serial.print(" ");
      }
      Serial.println();
      
      // 受信したバイナリデータをそのままUART経由でRaspberry Pi Picoへ転送
      Serial2.write(buffer, PACKET_SIZE);
      Serial.println("Forwarded binary data to Pico via UART.");
    } else {
      Serial.print("Unexpected packet size: ");
      Serial.println(len);
    }
  }
  delay(10);
}
