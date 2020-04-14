import time
import spidev
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)

XLevelL = 0x00
XLevelH = 0x10
XLevel = (XLevelH&0x0F)*16 + XLevelL
HRES = 128
VRES = 56
Brightness = 0xBF

# needs porting
# PORTB are pins 8-13   
def CLR_CS():
    # PORTB &= ~(0x01)
    # PORTB = PORTB & ~(0x01)
    # PORTB = PORTB and not 0x01
    GPIO.output(8,GPIO.LOW)
    pass

# needs porting
def SET_CS():
    # PORTB |=  (0x01)
    # PORTB = PORTB | 0x01
    # PORTB = PORTB or 0x01
    GPIO.output(8,GPIO.HIGH)
    pass

# needs porting
def CLR_RESET():
    # (PORTB &= ~(0x02))
    GPIO.output(9,GPIO.LOW)
    pass

# needs porting
def SET_RESET():
    # (PORTB |=  (0x02))
    GPIO.output(9,GPIO.HIGH)
    pass

# needs porting
# PORTC are pins A0-A5
def CLR_DC():
    # (PORTC &= ~(0x01))
    #change pin
    GPIO.output(A0,GPIO.LOW)
    pass

# needs porting
def SET_DC():
    # (PORTC |=  (0x01))
    #change pin
    GPIO.output(A0,GPIO.HIGH)
    pass

# needs porting
def CLR_WR():
    # (PORTC &= ~(0x02))
    #change pin
    GPIO.output(A1,GPIO.LOW)
    pass

# needs porting
def SET_WR():
    # (PORTC |=  (0x02))
    #change pin
    GPIO.output(A1, GPIO.HIGH)
    pass

# needs porting
def CLR_RD():
    # (PORTC &= ~(0x04))
    #change pin
    GPIO.output(A2, GPIO.LOW)
    pass

# needs porting
def SET_RD():
    # (PORTC |=  (0x04))
    #change pin
    GPIO.output(A2, GPIO.HIGH)
    pass

def WRITE_RAM():
    writeCommand([0x5C])

def writeCommand(command):
  # Select the LCD's command register
  CLR_DC()
  # Select the LCD controller
  CLR_CS()
  # Send the command via SPI:
  spi.xfer2(command)
  # deselect the controller
  SET_CS()

def writeData(data):
  # Select the LCD's data register
  SET_DC()
  # Select the LCD controller
  CLR_CS()
  # Send the command via SPI:
  spi.xfer2(data)
  # Deselect the LCD controller
  SET_CS()
}

def Set_Start_Column(d):
    writeCommand([0x00 + d % 16])
    writeCommand([0x10 + d / 16])

def Set_Column_Address(a, b):
    writeCommand([0x21, a, b])

def Set_Page_Address(a, b):
    writeCommand([0x22, a, b])

def Set_Start_Page(d):
    writeCommand([0xB0 | d])

def Fill_RAM(Data):
  for i in range(0, 8):
    Set_Start_Page(i)
    Set_Start_Column(0x00)
    for j in range(0, 128):
        writeData([Data])

def OLED_Init():
    CLR_RESET()
    sleep(200)
    SET_RESET()
    sleep(200)
    writeCommand([0xFD, 0X12, 0XAE, 0xD5, 0XA0, 0xA8, 0X3F, 0xD3, 0X00, 0x40])
    writeCommand([0xD8, 0x05, 0x20, 0x02, 0xA1, 0xC8, 0xDA, 0x12, 0x81, 0xBF])
    writeCommand([0xD9, 0x25, 0xDB, 0x34, 0xA4, 0xA6])
    Fill_RAM(0x00)
    writeCommand([0XAF])

# needs porting
DDRD = 0xff # Sets pins 0-7 as output
GPIO.setup(__,GPIO.OUT)
GPIO.setup(__,GPIO.OUT)
GPIO.setup(__,GPIO.OUT)
GPIO.setup(__,GPIO.OUT)
GPIO.setup(__,GPIO.OUT)
GPIO.setup(__,GPIO.OUT)
GPIO.setup(__,GPIO.OUT)
GPIO.setup(__,GPIO.OUT)

# needs porting
DDRC = 0xff # Sets pins A0-A5 as output

GPIO.setup(__,GPIO.OUT)
GPIO.setup(__,GPIO.OUT)
GPIO.setup(__,GPIO.OUT)
GPIO.setup(__,GPIO.OUT)
GPIO.setup(__,GPIO.OUT)
GPIO.setup(__,GPIO.OUT)

# needs porting
DDRB = 0x03 # Sets pins 8 and 9 to outputs and 10-13 as inputs
GPIO.setup(__,GPIO.OUT)
GPIO.setup(__,GPIO.OUT)
GPIO.setup(__,GPIO.OUT)
GPIO.setup(__,GPIO.OUT)
GPIO.setup(__,GPIO.IN)
GPIO.setup(__,GPIO.IN)

# needs porting
PORTD = 0xff # Sets pins 0-7 as high
GPIO.output(__,GPIO.HIGH)
GPIO.output(__,GPIO.HIGH)
GPIO.output(__,GPIO.HIGH)
GPIO.output(__,GPIO.HIGH)
GPIO.output(__,GPIO.HIGH)
GPIO.output(__,GPIO.HIGH)
GPIO.output(__,GPIO.HIGH)
GPIO.output(__,GPIO.HIGH)
GPIO.output(__,GPIO.HIGH)

SET_RD()
SET_WR()
SET_CS()

spi = spidev.SpiDev()
spi.open(0, 1) # bus = 0, device = 1; may need to change device to CS pin - Will Long
spi.max_speed_hz = 8000000
spi.lsbfirst = False # not sure about this one - Will Long
spi.mode = 0b00
OLED_Init()

while True:
    print('top of loop')
    Fill_RAM(0x00)
    sleep(1)
    Fill_RAM(0x0f)
    sleep(1)
