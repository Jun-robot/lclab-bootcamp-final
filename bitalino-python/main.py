import time
import socket
import struct
import numpy as np
from bitalino import BITalino

# BITalinoの設定
macAddress = "98:D3:11:FE:03:6B"
batteryThreshold = 30
channels = [0, 1, 2]
samplingRate = 100

# UDPの設定
ESP32_IP = "192.168.0.23"  # jcom
UDP_PORT = 8000  # ESP32側の受信用ポート
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# BITalinoに接続
device = BITalino(macAddress)
device.battery(batteryThreshold)
print(device.version())

# データ取得開始
device.start(samplingRate, channels)

time.sleep(1)

try:
    while True:
        nFrames = 10
        data = device.read(nFrames)
        emg = np.mean(data[:, 5])
        joyX = np.mean(data[:, 6])
        joyY = np.mean(data[:, 7])

        # joystick
        joyX = joyX - 256
        joyY = joyY - 256
        if -25 < joyX < 10 and -25 < joyY < 10:
            joyX = 0
            joyY = 0

        # 度数法で角度を求める
        angle = int(np.arctan2(joyY, joyX) * 180 / np.pi)
        #print("Angle: ", angle)
        # 長さを求める(0-255)
        length = int(np.sqrt(joyX**2 + joyY**2)/1.3)
        #print("Length: ", length)  


        # emg -> speed
        emg = emg - 512
        




        # UDPで送信するデータをパック
        UDPangleX = max(min((angle+180)/1.4,255),0)
        UDPangleY = max(min(60,255),0)
        UDPspeed = max(min(length,255),0)
        UDPdata = struct.pack("BBB", int(UDPangleX), UDPangleY,int(UDPspeed))

        # UDPパケット送信
        sock.sendto(UDPdata, (ESP32_IP, UDP_PORT))
        print("Binary command sent:", (UDPangleX, UDPangleY, UDPspeed))

        # 0.008秒待つ
        time.sleep(0.008)

except KeyboardInterrupt:
    print("Stopping acquisition")

device.stop()
device.close()
sock.close()
