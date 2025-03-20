# # BITalinoライブラリのインポート
# from bitalino import BITalino
# import time
# import numpy as np

# # デバイスのMACアドレスを指定
# macAddress = "98:D3:11:FE:03:6B"  # MACアドレスを正しい値に置き換える
# device = None  # デバイス初期化

# # サンプリングレートや取得設定
# samplingRate = 1000  # サンプリングレート (Hz)
# acqChannels = [0]  # 取得チャネル（A1）
# nSamples = 100  # 取得サンプル数
# running_time = 10  # データ取得の実行時間（秒）

# #筋電位用
# BITS = 10 # 信号のビット幅
# VCC = 3.3 # 電源電圧
# GAIN = 1009 # センサーゲイン

# try:
#     # デバイスの取得
#     device = BITalino(macAddress)
#     print("デバイスバージョン:", device.version())  # バージョンの表示

#     # データ取得の開始
#     device.start(samplingRate, acqChannels)
#     print("リアルタイムデータ取得を開始します... (Ctrl+Cで終了)")

#     start_time = time.time()
#     while True:
#         # データをリアルタイムで取得
#         data = device.read(nSamples)
#         sixth_column = data[:, 5]
#         print(sixth_column[:1])
        
#         #筋電位用
#         # emg = (((((sixth_column[:1] / 2**BITS) - 1/2) * VCC) / GAIN) * 1000) # 単位変換（mV）
#         # emg = np.abs(emg) # 絶対値 
#         # print(emg)
#         # print("Average:" + str(np.average(emg)))
#         # print("Standard Deviation:" + str(np.std(emg)))
#         # print("Maximum:" + str(np.max(emg)))
        
        
#         # print("取得データ:", sixth_column[:1])  # 最初の1行だけ表示（必要に応じて変更）
        
#         # 指定時間を超えたら終了
#         if time.time() - start_time > running_time:
#             break



# except KeyboardInterrupt:
#     print("\nユーザーが停止しました (Ctrl+C)。")
# except Exception as e:
#     print("エラーが発生しました:", e)
# finally:
#     if device:
#         # データ取得の停止とデバイスの切断
#         device.stop()
#         device.close()
#         print("デバイスを正常に終了しました。")
#     else:
#         print("デバイスの初期化に失敗しました。")
import time

from bitalino import BITalino

# The macAddress variable on Windows can be "XX:XX:XX:XX:XX:XX" or "COMX"
# while on Mac OS can be "/dev/tty.BITalino-XX-XX-DevB" for devices ending with the last 4 digits of the MAC address or "/dev/tty.BITalino-DevB" for the remaining
macAddress = "98:D3:11:FE:03:6B"

# This example will collect data for 5 sec.
running_time = 5

batteryThreshold = 30
# acqChannels = [0, 1, 2, 3, 4, 5]
acqChannels = [0,1,2]
samplingRate = 1000
nSamples = 10
digitalOutput_on = [1, 1]
digitalOutput_off = [0, 0]

# Connect to BITalino
device = BITalino(macAddress)
# Set battery threshold
device.battery(batteryThreshold)
# Read BITalino version
print(device.version())

# Start Acquisition
device.start(samplingRate, acqChannels)

start = time.time()
end = time.time()
while (end - start) < running_time:
    # Read samples
    print(device.read(nSamples))
    end = time.time()

# Turn BITalino led and buzzer on
device.trigger(digitalOutput_on)

# Script sleeps for n seconds
time.sleep(running_time)

# Turn BITalino led and buzzer off
device.trigger(digitalOutput_off)

# Stop acquisition
device.stop()

# Close connection
device.close()