%include 'register.s'

section		.data

	mystr:	db	'Flag: %x', 0xA, 0x0

section		.text

	global	main	; main visible
	extern	printf	; call printf

main:

	push dword	ebp
	mov dword	ebp, esp

	mov dword	edx, 0x0

	mov dword	eax, 0x8A23

	mov dword	ebx, 0x8A23
;	mov dword	ebx, 0x0605

	add word	ax, bx

	call		set_flag_cy16		; Set CY flag
	call		set_flag_h16		; Set H flag

	; Set N flag to 0
	mov dword	ebx, 0
	mov dword	ecx, 2
	call		change_flag

	push dword	edx
	push dword	mystr
	call		printf

	mov dword	eax, 0x0

	leave
	ret
