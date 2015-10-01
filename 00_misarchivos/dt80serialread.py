#!/usr/bin/python
import serial, time
#initialization and open the port
#possible timeout values:
#    1. None: wait forever, block call
#    2. 0: non-blocking mode, return immediately
#    3. x, x is bigger than 0, float allowed, timeout block call
ser = serial.Serial()
ser.port = "/dev/ttyS1"
#ser.port = "/dev/ttyS2"
ser.baudrate = 9600
ser.bytesize = serial.EIGHTBITS #number of bits per bytes
ser.parity = serial.PARITY_NONE #set parity check: no parity
ser.stopbits = serial.STOPBITS_ONE #number of stop bits
#ser.timeout = None          #block read
ser.timeout = 3             #non-block read
#ser.timeout = 2              #timeout block read
ser.xonxoff = False     #disable software flow control
ser.rtscts = False     #disable hardware (RTS/CTS) flow control
ser.dsrdtr = False       #disable hardware (DSR/DTR) flow control
ser.writeTimeout = 2     #timeout for write

try:
    ser.open()

except Exception, e:
    print "error open serial port: " + str(e)
    exit()

if ser.isOpen():
    try:
        ser.flushInput() #flush input buffer, discarding all its contents
        ser.flushOutput()#flush output buffer, aborting current output
                     #and discard all that is in buffer
        #write data
        ser.write("XB\r\n")
        #print("write data: XB")
        time.sleep(0.5)  #give the serial port sometime to receive the data
        numOfLines = 0

        while True:
            response = ser.readline()
            #print("read data: " + response)
            print (response)
            numOfLines = numOfLines + 1
            if (numOfLines >= 1):
                break
        ser.close()

    except Exception, e1:
        print "error communicating...: " + str(e1)

else:
    print "cannot open serial port "
