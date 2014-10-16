#ifndef _VMM_H_
#define _VMM_H_

#include "stdint.h"

/* allocate a 4K frame */
extern uint32_t vmm_frame(void);

/* enable paging */
extern void vmm_on(void);

/* map va to pa (RW, user & supervisor) */
extern void vmm_map(uint32_t va, uint32_t pa);

/* unmap va */
extern void vmm_unmap(uint32_t va);

/* returns 1 if the page is dirty, 0 otherwise */
extern int vmm_dirty(uint32_t va);

/* returns 1 if the page is accessed, 0 otherwise */
extern int vmm_accessed(uint32_t va);

/* returns the physical address to which va is mapped,
   0xffffffff if not mapped */
extern uint32_t vmm_pa(uint32_t va);

extern void vmm_dump(void);

#endif
