import time
from bitalino import BITalino
import numpy as np

macAddress = "98:D3:11:FE:03:6B"

batteryThreshold = 30
channels = [0,1,2]
samplingRate = 100

# Connect to BITalino
device = BITalino(macAddress)
device.battery(batteryThreshold)
print(device.version())

# Start Acquisition
device.start(samplingRate, channels)

time.sleep(1)

try:
    while True:
        nFrames = 10
        data = device.read(nFrames)
        emg = np.mean(data[:, 5])
        joyX = np.mean(data[:, 6])
        joyY = np.mean(data[:, 7])
        #print("EMG: ", emg, "JoyX: ", joyX, "JoyY: ", joyY)

        #joystick
        joyX = joyX - 256
        joyY = joyY - 256
        if -25 < joyX < 10 and -25 < joyY < 10:
            joyX = 0
            joyY = 0
        #print("JoyX: ", joyX, "JoyY: ", joyY)
        #度数法で角度を求める
        angle = int(np.arctan2(joyY, joyX) * 180 / np.pi)
        print("Angle: ", angle)

        

except KeyboardInterrupt:
    print("Stopping acquisition")

device.stop()
device.close()