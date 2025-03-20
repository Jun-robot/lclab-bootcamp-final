#ifndef _H_ESPUDP_
#define _H_ESPUDP_

#include <Arduino.h>

// UART設定
#define UART_RX_PIN 1
#define UART_TX_PIN 0
#define BAUD_RATE 115200
#define PACKET_SIZE 3

// 受信データ格納用構造体
struct ESPData {
	// ...変更前の型などがあればそのまま...
	uint8_t angleX;
	uint8_t angleY;
	uint8_t speed;
};

class espUdp {
public:
	// 初期化関数：UARTを初期化
	void begin();
	// データが受信できた場合、受信値をdataに格納しtrueを返す
	bool readData(ESPData &data);
};

#endif
