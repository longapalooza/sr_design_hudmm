def Test():
    for y in range(0, 7):
        # Set the starting page and column
        Set_Start_Page(y)
        Set_Start_Column(0x00)
        for x in range(0, int(128 / 8)):
            for i in range(0, 8):
                pixels = 0x00
                # Since the OLED writes down for each byte, we grab one bit  at a time from each of the
                # eight bytes that make up the page
                for j in range(0, 8):
                    # First determine the coordinate of the byte we wish to read from
                    coordinate = x + ((y * 8) + j) * HRES / 8
                    # Read the byte
                    theByte = pgm_read_byte( & Mono_1BPP()[coordinate])
                    # Determine which bit we need to keep and put it in the correct spot
                    pixels |= (((theByte << i) & 0x80) >> (7 - j))
                    # Send a byte
                writeData(pixels)
        Set_Start_Page(7)
        Set_Start_Column(0x00)
        for x in range(0, 128):
            writeData(0x00)

# Not sure what you are trying to do here - Will
def Mono_1BPP():
    Screen = {Hex}
