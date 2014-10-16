all : kernel.img

-include ../common.mak

kernel : Makefile $(OFILES)
	ld -N -m elf_i386 -e start -Ttext=0x7c00 -o kernel mbr.o $(filter-out mbr.o,$(OFILES))

clean ::
	rm -f kernel
