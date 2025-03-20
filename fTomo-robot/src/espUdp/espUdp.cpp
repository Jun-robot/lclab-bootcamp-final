#include "espUdp.h"

// ...その他の既存コード...

void espUdp::begin() {
	// UART初期化：指定されたRX/TXピンで初期化
	Serial1.begin(BAUD_RATE);
	// ...既存の初期化コードがあればそのまま...
}

bool espUdp::readData(ESPData &data) {
	// UARTで受信可能なデータがPACKET_SIZE以上ある場合
	if (Serial1.available() >= PACKET_SIZE) {
		data.angleX = Serial1.read();
		data.angleY = Serial1.read();
		data.speed  = Serial1.read();
		return true;
	}
	return false;
}
