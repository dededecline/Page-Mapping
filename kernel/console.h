#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include "stdint.h"

void putChar(char c);
void putStr(char* str);
void putDec(uint32_t val);
void putHex(uint32_t val);
void sayHex(char* msg, uint32_t val);
void shutdown(void);
void panic(char* msg);

#endif
