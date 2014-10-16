#ifndef _MACHINE_H_
#define _MACHINE_H_

#include "stdint.h"

extern int inb(int port);
extern int inl(int port);
extern void outb(int port, int val);

extern void ltr(uint32_t tr);

extern void pageFaultHandler();

extern uint32_t getcr0();
extern uint32_t getcr3();
extern void invlpg(uint32_t);

#endif
