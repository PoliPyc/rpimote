import serial, time, sys

ser = serial.Serial("/dev/ttyUSB0", 115200, timeout=1)
ser.setDTR(False)
time.sleep(1)
ser.flushInput()
ser.setDTR(True)
time.sleep(2)

command = sys.argv[1] 
while True: 
    print('Telling the Arduino to start blinking...')
    data = bytes.fromhex(command)
    print(data)
    print(ser.write(data))

    for i in range(0,1): 
        # read to get the acknowledgement from the Arduino
        ack = []
        counter = 0
        while True:
            ack += ser.read()
            if ack:
                if ack[-1] == ord(b'\n'):
                    break
            counter += 1
            if counter > 160:
                break
        print("".join(map(lambda x: chr(x), ack)))

