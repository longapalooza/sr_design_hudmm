#define CLR_CS     (PORTB &= ~(0x01)) //pin #8  - Chip Enable Signal
#define SET_CS     (PORTB |=  (0x01)) //pin #8  - Chip Enable Signal
#define CLR_RESET  (PORTB &= ~(0x02)) //pin #12 - Reset
#define SET_RESET  (PORTB |=  (0x02)) //pin #12 - Reset
#define CLR_DC     (PORTC &= ~(0x01)) //pin #9  - Data/Instruction
#define SET_DC     (PORTC |=  (0x01)) //pin #9  - Data/Instruction
#define CLR_WR     (PORTC &= ~(0x02)) //pin #10 - Write
#define SET_WR     (PORTC |=  (0x02)) //pin #10 - Write
#define CLR_RD     (PORTC &= ~(0x04)) //pin #11 - Read
#define SET_RD     (PORTC |=  (0x04)) //pin #11 - Read

#define XLevelL   0x00
#define XLevelH   0x10
#define XLevel    ((XLevelH&0x0F)*16+XLevelL)
#define HRES      128
#define VRES      56
#define Brightness  0xBF

#define WRITE_RAM writeCommand(0x5C);

void writeCommand(uint8_t command){
  // Select the LCD's command register
  CLR_DC;
  // Select the LCD controller
  CLR_CS;
  //Send the command via SPI:
  SPI.transfer(command);
  //deselect the controller
  SET_CS;
}

void writeData(uint8_t data){
  //Select the LCD's data register
  SET_DC;
  //Select the LCD controller
  CLR_CS;
  //Send the command via SPI:
  SPI.transfer(data);
  // Deselect the LCD controller
  SET_CS;
}

void Set_Start_Column(unsigned char d){
  writeCommand(0x00 + d % 16);  // Set Lower Column Start Address for Page Addressing Mode
                                //   Default => 0x00
  writeCommand(0x10 + d / 16);  // Set Higher Column Start Address for Page Addressing Mode
                                //   Default => 0x10
}

void Set_Column_Address(unsigned char a, unsigned char b){
  writeCommand(0x21);   // Set Column Address
  writeCommand(a);      //   Default => 0x00 (Column Start Address)
  writeCommand(b);      //   Default => 0x7F (Column End Address)
}

void Set_Page_Address(unsigned char a, unsigned char b){
  writeCommand(0x22);   // Set Page Address
  writeCommand(a);      //   Default => 0x00 (Page Start Address)
  writeCommand(b);      //   Default => 0x07 (Page End Address)
}

void Set_Start_Page(unsigned char d){
  writeCommand(0xB0 | d);   // Set Page Start Address for Page Addressing Mode
                            //   Default => 0xB0 (0x00)
}

void Fill_RAM(unsigned char Data){
  unsigned char i, j;
  for (i = 0; i < 8; i++){
    Set_Start_Page(i);
    Set_Start_Column(0x00);
    for (j = 0; j < 128; j++){
      writeData(Data);
    }
  }
}

void OLED_Init(){
  unsigned char i;
  CLR_RESET;
  delay(200);
  SET_RESET;
  delay(200);
  writeCommand(0xFD); // Set Command Lock
  writeCommand(0X12); //  Default => 0x12
                      //     0x12 => Driver IC interface is unlocked from entering command.
                      //     0x16 => All Commands are locked except 0xFD.

  writeCommand(0XAE); // Set Display On/Off
                      //  Default => 0xAE
                      //     0xAE => Display Off
                      //     0xAF => Display On
                            
  writeCommand(0xD5); // Set Display Clock Divide Ratio / Oscillator Frequency
  writeCommand(0XA0); // Set Clock as 116 Frames/Sec
                      //  Default => 0x70
                      //   D[3:0] => Display Clock Divider
                      //   D[7:4] => Oscillator Frequency

  writeCommand(0xA8); // Set Multiplex Ratio
  writeCommand(0X3F); //  Default => 0x3F (1/64 Duty)

  writeCommand(0xD3); // Set Display Offset
  writeCommand(0X00); //  Default => 0x00

  writeCommand(0x40);  // Set Mapping RAM Display Start Line (0x00~0x3F)
                       //  Default => 0x40 (0x00)

  writeCommand(0xD8); // Set Low Power Display Mode (0x04/0x05)
  writeCommand(0x05); //  Default => 0x04 (Normal Power Mode)

  writeCommand(0x20); // Set Page Addressing Mode (0x00/0x01/0x02)
  writeCommand(0x02); //  Default => 0x02
                      //     0x00 => Horizontal Addressing Mode
                      //     0x01 => Vertical Addressing Mode
                      //     0x02 => Page Addressing Mode

  writeCommand(0xA1);  // Set SEG/Column Mapping (0xA0/0xA1)
                       //  Default => 0xA0
                       //     0xA0 => Column Address 0 Mapped to SEG0
                       //     0xA1 => Column Address 0 Mapped to SEG127

  writeCommand(0xC8);  // Set COM/Row Scan Direction (0xC0/0xC8)
                       //  Default => 0xC0
                       //     0xC0 => Scan from COM0 to 63
                       //     0xC8 => Scan from COM63 to 0

  writeCommand(0xDA);  // Set COM Pins Hardware Configuration
  writeCommand(0x12);  //  Default => 0x12
                       //     Alternative COM Pin Configuration
                       //     Disable COM Left/Right Re-Map

  writeCommand(0x81);  // Set SEG Output Current
  writeCommand(0xBF);  // Set Contrast Control for Bank 0
                       //  Default => 0x7F

  writeCommand(0xD9);  // Set Pre-Charge as 2 Clocks & Discharge as 5 Clocks
  writeCommand(0x25);  //  Default => 0x22 (2 Display Clocks [Phase 2] / 2 Display Clocks [Phase 1])
                       //   D[3:0] => Phase 1 Period in 1~15 Display Clocks
                       //   D[7:4] => Phase 2 Period in 1~15 Display Clocks
  
  writeCommand(0xDB);  // Set VCOMH Deselect Level
  writeCommand(0x34);  //  Default => 0x34 (0.78*VCC)

  writeCommand(0xA4);  // Set Entire Display On / Off
                       //  Default => 0xA4
                       //     0xA4 => Normal Display
                       //     0xA5 => Entire Display On

  writeCommand(0xA6);  // Set Inverse Display On/Off
                       //  Default => 0xA6
                       //     0xA6 => Normal Display
                       //     0xA7 => Inverse Display On

  Fill_RAM(0x00);      // Clear Screen
  writeCommand(0XAF);  // Display On (0xAE/0xAF)
}

void setup(){
  DDRD = 0xff;
  DDRC = 0xff;
  DDRB = 0x03;
  PORTD = 0xff;
  SET_RD;
  SET_WR;
  SET_CS;
  SPI.begin();
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
  OLED_Init();
}

void loop(){
  Serial.println("top of loop");
  Fill_RAM(0x00);
  delay(1000);
  Fill_RAM(0x0f);
  delay(1000);
}
