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
;	PC : 0x8 - 0x9
;	SP : 0xA - 0xB (0xffff Stack size @ 0x1000B)
;
;	MEMORY MAP @0x1000B
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
;	Reserved memory address is in edi register
;

%ifndef	ASM_REG
%define ASM_REG
%include 'src/cpu/register.s'
%include 'src/cpu/instruction.s'

; Z N H CY
set_flags_16 :

	push qword	rbp
	mov qword	rbp, rsp

	push qword	rbx
	push qword	rcx
	push qword	rdx
	push qword	rdi

	push qword	rax					; Save flags code
	mov qword	rax, rsi			; ax <- CPU flags
	xor qword	rdx, rdx
	mov qword	rdi, rax			; edi <- eax

	; Manage Z flag
	mov qword	rax, [rsp]
	mov qword	rbx, 2				; 2 bit code
	mov qword	rcx, 7				; 8th bit
	call		get_register_code

	mov word	cx, 1000b
	mov word	bx, 0b				; 0 by default

	cmp  byte	al, 1b
	cmove word	bx, cx				; code == 1 : bx <- 1000b
	cmp byte	al, 10b
	je			set_flag_z_16		; code == 2 : bx <- CPU ZF flag
	jmp			set_flags2_16		; code == 3 : continue

set_flag_z_16:

	mov qword	rax, rdi
	and byte	al, 1000000b
	cmp byte	al, 1000000b
	cmove word	bx, cx

set_flags2_16:

	mov byte	dh, bl

	; Manage N flag
	mov qword	rax, [rsp]			; Get save flags code
	mov qword	rbx, 2				; 2 bit code
	mov qword	rcx, 5				; 5th bit
	call		get_register_code

	mov word	cx, 100b
	mov word	bx, 0b				; 0 by default

	cmp byte	al, 1b
	cmovz word	bx, cx				; code == 1 : bx <- 100b

	or byte		dh, bl				; dh <- dh | bl (insert flag value into dh)

	; Manage H flag
	mov qword	rax, [rsp]
	mov qword	rbx, 2				; 2 bit code
	mov qword	rcx, 3				; 3th bit
	call		get_register_code

	mov word	cx, 10b
	mov word	bx, 0b				; 0 by default

	cmp  byte	al, 1b
	cmove word	bx, cx				; code == 1 : bx <- 10b
	cmp byte	al, 10b
	je			set_flag_h_16		; code == 2 : bx <- CPU CF flag
	jmp			set_flags3_16

set_flag_h_16:

	mov qword	rax, rdi
	and byte	al, 1b
	cmp byte	al, 1b
	cmove word	bx, cx

set_flags3_16:

	or byte		dh, bl				; dh <- dh | bl (insert flag value into dh)

	; Manage CY flag
	mov qword	rax, [rsp]
	mov qword	rbx, 2				; 2 bit code
	mov qword	rcx, 1				; 1st bit
	call		get_register_code

	mov word	cx, 1b
	mov word	bx, 0b				; 0 by default

	cmp  byte	al, 1b
	cmove word	bx, cx				; code == 1 : bx <- 10
	cmp byte	al, 10b
	je			set_flag_cy_16		; code == 2 : bx <- CPU OF flag
	jmp			set_flagsend_16		; code == 3 : do nothing

set_flag_cy_16:

	mov qword	rax, rdi
	and word	ax, 0x800
	cmp word	ax, 0x800
;	and byte	al, 1b
;	cmp byte	al, 1b
	cmove word	bx, cx

set_flagsend_16:

	or byte		dh, bl				; dh <- dh | bl (insert flag value into dh)
;	mov byte	dh, 0xFF

	pop qword	rax
	pop qword	rdi

	; Save flag in RSSP
	mov byte	dl, 0x0
	mov byte	[rdi + 0x1], dh

	pop qword	rdx
	pop qword	rcx
	pop qword	rbx

	leave
	ret

get_register_code :

	push qword	rbp
	mov qword	rbp, rsp

	push qword	rdx
	push qword	rcx

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

	pop qword	rcx
	pop qword	rdx

	leave
	ret

is_register_8bit :

	push qword	rbp
	mov qword	rbp, rsp

	mov qword	rbx, 0x1
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
	xor qword	rbx, rbx

	leave
	ret

is_register_16bit :

	push qword	rbp
	mov qword	rbp, rsp

	push qword	rdx
	mov qword	rdx, 0x0
	mov byte	dl, al
	mov qword	rax, rdx
	pop qword	rdx

	cmp qword	rax, 00b		;is register BC
	je			is_register_ok
	cmp qword	rax, 01b		;DE
	je			is_register_ok
	cmp qword	rax, 10b		;HL
	je			is_register_ok
	cmp qword	rax, 11b		;SP
	je			is_register_ok

	mov qword	rax, 0x0
	leave
	ret

is_register_ok :

	mov qword	rax, 0x1

	leave
	ret

;; GETTERS

get_real_address :

	push qword	rbp
	mov qword	rbp, rsp

	push qword	rbx

	mov qword	rbx, rdi
	add qword	rbx, 0xB
	add word	bx, ax
	mov qword	rax, rbx

	pop qword	rbx

	leave
	ret

get_ram_address :

	push qword	rbp
	mov qword	rbp, rsp

	push qword	rbx

	mov qword	rbx, rdi
	add qword	rbx, 0x1000B
	add word	bx, ax
	mov qword	rax, rbx

	pop qword	rbx

	leave
	ret

get_stack_top :

	push qword	rbp
	mov qword	rbp, rsp

	push qword	rcx

	; eax <- Stack start address
	mov qword	rax, rdi
	add qword	rax, 0xB

	; ecx <- (size - offset)
	mov qword	rcx, 0xffff
	sub qword	rcx, [rdi + 0xA]

	; eax <- (start + ecx)
	add qword	rax, rcx

	pop qword	rcx

	leave
	ret

load_real_address :

	push qword	rbp
	mov qword	rbp, rsp

	push qword	rbx

	mov qword	rbx, rdi
	sub qword	rbx, 0x9
	sub word	bx, ax

	mov qword	rax, 0x0
	mov byte	al, [rbx]

	pop qword	rbx

	leave
	ret	

get_register_8bit :

	push qword	rbp
	mov qword	rbp, rsp

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

	push qword	rbp
	mov qword	rbp, rsp

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

	push qword	rbp
	mov qword	rbp, rsp

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

	mov	qword	rax, rdi

	leave
	ret

get_register_B :

	mov	qword	rax, rdi
	add qword	rax, 0x2

	leave
	ret

get_register_C :

	mov	qword	rax, rdi
	add qword	rax, 0x3

	leave
	ret

get_register_D :

	mov	qword	rax, rdi
	add qword	rax, 0x4

	leave
	ret

get_register_E :

	mov	qword	rax, rdi
	add qword	rax, 0x5

	leave
	ret

get_register_H :

	mov	qword	rax, rdi
	add qword	rax, 0x6

	leave
	ret

get_register_L :

	mov	qword	rax, rdi
	add qword	rax, 0x7

	leave
	ret

;	Register pair

get_register_AF :

	mov	qword	rax, rdi

	leave
	ret

get_register_BC :

	mov	qword	rax, rdi
	add qword	rax, 0x2

	leave
	ret

get_register_DE :

	mov	qword	rax, rdi
	add qword	rax, 0x4

	leave
	ret

get_register_HL :

	mov	qword	rax, rdi
	add qword	rax, 0x6

	leave
	ret

get_register_SP :

	mov	qword	rax, rdi
	add qword	rax, 0x9

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

	mov	byte	[rdi + 0x0], al
	ret

set_register_B :

	mov	byte	[rdi + 0x10], al
	ret

set_register_C :

	mov	byte	[rdi + 0x18], al
	ret

set_register_D :

	mov	byte	[rdi + 0x20], al
	ret

set_register_E :

	mov	byte	[rdi + 0x28], al
	ret

set_register_H :

	mov	byte	[rdi + 0x30], al
	ret

set_register_L :

	mov	byte	[rdi + 0x38], al
	ret

;	Register pair

set_register_AF :

	mov	word	[rdi + 0x0], ax
	ret

set_register_BC :

	mov	word	[rdi + 0x10], ax
	ret

set_register_DE :

	mov	word	[rdi + 0x20], ax
	ret

set_register_HL :

	mov	word	[rdi + 0x30], ax
	ret

save_register_HL :

	mov	word	[rdi + 0x30], ax
	ret

set_register_SP :

	mov	word	[rdi + 0x50], ax
	ret

load_register_HL :

	ret


%endif