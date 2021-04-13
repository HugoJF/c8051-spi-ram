#include <stdio.h>
#include "config.c"
#include "def_pinos.h"

// RAM module chip-select
#define CS P2_3

// RAM addresses
#define RAM_SIZE 1<<13
#define RAM_WRITE_INSTRUCTION 0x02
#define RAM_READ_INSTRUCTION 0x03

unsigned char read_ram(unsigned short address) {
    unsigned char address_low = address;
    unsigned char address_high = address >> 8;

    // execute byte read sequence (specified in the datasheet)
    CS = 0; // enable RAM
    SPI0DAT = RAM_READ_INSTRUCTION; // send read instruction
    while (!TXBMT); // wait SPI0 buffer to get empty
    SPI0DAT = address_high;
    while (!TXBMT);
    SPI0DAT = address_low;
    while (!TXBMT);
    SPI0DAT = 0x00; // write 8 bits (either 1s or 0s) during which data will be received
    while (!TXBMT);
    SPIF = 0; // reset SPI0 interrupt flag
    while (!SPIF); // wait for end of transmission interrupt
    SPIF = 0; // reset it again
    CS = 1; // disable ram

    return SPI0DAT; // return the data sent by the RAM
}

void write_ram(unsigned int address, unsigned short data) {
    unsigned char address_low = address;
    unsigned char address_high = address >> 8;

    // execute byte write sequence (specified in the datasheet)
    CS = 0; // enable RAM
    SPI0DAT = RAM_WRITE_INSTRUCTION; // send write instruction
    while (!TXBMT); // wait SPI0 buffer to get empty
    SPI0DAT = address_high;
    while (!TXBMT);
    SPI0DAT = address_low;
    while (!TXBMT);
    SPI0DAT = data;
    while (!TXBMT);
    SPIF = 0; // reset SPI0 interrupt flag
    while (!SPIF); // wait for end of transmission interrupt
    SPIF = 0; // reset it again
    CS = 1; // disable ram
}

// handles printf
void putchar (unsigned char c) {
    SBUF0 = c;
    while (TI0 == 0);
    TI0 = 0;
}

void main (void) {
    unsigned int i = 0;

    Init_Device();
    SFRPAGE = LEGACY_PAGE;

    for (i = 0; i < RAM_SIZE; ++i)
    {
        write_ram(i, (unsigned char) i);
        printf_fast_f("[%d] = %d\n", i, read_ram(i));
    }
}