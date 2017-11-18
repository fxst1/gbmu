;
;	Page 13 - 14 ...
;
;	RSSP :
;	(0x0 - 0x1005F)	
;	Registers are in a reserved space in the stack
;
;	CPU Registers <0x0>
;
;	A : 0x0
;	F : 0x1
;	B : 0x2
;	C : 0x3
;	D : 0x4
;	E : 0x5
;	H : 0x6
;	L : 0x7
;	PC : 0x8
;	SP : 0x9  (0xffff Stack size @ 0x10009)
;
;	MEMORY MAP @0x10009
;
;	Port/Mode Registers <0xFF00>
;
;		 P1		-> 0xFF00 (0xFF60)
;		 SB		-> 0xFF01 (0xFF61)
;		 SC		-> 0xFF02 (0xFF62)
;		 DIV	-> 0xFF04 (0xFF64)
;		 TIMA	-> 0xFF05 (0xFF65)
;		 TMA	-> 0xFF06 (0xFF66)
;		 TAC	-> 0xFF07 (0xFF67)
;		 KEY1	-> 0xFF4D (0xFFAD)		CGB
;		 RP		-> 0xFF56 (0xFFB6)		CGB
;
;	Bank Control Register <0xFF4F>
;
;		VBK		-> 0xFF4F (0xFFAF)		CGB
;		SVBK	-> 0xFF70 (0xFFD0)		CGB
;
;	Interrupt Flags
;
;		IF		-> 0xFF0F (0xFF6F)
;		IE		-> 0xFFFF (0x1005F)
;
;	LCD Display Registers <0xFF40>
;
;		LCDC	-> 0xFF40 (0xFFA0)
;		STAT	-> 0xFF41 (0xFFA1)
;		SCY		-> 0xFF42 (0xFFA2)
;		SCX		-> 0xFF43 (0xFFA3)
;		LY		-> 0xFF44 (0xFFA4)
;		LYC		-> 0xFF45 (0xFFA5)
;		DMA		-> 0xFF46 (0xFFA6)
;		BGP		-> 0xFF47 (0xFFA7)
;		OBP0	-> 0xFF48 (0xFFA8)
;		OBP1	-> 0xFF49 (0xFFA9)
;		WY		-> 0xFF4A (0xFFAA)
;		WX		-> 0xFF4B (0xFFAB)
;		HDMA1	-> 0xFF51 (0xFFB1)		CGB
;		HDMA2	-> 0xFF52 (0xFFB2)		CGB
;		HDMA3	-> 0xFF53 (0xFFB3)		CGB
;		HDMA4	-> 0xFF54 (0xFFB4)		CGB
;		HDMA5	-> 0xFF55 (0xFFB5)		CGB
;		BCPS	-> 0xFF68 (0xFFB8)		CGB
;		BCPD	-> 0xFF69 (0xFFB9)		CGB
;		OCPS	-> 0xFF6A (0xFFBA)		CGB
;		OCPD	-> 0xFF6B (0xFFBB)		CGB
;		OAM		-> 0xFE00 (0xFE60)
;				-> 0xFE9F (0xFEFF)
;
;	Sound Registers <0xFF70>
;
;		NRxx	-> 0xFF10 (0xFF70)
;				-> 0xFF26 (0xFF86)
;		Waveform-> 0xFF30 (0xFF90)
;				-> 0xFF3F (0xFF9F)
;
;
;	Flags <0xFF>
;
;
;	Reserved memory address is in edi register
;

%include 'flag.s'

; Z N H CY
set_flags_16 :

	push dword	ebp
	mov dword	ebp, esp

	push dword	ebx
	push dword	ecx
	push dword	edx
	push dword	edi

	push dword	eax					; Save flags code
	mov dword	eax, esi			; ax <- CPU flags
	xor dword	edx, edx
	mov dword	edi, eax			; edi <- eax

	; Manage Z flag
	mov dword	eax, [esp]
	mov dword	ebx, 2				; 2 bit code
	mov dword	ecx, 7				; 8th bit
	call		get_register_code

	mov word	cx, 1000b
	mov word	bx, 0b				; 0 by default

	cmp  byte	al, 1b
	cmove word	bx, cx				; code == 1 : bx <- 1000b
	cmp byte	al, 10b
	je			set_flag_z_16		; code == 2 : bx <- CPU ZF flag
	jmp			set_flags2_16		; code == 3 : continue

set_flag_z_16:

	mov dword	eax, edi
	and byte	al, 1000000b
	cmp byte	al, 1000000b
	cmove word	bx, cx

set_flags2_16:

	mov byte	dh, bl

	; Manage N flag
	mov dword	eax, [esp]			; Get save flags code
	mov dword	ebx, 2				; 2 bit code
	mov dword	ecx, 5				; 5th bit
	call		get_register_code

	mov word	cx, 100b
	mov word	bx, 0b				; 0 by default

	cmp byte	al, 1b
	cmovz word	bx, cx				; code == 1 : bx <- 100b

	or byte		dh, bl				; dh <- dh | bl (insert flag value into dh)

	; Manage H flag
	mov dword	eax, [esp]
	mov dword	ebx, 2				; 2 bit code
	mov dword	ecx, 3				; 3th bit
	call		get_register_code

	mov word	cx, 10b
	mov word	bx, 0b				; 0 by default

	cmp  byte	al, 1b
	cmove word	bx, cx				; code == 1 : bx <- 10b
	cmp byte	al, 10b
	je			set_flag_h_16		; code == 2 : bx <- CPU CF flag
	jmp			set_flags3_16

set_flag_h_16:

	mov dword	eax, edi
	and byte	al, 1b
	cmp byte	al, 1b
	cmove word	bx, cx

set_flags3_16:

	or byte		dh, bl				; dh <- dh | bl (insert flag value into dh)

	; Manage CY flag
	mov dword	eax, [esp]
	mov dword	ebx, 2				; 2 bit code
	mov dword	ecx, 1				; 1st bit
	call		get_register_code

	mov word	cx, 1b
	mov word	bx, 0b				; 0 by default

	cmp  byte	al, 1b
	cmove word	bx, cx				; code == 1 : bx <- 10
	cmp byte	al, 10b
	je			set_flag_cy_16		; code == 2 : bx <- CPU OF flag
	jmp			set_flagsend_16		; code == 3 : do nothing

set_flag_cy_16:

	mov dword	eax, edi
	and word	ax, 0x800
	cmp word	ax, 0x800
;	and byte	al, 1b
;	cmp byte	al, 1b
	cmove word	bx, cx

set_flagsend_16:

	or byte		dh, bl				; dh <- dh | bl (insert flag value into dh)
;	mov byte	dh, 0xFF

	pop dword	eax
	pop dword	edi

	; Save flag in RSSP
	mov byte	dl, 0x0
	mov byte	[edi + 0x1], dh

	pop dword	edx
	pop dword	ecx
	pop dword	ebx

	leave
	ret

get_register_code :

	push dword	ebp
	mov dword	ebp, esp

	push dword	edx
	push dword	ecx

	; Shift left (7 - X)
	mov byte	dl, 7
	sub byte	dl, cl
	mov	byte	cl, dl
	shl byte	al, cl

	; Shift right (7 - L) + 1
	mov byte	dl, 8
	sub byte	dl, bl
	mov	byte	cl, dl
	shr byte	al, cl

	pop dword	ecx
	pop dword	edx

	leave
	ret

is_register_8bit :

	push dword	ebp
	mov dword	ebp, esp

	mov dword	ebx, 0x1
	cmp byte	al, 111b		;is register A
	je			is_register_ok
	cmp byte	al, 000b		;B
	je			is_register_ok
	cmp byte	al, 001b		;C
	je			is_register_ok
	cmp byte	al, 010b		;D
	je			is_register_ok
	cmp byte	al, 011b		;E
	je			is_register_ok
	cmp byte	al, 100b		;H
	je			is_register_ok
	cmp byte	al, 101b		;L
	je			is_register_ok
	xor dword	ebx, ebx

	leave
	ret

is_register_16bit :

	push dword	ebp
	mov dword	ebp, esp

	push dword	edx
	mov dword	edx, 0x0
	mov byte	dl, al
	mov dword	eax, edx
	pop dword	edx

	cmp dword	eax, 00b		;is register BC
	je			is_register_ok
	cmp dword	eax, 01b		;DE
	je			is_register_ok
	cmp dword	eax, 10b		;HL
	je			is_register_ok
	cmp dword	eax, 11b		;SP
	je			is_register_ok

	mov dword	eax, 0x0
	leave
	ret

is_register_ok :

	mov dword	eax, 0x1

	leave
	ret

;; GETTERS

get_real_address :

	push dword	ebp
	mov dword	ebp, esp

	push dword	ebx

	mov dword	ebx, edi
	add dword	ebx, 0x9
	add word	bx, ax
	mov dword	eax, ebx

	pop dword	ebx

	leave
	ret

get_stack_top :

	push dword	ebp
	mov dword	ebp, esp

	push dword	ecx

	; eax <- Stack start address
	mov dword	eax, edi
	add dword	eax, 0xA

	; ecx <- (size - offset)
	mov dword	ecx, 0xffff
	sub dword	ecx, [edi + 0x9]

	; eax <- (start + ecx)
	add dword	eax, ecx

	pop dword	ecx

	leave
	ret

load_real_address :

	push dword	ebp
	mov dword	ebp, esp

	push dword	ebx

	mov dword	ebx, edi
	sub dword	ebx, 0x9
	sub word	bx, ax

	mov dword	eax, 0x0
	mov byte	al, [ebx]

	pop dword	ebx

	leave
	ret	

get_register_8bit :

	push dword	ebp
	mov dword	ebp, esp

	cmp byte	al, 111b		;A
	je			get_register_A
	cmp byte	al, 000b		;B
	je			get_register_B
	cmp byte	al, 001b		;C
	je			get_register_C
	cmp byte	al, 010b		;D
	je			get_register_D
	cmp byte	al, 011b		;E
	je			get_register_E
	cmp byte	al, 100b		;H
	je			get_register_H
	cmp byte	al, 101b		;L
	je			get_register_L

	leave
	ret


get_register_16bit_2 :

	push dword	ebp
	mov dword	ebp, esp

	cmp byte	al, 0b
	je			get_register_BC
	cmp byte	al, 1b
	je			get_register_DE
	cmp byte	al, 10b
	je			get_register_HL
	cmp byte	al, 11b
	je			get_register_AF

	leave
	ret

get_register_16bit :

	push dword	ebp
	mov dword	ebp, esp

	cmp byte	al, 0b
	je			get_register_BC
	cmp byte	al, 1b
	je			get_register_DE
	cmp byte	al, 10b
	je			get_register_HL
	cmp byte	al, 11b
	je			get_register_SP

	leave
	ret

get_register_A :

	mov	dword	eax, edi

	leave
	ret

get_register_B :

	mov	dword	eax, edi
	add dword	eax, 0x2

	leave
	ret

get_register_C :

	mov	dword	eax, edi
	add dword	eax, 0x3

	leave
	ret

get_register_D :

	mov	dword	eax, edi
	add dword	eax, 0x4

	leave
	ret

get_register_E :

	mov	dword	eax, edi
	add dword	eax, 0x5

	leave
	ret

get_register_H :

	mov	dword	eax, edi
	add dword	eax, 0x6

	leave
	ret

get_register_L :

	mov	dword	eax, edi
	add dword	eax, 0x7

	leave
	ret

;	Register pair

get_register_AF :

	mov	dword	eax, edi

	leave
	ret

get_register_BC :

	mov	dword	eax, edi
	add dword	eax, 0x2

	leave
	ret

get_register_DE :

	mov	dword	eax, edi
	add dword	eax, 0x4

	leave
	ret

get_register_HL :

	mov	dword	eax, edi
	add dword	eax, 0x6

	leave
	ret

get_register_SP :

	mov	dword	eax, edi
	add dword	eax, 0x9

	leave
	ret

;; SETTERS

set_register_8bit :

	cmp byte	bl, 111b		;is register A
	je			set_register_A
	cmp byte	bl, 000b		;B
	je			set_register_B
	cmp byte	bl, 001b		;C
	je			set_register_C
	cmp byte	bl, 010b		;D
	je			set_register_D
	cmp byte	bl, 011b		;E
	je			set_register_E
	cmp byte	bl, 100b		;H
	je			set_register_H
	cmp byte	bl, 101b		;L
	je			set_register_L
	ret

set_register_16bit :

	cmp byte	bl, 0b
	je			set_register_BC
	cmp byte	bl, 1b
	je			set_register_DE
	cmp byte	bl, 10b
	je			set_register_HL
	cmp byte	bl, 11b
	je			set_register_AF
	ret

set_register_A :

	mov	byte	[edi + 0x0], al
	ret

set_register_B :

	mov	byte	[edi + 0x10], al
	ret

set_register_C :

	mov	byte	[edi + 0x18], al
	ret

set_register_D :

	mov	byte	[edi + 0x20], al
	ret

set_register_E :

	mov	byte	[edi + 0x28], al
	ret

set_register_H :

	mov	byte	[edi + 0x30], al
	ret

set_register_L :

	mov	byte	[edi + 0x38], al
	ret

;	Register pair

set_register_AF :

	mov	word	[edi + 0x0], ax
	ret

set_register_BC :

	mov	word	[edi + 0x10], ax
	ret

set_register_DE :

	mov	word	[edi + 0x20], ax
	ret

set_register_HL :

	mov	word	[edi + 0x30], ax
	ret

save_register_HL :

	mov	word	[edi + 0x30], ax
	ret

set_register_SP :

	mov	word	[edi + 0x50], ax
	ret

load_register_HL :

