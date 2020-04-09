import time
from tmp102 import TMP102

# Initialize TMP Object
tmp = TMP102('C', 0x48, 1)

while True:
    print("Current temp: {:.1f} degC".format(tmp.readTemperature()))
    time.sleep(1)