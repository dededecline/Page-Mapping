#ifndef _TSS_H_
#define _TSS_H_

#include "stdint.h"

extern void initTss(void);

extern void setEsp0(uint32_t esp0);

#endif
