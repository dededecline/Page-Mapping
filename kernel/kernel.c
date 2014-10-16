#include "ide.h"
#include "machine.h"
#include "mmu.h"
#include "gdt.h"
#include "idt.h"
#include "tss.h"
#include "console.h"
#include "vmm.h"

#define ANNOUNCE(code) do { \
    putStr(#code); putStr(" ... "); \
    code; \
    putStr("done\n"); \
} while (0)

#define CHECK(cond) do { \
    int zztop = (cond); \
    putStr(__FILE__); \
    putStr(":"); \
    putDec(__LINE__); \
    putStr(" -- "); \
    putStr("("); \
    putStr(#cond); \
    putStr(") ==> "); \
    if (zztop) { \
        putStr("happy"); \
    } else { \
        putStr("sad"); \
    } \
    putStr("\n"); \
} while (0)

uint32_t peek(uint32_t ptr) {
    return *((volatile uint32_t*)ptr);
}

void poke(uint32_t ptr, uint32_t val) {
    *((volatile uint32_t*)ptr) = val;
}

void kernelMain(void) {
    putStr("\nstarting kernel\n");

    initGdt();
    initTss();
    initIdt();

    putStr("mapping the first 2M with va = pa\n");

    for (uint32_t p = 0; p < 0x200000; p += 4096) {
        vmm_map(p,p);
    }

    CHECK((getcr0() & 0x80000000) == 0);

    putStr("about to enable paging\n");
    vmm_on();

    CHECK((getcr0() & 0x80000000) != 0);

    CHECK (vmm_pa(0xf0000000) == 0xffffffff);

    uint32_t pa = vmm_frame();
    vmm_map(0xf0000000, pa);

    CHECK (vmm_pa(0xf0000000) != 0xffffffff);
    CHECK ((vmm_pa(0xf0000123) & 0xfff) == 0x123);

    CHECK(vmm_dirty(0xf0000000) == 0); 
    CHECK(vmm_accessed(0xf0000000) == 0); 

    CHECK(peek(0xf0000000) == 0);
    CHECK(vmm_dirty(0xf0000000) == 0); 
    CHECK(vmm_accessed(0xf0000000) == 1); 

    poke(0xf0000000, 0x12345678);
    CHECK(peek(0xf0000000) == 0x12345678);
    CHECK(vmm_dirty(0xf0000000) == 1); 
    CHECK(vmm_accessed(0xf0000000) == 1); 

    CHECK(vmm_dirty(0xe0000000) == 0); 
    CHECK(vmm_accessed(0xe0000000) == 0); 

    CHECK(vmm_pa(0x40000000) == 0xffffffff);

    vmm_map(0xe0000000, pa);
    CHECK(peek(0xe0000000) == 0x12345678);

    CHECK(vmm_dirty(0xe0000000) == 0); 
    CHECK(vmm_accessed(0xe0000000) == 1); 

    CHECK(peek(0x44444444) == 0);
    CHECK(vmm_dirty(0x44444000) == 0); 
    CHECK(vmm_accessed(0x44443000) == 0); 
    CHECK(vmm_accessed(0x44444000) == 1); 
    CHECK(vmm_accessed(0x44445000) == 0); 

    poke(0x88888888,0x88888888);
    CHECK(peek(0x88888888) == 0x88888888);

    vmm_map(0xccccc000, vmm_pa(0x88888000));
    CHECK(peek(0xccccc888) == 0x88888888);

    CHECK (vmm_pa(0xccccc666) == vmm_pa(0x88888666));

    vmm_unmap(0xccccc000);
    CHECK(peek(0xccccc888) == 0);
    CHECK(peek(0x88888888) == 0x88888888);

    shutdown();
}
