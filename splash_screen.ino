void showSplash(){
   uint8_t pixels;
   uint8_t theByte;
   uint16_t coordinate;
   for (uint8_t y = 0; y < 7; y++){
     // Set the starting page and column
     Set_Start_Page(y);
     Set_Start_Column(0x00);
     for (uint8_t x = 0; x < 128 / 8; x++){
       for (uint8_t i = 0; i < 8; i++){
         pixels = 0x00;
         // Since the OLED writes down for each byte, we grab one bit at a time from each of the
         // eight bytes that make up the page
         for (int j = 0; j < 8; j++){
           // First determine the coordinate of the byte we wish to read from
           coordinate = x + ((y * 8) + j) * HRES / 8;
           // Read the byte
           theByte = pgm_read_byte(&Mono_1BPP[coordinate]);
           // Determine which bit we need to keep and put it in the correct spot
           pixels |= (((theByte << i) & 0x80) >> (7 - j));
         }
         // Send a byte
         writeData(pixels);
       }
     }
   }
   Set_Start_Page(7);
   Set_Start_Column(0x00);
   for (uint8_t x = 0; x < 128; x++){
     writeData(0x00);
   }
}
