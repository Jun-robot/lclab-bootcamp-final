#include <Arduino.h>

// 使用するUARTピン（Picoの配線に合わせて変更してください）
// ここでは例として、RX: GPIO4、TX: GPIO5 を使用
#define UART_RX_PIN 1
#define UART_TX_PIN 0
#define BAUD_RATE 115200  // ESP32側と同じボーレート

// 受信するデータサイズ：角度X、角度Y、スピード 各1バイト
#define PACKET_SIZE 3

void setup() {
  // USBシリアル（デバッグ用）の初期化
  Serial.begin(115200);
  while (!Serial) { delay(10); }  // シリアルポートが開くまで待つ

  // UART (Serial1) の初期化：指定したRX/TXピンで開始
  Serial1.begin(BAUD_RATE);
  Serial.println("Raspberry Pi Pico UART Binary Receiver started");
}

void loop() {
  // UARTで最低3バイトが受信されているか確認
  if (Serial1.available() >= PACKET_SIZE) {
    // 受信バッファから3バイト読み出す
    uint8_t angleX = Serial1.read();
    uint8_t angleY = Serial1.read();
    uint8_t speed  = Serial1.read();

    // デバッグ用に各値を表示
    Serial.print("Received: angleX=");
    Serial.print(angleX);
    Serial.print(", angleY=");
    Serial.print(angleY);
    Serial.print(", speed=");
    Serial.println(speed);

    // TODO: ここに実際の4輪オムニロボットのモータ制御コードを実装してください。
    // 例: setMotorControl(angleX, angleY, speed);
  }
  delay(10); // 適宜調整
}
