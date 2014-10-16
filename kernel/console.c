#include "u8250.h"
#include "console.h"

static char hexChars[] = {
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
};

void putChar(char c) {
    u8250_putChar(c);
}

void putStr(char* str) {
    char c;

    if (str == 0) return;

    while ((c = *str) != 0) {
        putChar(c);
        str ++;
    }
}

void putHex(uint32_t v) {
    putChar('0');
    putChar('x');
    long shift = 28;

    for (int i=0; i<8; i++) {
        long x = (v >> shift) & 0xf;
        putChar(hexChars[x]);
        shift = shift - 4;
    }
}

void putDec(uint32_t v) {
    char digits[10];
    int n = 0;

    while (v) {
        digits[n++] = hexChars[v % 10];
        v = v / 10;
    }

    if (n == 0) {
        putChar('0');
    } else {
        for (int i=n-1; i>=0; i--) {
            putChar(digits[i]);
        }
    }
}


void sayHex(char* msg, uint32_t v) {
    putStr(msg);
    putHex(v);
    putStr("\n");
}

void shutdown() {
    putStr("*** System Shutdown ***\n");
    while (1) { __asm __volatile ( "hlt" ); }
}

void panic(char *msg) {
    putStr(msg);
    putStr("\n");
    shutdown();
}
