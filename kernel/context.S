// context switching functions

// void contextSwitch(long newESP, long* saveOldESP)

	.global contextSwitch
contextSwitch:
	pushf
	push %eax
	push %ebx
	push %ecx
	push %edx
	push %edi
	push %esi
	push %ebp

	mov 40(%esp),%eax
	mov %esp,(%eax)
	mov 36(%esp),%esp

	.global resume
resume:
	pop %ebp
	pop %esi
	pop %edi
	pop %edx
	pop %ecx
	pop %ebx
	pop %eax
	popf

	ret

// void contextResult(esp)

	.global contextResume
contextResume:

	mov 4(%esp),%esp # new ESP
	jmp resume
