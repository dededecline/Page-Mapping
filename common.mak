CFLAGS = -std=c99 -m32 -nostdlib -nostdinc -g -O3 -Wall -Werror

CFILES = $(wildcard *.c)
SFILES = $(wildcard *.S) $(wildcard *.s)

OFILES = $(subst .c,.o,$(CFILES)) $(subst .s,.o,$(subst .S,.o,$(SFILES)))

# keep all files
.SECONDARY :

%.o :  Makefile %.c
	gcc -c -MD $(CFLAGS) $*.c

%.o :  Makefile %.S
	gcc -MD -m32 -c $*.S

%.o :  Makefile %.s
	gcc -MD -m32 -c $*.s

%.bin : Makefile %
	objcopy -O binary $* $*.bin

%.img : Makefile %.bin
	dd if=$*.bin of=$*.img bs=512 conv=sync

clean ::
	rm -f *.img
	rm -f *.bin
	rm -f *.o
	rm -f *.d

-include *.d
