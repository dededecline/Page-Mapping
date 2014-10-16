#include "vmm.h"
#include "console.h"
#include "stdint.h"
#include "machine.h"

#define LAST(term,leftcut) ((term) & ((1<<(leftcut))-1))
#define MID(term,rightcut,leftcut) LAST((term)>>(rightcut),leftcut)
#define MISSING() do { \
    putStr(__FILE__); \
    putStr(":"); \
    putDec(__LINE__); \
    putStr(" is missing\n"); \
    shutdown(); \
} while (0)

/* Each frame is 4K */
#define FRAME_SIZE (1 << 12)

/* A table contains 4K/4 = 1K page table entries */
#define TABLE_ENTRIES (FRAME_SIZE / sizeof(uint32_t))

/* A table, either a PD, or a PT */
typedef struct {
    uint32_t entries[TABLE_ENTRIES];
} table_s;


/* Address of first avaialble frame */
uint32_t avail = 0x100000;

/* pointer to page directory */
table_s *pd = 0;

/* zero-fill a frame */
static void zeroFrame(uint32_t ptr) {
    char* p = (char*) ptr;
    for (int i=0; i<FRAME_SIZE; i++) {
        p[i] = 0;
    }
}

/* The world's simplest frame allocator */
uint32_t vmm_frame(void) {
    uint32_t p = avail;
    avail += FRAME_SIZE;
    zeroFrame(p);
    return p;
}

/* handle a page fault */
void pageFault(uint32_t addr) {
   sayHex("page fault @ ", addr);
   uint32_t p = vmm_frame();
   vmm_map(addr, p);
}

/* Return a pointer to the PD, allocate it if you have to */
table_s* getPD() {
    if (pd == 0) {
        pd = (table_s*) vmm_frame();
    }
    return pd;            
}

/* Return a pointer to the PT that maps the given va
   allocate it if you have to */
table_s* getPT(uint32_t va) {
    pd = getPD();
    uint32_t idx = va >> 22;
    uint32_t pde = pd->entries[idx];
    if(pde == 0)
    {
        pd->entries[idx] = vmm_frame() | 1;
        return (table_s*) ((uint32_t) pd->entries[idx] & 0xfffff000);
    }
    return (table_s*) ((pd->entries[idx] | 1) & 0xfffff000);
}
//take first ten bits (PD), set entry value

/* Create a new mapping from va to pa */
void vmm_map(uint32_t va, uint32_t pa) {
    /*table_s* pt = getPT(va);
    pt->entries[MID(va, 12, 10)] = (((pa >> 12) << 12) | 1);*/
    table_s* pt = getPT(va);
    uint32_t idx = (va << 10) >> 22;
    pt->entries[idx] = (pa >> 12) << 12;
    pt->entries[idx] |= 1;
}

/* check if the page containing the given PA is dirty */
int vmm_dirty(uint32_t va) {
    table_s* pt = getPT(va);
    uint32_t pte = pt->entries[MID(va, 12, 10)];
    return MID(pte, 6, 1);
}

/* check if the page containing the given PA has been accessed */
int vmm_accessed(uint32_t va) {
    table_s* pt = getPT(va);
    uint32_t pte = pt->entries[MID(va, 12, 10)];
    return MID(pte, 5, 1);
}

/* return the PA that corresponds to the given VA, 0xffffffff is not mapped */
uint32_t vmm_pa(uint32_t va) {
    pd = getPD();
    uint32_t off =  va >> 22;
    uint32_t idx = pd->entries[off];
    if(idx == 0)
    {
        return 0xffffffff;
    }
    table_s *pt = (table_s*)(pd->entries[off] & 0xfffff000);
    off = (va >> 12) & 0x3ff;
    uint32_t pa = pt->entries[off];
    return (pa & 0xfffff000) | (va & 0x3ff);}

extern void invlpg();

/* unmap the given va */ //manipulates p bit and more, see machine.S
void vmm_unmap(uint32_t va) {
    table_s* pd = getPD();
    uint32_t off = va >> 22;
    if((pd->entries[off] & 1) == 1)
    {
        table_s* pt = (table_s*) (pd->entries[off] & 0xfffff000);
        off = (va << 10) >> 22;
        pt->entries[off] = pt->entries[off] & 0xfffffffe;
        invlpg(va);
    }
}

/* print the contents of the page table */
void vmm_dump() {
    table_s *pd = getPD();
    sayHex("PD @ ",(uint32_t) pd);
    for (int i=0; i<TABLE_ENTRIES; i++) {
        uint32_t e = pd->entries[i];
        if (e != 0) {
            putStr("    ");
            putHex(i);
            sayHex(") PDE = ",e);
            table_s * pt = (table_s*) (e & 0xfffff000);
            for (int j=0; j<TABLE_ENTRIES; j++) {
                uint32_t e = pt->entries[j];
                if (e != 0) {
                    putStr("        ");
                    putHex(j);
                    sayHex(") PTE = ",e);
                }
            }
        }
    }
}