#define ASSEMBLY
#include "gdt.h"

	# outb(int port, int val)
	.global outb
outb:
	push %edx
	mov 8(%esp),%dx
	mov 12(%esp),%al
	outb %al,%dx
	pop %edx
	ret

	# int inb(int port)
	.global inb
inb:
	push %edx
	mov 8(%esp),%dx
	inb %dx,%al
	pop %edx
	and $0xff,%eax
	ret

	# unsigned long inb(int port)
	.global inl
inl:
	push %edx
	mov 8(%esp),%dx
	inl %dx,%eax
	pop %edx
	ret


	#
	# void ltr(uint32_t tr)
	#
	.global ltr
ltr:
	mov 4(%esp),%eax
	ltr %ax
	ret

	.extern pd
	.global vmm_on
vmm_on:
	mov pd,%eax
	mov %eax,%cr3

	mov %cr0,%eax
	or $0x80000000,%eax
	mov %eax,%cr0
	ret
	
	.global getcr0
getcr0:
	mov %cr0,%eax
	ret
	
	.global getcr3
getcr3:
	mov %cr3,%eax
	ret

	.global pageFaultHandler
pageFaultHandler:

	pushf
	push %ds
	push %eax
	push %ebx
	push %ecx
	push %edx
	push %esi
	push %edi
	push %ebp

	mov $(GDT_KERNEL_DATA << 3),%eax
	movw %ax,%ds

	mov %cr2,%eax
	push %eax

	.extern pageFault
	call pageFault

	add $4,%esp    /* pop argument */

	pop %ebp
	pop %edi
	pop %esi
	pop %edx
	pop %ecx
	pop %ebx
	pop %eax
	pop %ds
	popf
	add $4,%esp   /* pop error */
	iret



	.global invlpg
invlpg:
	mov 4(%esp),%eax
	invlpg (%eax)
	ret

