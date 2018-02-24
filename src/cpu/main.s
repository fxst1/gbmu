%ifdef aTEST
%include 'src/cpu/instruction.s'

section		.data

	buffer:			db	00001000b, 0xF8, 0xFF

	register_str:	db	'A : %p (%hhx) | F : %p (%hhx)', 0xA, 'B : %p (%hhx) | C : %p (%hhx)', 0xA, 'D : %p (%hhx) | E : %p (%hhx)', 0xA, 'H : %p (%hhx) | L : %p (%hhx)', 0xA, 'PC : %p (%hx)', 0xA,'SP : %p (%hx)', 0xA, 'Flags : %c %c %c %c', 0xA, 0xA, 0x0
	flags_str:		db	'flags %c %c %c %c', 0xA, 0
	dbgFormat:		db	'Buffer: %hhx', 0xA, 'ECX: %d | EDX : %c (%hhx)', 0xA, 0

	tmp_str:		db	'%hhx @%p', 0xA, 0

	error_str:		db	'= ERROR =', 0xA, 0

section		.text

	global	main	; main visible
	extern	printf	; call printf

main:

	push dword	ebp
	mov dword	ebp, esp

	; Create Reserve space
	sub dword	esp, 0x10008			; Reserve space
	mov dword	edi, esp				; Save start of RSSP in edi

	call		init

	mov byte	[edi], 0xFF
	mov byte	[edi + 0x1], 0xEE
	mov byte	[edi + 0x2], 0x2
	mov byte	[edi + 0x3], 0x3
	mov byte	[edi + 0x4], 0x4
	mov byte	[edi + 0x5], 0x5

	mov dword	eax, edi
	mov byte	[edi + 0x6], ah
	mov byte	[edi + 0x7], al

	mov word	[edi + 0x8], 0x0
	mov word	[edi + 0x9], 0xFFFC
	call		exec

	mov dword	edx, edi
	; Delete RSSP
	add dword	esp, 0x10008

	leave
	ret


mytest:

	push dword	ebp
	mov dword	ebp, esp

	push dword	ecx

	; Get qq register into eax
	mov dword	eax, 0x0
	mov byte	al, [edx + ecx]
	mov dword	ebx, 2
	mov dword	ecx, 5
	call		get_register_code
	call		get_register_16bit_2

	push dword	eax

	; Get real stack address into ebx
	call		get_stack_top
	mov dword	ebx, eax
	pop dword	eax


	; Store qq register content into address at SP - 1 and SP - 2
	mov dword	ecx, [eax]
	mov byte	cl, [ebx + 0x1]
	mov byte	ch, [ebx + 0x2]
	add word	[edi + 0x9], 0x2

	mov byte	[eax], cl
	mov byte	[eax + 0x1], ch

	pop dword	ecx
	inc dword	ecx

	leave
	ret

debug3:

	push dword	ebp 
	mov	dword	ebp, esp

	push dword	eax
	push dword	[eax]
	push dword	tmp_str 
	call		printf

	leave
	ret


debug2:

	push dword	ebp 
	mov	dword	ebp, esp

	push dword	[edx + ecx]
	push dword	[edx + ecx]
	push dword	ecx
	push dword	edx
	push dword	dbgFormat

	call		printf

	leave
	ret

debug:

	push dword	ebp 
	mov	dword	ebp, esp

	mov dword	eax, [edi + 0x1]
	and byte	al, 1000b
	cmp byte	al, 1000b
	je			debug_flags_add_z

	push dword	' '
	jmp			debug_flags_n

debug_flags_add_z:

	push dword	'Z'

debug_flags_n :

	mov dword	eax, [edi + 0x1]
	and byte	al, 100b
	cmp byte	al, 100b
	je			debug_flags_add_n

	push dword	' '
	jmp			debug_flags_h

debug_flags_add_n:

	push dword	'N'

debug_flags_h :

	mov dword	eax, [edi + 0x1]
	and byte	al, 10b
	cmp byte	al, 10b
	je			debug_flags_add_h

	push dword	' '
	jmp			debug_flags_cy

debug_flags_add_h:

	push dword	'H'

debug_flags_cy :

	mov dword	eax, [edi + 0x1]
	and byte	al, 1b
	cmp byte	al, 1b
	je			debug_flags_add_cy

	push dword	' '
	jmp			debug_end

debug_flags_add_cy:

	push dword	'C'

debug_end :

	; SP
	mov dword	eax, edi
	add dword	eax, 0x9
	push dword	[eax]
	push dword	eax

	; PC
	mov dword	eax, edi
	add dword	eax, 0x8
	push dword	[eax]
	push dword	eax

	; L
	mov dword	eax, edi
	add dword	eax, 0x7
	push dword	[eax]
	push dword	eax

	; H
	mov dword	eax, edi
	add dword	eax, 0x6
	push dword	[eax]
	push dword	eax

	; E
	mov dword	eax, edi
	add dword	eax, 0x5
	push dword	[eax]
	push dword	eax

	; D
	mov dword	eax, edi
	add dword	eax, 0x4
	push dword	[eax]
	push dword	eax

	; C
	mov dword	eax, edi
	add dword	eax, 0x3
	push dword	[eax]
	push dword	eax

	; B
	mov dword	eax, edi
	add dword	eax, 0x2
	push dword	[eax]
	push dword	eax

	; F
	mov dword	eax, edi
	add dword	eax, 0x1
	push dword	[eax]
	push dword	eax

	; A
	mov dword	eax, edi
	push dword	[eax]
	push dword	eax

	push dword	register_str
	call		printf

	mov dword	eax, eax
	mov dword	ebx, ebx

	leave
	ret

init:

	push dword	ebp
	mov dword	ebp, esp

	; Clear registers
	xor dword	eax, eax
	xor dword	ebx, ebx
	xor dword	ecx, ecx
	xor dword	edx, edx

	; Clear flags
	mov byte	[edi + 0x1], 0x0

	; Initialize RSSP
	mov byte	[edi + 0xFF09], 0x0		; P1 <- 0
	mov byte	[edi + 0xFF0A], 0x0		; SC <- 0
	mov byte	[edi + 0xFF0E], 0x0		; TIMA <- 0
	mov byte	[edi + 0xFF0F], 0x0		; TAC <- 0
	mov byte	[edi + 0x10008], 0x0	; IE <- 0
	mov byte	[edi + 0xFF49], 0x0		; LCDC <- $83 BG/OBJ ON, LCDC OPERATION
	mov byte	[edi + 0xFF4B], 0x0		; SCY <- 0
	mov byte	[edi + 0xFF4C], 0x0		; SCX <- 0
	mov byte	[edi + 0xFF4E], 0x0		; LYC <- 0
	mov byte	[edi + 0xFF53], 0x0		; WY <- 0
	mov byte	[edi + 0xFF54], 0x0		; WX <- 0

	xor dword	ecx, ecx				; index = 0
	xor dword	edx, edx				; clear edx
	mov dword	edx, buffer				; edx = buffer

	leave
	ret

error :

	push dword	error_str
	call		printf

	leave
	ret

exec :

	push dword	ebp
	mov dword	ebp, esp

	push dword	ecx
	push dword	edx
	call		debug2
	call		debug
	pop dword	edx
	pop dword	ecx

	call		mytest

	push dword	ecx
	push dword	edx
	call		debug2
	call		debug
	pop dword	edx
	pop dword	ecx


	mov dword	eax, 0

	leave
	ret
%endif