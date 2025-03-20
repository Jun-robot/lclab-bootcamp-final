import socket
import struct

# ESP32のIPアドレス（WiFi接続後のESP32のIPに合わせる）
ESP32_IP = "192.168.0.23"
UDP_PORT = 8000  # ESP32側の受信用ポート

# UDPソケットを作成
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

a = 0
while True:
    # 送信する各値（例として角度X=128, 角度Y=64, スピード=200）
    a += 1
    a = a % 255
    angleX = a  # 0～255
    angleY = 60   # 0～255
    speed  = 20  # 0～255

    # 3バイト（B: unsigned char）としてパックする
    data = struct.pack("BBB", angleX, angleY, speed)

    # UDPパケット送信
    sock.sendto(data, (ESP32_IP, UDP_PORT))
    print("Binary command sent:", (angleX, angleY, speed))
