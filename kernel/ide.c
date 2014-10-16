#include "kernel.h"
#include "ide.h"
#include "machine.h"
#include "stdint.h"

// The drive number encodes the controller in bit 1 and the channel in bit 0

static inline int controller(int drive) {
    return (drive >> 1) & 1;
}

static inline int channel(int drive) {
    return drive & 1;
}

// the base I/O port for each controller

static int ports[2] = { 0x1f0, 0x170 };

static inline int port(int drive) {
    return ports[controller(drive)];
}

//////////////////
// drive status //
//////////////////

static inline long getStatus(int drive) {
    return inb(port(drive) + 7);
}

// Status bits
#define BSY	0x80
#define DRDY	0x40
    
static inline int isBusy(int drive) {
    return getStatus(drive) & BSY;
}

static inline int isReady(int drive) {
    return getStatus(drive) & DRDY;
}

static inline void waitForDrive(int drive) {
    while (isBusy(drive));
    while (!isReady(drive));
}
   

void ide_readSector(int drive, uint32_t sector, uintptr_t ptr) {
    uint32_t* buffer = (uint32_t*) ptr;
    int base = port(drive);
    int ch = channel(drive);

    waitForDrive(drive);

    outb(base + 2, 1);			// sector count
    outb(base + 3, sector >> 0);	// bits 7 .. 0
    outb(base + 4, sector >> 8);	// bits 15 .. 8
    outb(base + 5, sector >> 16);	// bits 23 .. 16
    outb(base + 6, 0xE0 | (ch << 4) | ((sector >> 24) & 0xf));
    outb(base + 7, 0x20);		// read with retry

    waitForDrive(drive);

    for (int i=0; i<IDE_SECTOR_SIZE/sizeof(uint32_t); i++) {
        buffer[i] = inl(base);
    }

}
