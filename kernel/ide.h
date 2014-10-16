#ifndef _IDE_H_
#define _IDE_H_

#include "stdint.h"

#define IDE_SECTOR_SIZE 512

extern void ide_readSector(int drive, uint32_t sector, uintptr_t buffer);

#endif
