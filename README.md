# C8051 SPI RAM module 

Firmware writes to a 8192-byte RAM module via SPI and prints them to the terminal. Writes a sequential value (0 to 255) to every address, read it back and prints it using `printf_fast_f` to virtual terminal using UART.