import time
import socket
import struct
import numpy as np
from bitalino import BITalino
from plot import plot_emg, plot_emg_realtime, update_plot

# BITalinoの設定
macAddress = "98:D3:11:FE:03:6B"
batteryThreshold = 30
channels = [0, 1, 2]
samplingRate = 100

# UDPの設定
# ESP32_IP = "192.168.0.23"  # jcom
ESP32_IP = "192.168.99.218"
UDP_PORT = 8000  # ESP32側の受信用ポート
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# BITalinoに接続
device = BITalino(macAddress)
device.battery(batteryThreshold)
print(device.version())

# データ取得開始
device.start(samplingRate, channels)

time.sleep(1)

# 移動平均を計算するためのバッファ
emg_buffer = []
buffer_size = 10  # バッファサイズを設定
emg_data = []  # EMGデータを保存するリストを追加
speed_data = []  # Speedデータを保存するリストを追加

# リアルタイムプロットの初期化
fig, ax1, line1, ax2, line2 = plot_emg_realtime(emg_data, speed_data)

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
        emg_data.append(emg)  # EMGデータをリストに追加
        if 0 < abs(emg) < 5.0:
            emg = 0
        emg_buffer.append(abs(emg))
        if len(emg_buffer) > buffer_size:
            emg_buffer.pop(0)
        emg_avg = np.mean(emg_buffer)

        emg_avg = emg_avg*emg_avg
        speed = max(min(emg_avg, 255), 0)
        speed_data.append(speed)  # Speedデータをリストに追加
        
        if len(emg_data) > 100:  # プロットするデータの長さを制限
            emg_data.pop(0)
        if len(speed_data) > 100:  # プロットするデータの長さを制限
            speed_data.pop(0)
        
        update_plot(fig, ax1, line1, emg_data, ax2, line2, speed_data)  # プロットを更新

        # UDPで送信するデータをパック
        UDPangleX = max(min((angle+180)/1.4,255),0)
        UDPangleY = max(min(60,255),0)
        if length < 30:
            UDPspeed = 0
        else:
            UDPspeed = speed
        UDPdata = struct.pack("BBB", int(UDPangleX), UDPangleY,int(UDPspeed))

        # UDPパケット送信
        sock.sendto(UDPdata, (ESP32_IP, UDP_PORT))
        print("Binary command sent:", (int(UDPangleX), UDPangleY, int(UDPspeed)))

        # 0.008秒待つ
        time.sleep(0.008)

except KeyboardInterrupt:
    print("Stopping acquisition")
    #plot.ioff()
    plot_emg(emg_data)  # プログラム終了時にEMGデータを描画

device.stop()
device.close()
sock.close()
