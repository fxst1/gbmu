%ifndef	ASM_LD
%define ASM_LD
%include 'src/cpu/register.s'
%include 'src/cpu/instruction.s'

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; LD	dd, nn | 00 dd0 001 - n - n | P90

instruction_ld_16_1 :

	push qword	rdx
	push qword	rcx

	; Get dd address register into eax
	mov qword	rax, 0x0
	mov qword	rax, [rdx + rcx]	; opcode
	mov qword	rbx, 2				; 2 bits
	mov qword	rcx, 5				; @ 5th bit
	call		get_register_code
	call		get_register_16bit

	; Get nn value
	pop qword	rcx
	inc qword	rcx
	mov byte	bl, [rdx + rcx]
	mov byte	[rax], bl
	inc qword	rcx
	mov byte	bl, [rdx + rcx]
	mov byte	[rax + 0x1], bl

	pop qword	rdx
	inc qword	rcx

	jmp			instruction_loop

; LD	SP, HL | 11 111 001 | P90

instruction_ld_16_2 :

	mov byte	al, [rdi + 0x6]
	mov byte	ah, [rdi + 0x7]
	mov word	[rdi + 0x9], ax
	inc qword	rcx
	jmp			instruction_loop

; LD	(nn), SP | 00 001 000 | P91

instruction_ld_16_3 :

	push qword	rdx
	push qword	rcx

	; Get nn address into eax
	mov qword	rax, 0x0
	inc qword	rcx
	mov byte	al, [rdx + rcx]
	inc qword	rcx
	mov byte	ah, [rdx + rcx]

	; Store SP in [rax]
	call		get_real_address
	mov qword	rbx, 0x0
	mov word	bx, [rdi + 0x9]
	mov byte	[rax], bl
	mov byte	[rax + 0x1], bh

	inc qword	rcx

	jmp			instruction_loop


; LD	r, r' | 01 r r' | P85

instruction_ld_8_1 :

	push qword	rdx
	push qword	rcx

	; Get r' address register into eax
	mov qword	rax, 0x0
	mov qword	rax, [rdx + rcx]	; opcode
	mov qword	rbx, 3				; 3 bits
	mov qword	rcx, 2				; @ 3th bit
	call		get_register_code

	cmp byte	al, 110b
	je			instruction_ld_8_1_fromHL

	; SPECIFIC 8 BIT REGISTER
	call		get_register_8bit	; eax <- &r'
	mov qword	rdx, rax			; Save address into edx

	; Get r address register into eax
	mov qword	rcx, [rsp]
	mov qword	rax, [rdx + rcx]	; opcode
	mov qword	rbx, 3				; 3 bits
	mov qword	rcx, 5				; @ 5th bit
	call		get_register_code

	cmp byte	al, 110b
	je			instruction_ld_8_1_toHL

	; SPECIFIC 8 BIT REGISTER
	call		get_register_8bit	; eax <- &r

	; r <- r'
	mov byte	bl, [rdx]
	mov byte	[rax], bl

	pop qword	rcx
	pop qword	rdx

	inc qword	rcx

	jmp			instruction_loop

; (HL) <- r'
instruction_ld_8_1_toHL :

	mov qword	rax, 0x0
	mov byte	ah, [rdi + 0x6]
	mov byte	al, [rdi + 0x7]
	call		get_real_address

	mov byte	bl, [rax]
	mov byte	[rdx], bl

	pop qword	rcx
	pop qword	rdx

	inc qword	rcx

	jmp			instruction_loop

; r <- (HL)
instruction_ld_8_1_fromHL :

	mov qword	rax, 0x0
	mov byte	ah, [rdi + 0x6]
	mov byte	al, [rdi + 0x7]
	call		get_real_address
	mov qword	rdx, rax

	; Get r address register into eax
	mov qword	rcx, [rsp]
	mov qword	rax, [rdx + rcx]	; opcode
	mov qword	rbx, 3				; 3 bits
	mov qword	rcx, 5				; @ 5th bit
	call		get_register_code

	; SPECIFIC 8 BIT REGISTER
	call		get_register_8bit	; eax <- &r

	mov byte	bl, [rdx]
	mov byte	[rax], bl

	pop qword	rcx
	pop qword	rdx

	inc qword	rcx

	jmp			instruction_loop

; LD	r, n | 00 r 110 - n | P85

instruction_ld_8_1_5 :

	push qword	rdx
	push qword	rcx

	; Get r' address register into eax
	mov qword	rax, 0x0
	mov qword	rax, [rdx + rcx]	; opcode
	mov qword	rbx, 3				; 3 bits
	mov qword	rcx, 2				; @ 3th bit
	call		get_register_code

	cmp byte	al, 110b
	je			instruction_ld_8_1_5_HL

	; SPECIFIC 8 BIT REGISTER
	call		get_register_8bit	; eax <- &r'
	mov qword	rdx, rax			; Save address into edx

	; Get n into eax
	mov qword	rcx, [rsp]
	inc qword	rcx
	mov qword	rax, [rdx + rcx]

	; r <- n
	mov byte	[rdx], al

	pop qword	rcx
	pop qword	rdx

	add qword	rcx, 2

	jmp			instruction_loop

instruction_ld_8_1_5_HL :

	mov qword	rax, 0x0
	mov word	ax, [rdi + 0x6]
	call		get_real_address
	mov qword	rdx, rax

	; Get n into eax
	mov qword	rcx, [rsp]
	inc qword	rcx
	mov qword	rax, [rdx + rcx]

	; r <- n
	mov byte	[rdx], al

	pop qword	rcx
	pop qword	rdx

	add qword	rcx, 2

	jmp			instruction_loop

; LD	A, (BC) | 00 001 010 | P86

instruction_ld_8_2 :

	mov qword	rax, 0x0
	mov byte	ah, [rdi + 0x2]
	mov byte	al, [rdi + 0x3]
	call		get_real_address

	mov	byte	al, [rax]
	mov byte	[rdi], al
	inc qword	rcx

	jmp			instruction_loop

; LD	A, (DE) | 00 011 010 | P86

instruction_ld_8_3 :

	mov qword	rax, 0x0
	mov byte	ah, [rdi + 0x4]
	mov byte	al, [rdi + 0x5]
	call		get_real_address

	mov	byte	al, [rax]
	mov byte	[rdi], al
	inc qword	rcx

	jmp			instruction_loop

; LD	A, (C) | 11 110 010 | P86

instruction_ld_8_4 :

	mov qword	rax, 0x0
	mov byte	al, [rdi + 0x3]
	add	word	ax, 0xff00
	call		get_ram_address

	mov	byte	al, [rax]
	mov byte	[rdi], al
	inc qword	rcx

	jmp			instruction_loop


; LD	(C), A | 11 100 010 | P87

instruction_ld_8_5 :

	mov qword	rax, 0x0
	mov byte	al, [rdi + 0x3]
	add	word	ax, 0xff00
	call		get_ram_address

	mov	byte	bl, [rdi]
	mov byte	[rax], bl
	inc qword	rcx

	jmp			instruction_loop

; LD	A, (n) | 11 110 000 - n | P87

instruction_ld_8_6 :

	push qword	rcx
	inc qword	rcx

	mov qword	rax, 0x0
	mov byte	al, [rdx + rcx]
	add	word	ax, 0xff00
	call		get_ram_address

	mov	byte	bl, [rax]
	mov byte	[rdi], bl

	pop qword	rcx
	inc qword	rcx

	jmp			instruction_loop


; LD	(n), A | 11 100 000  - n | P87

instruction_ld_8_7 :

	push qword	rcx
	inc qword	rcx

	mov qword	rax, 0x0
	mov byte	al, [rdx + rcx]
	add	word	ax, 0xff00
	call		get_ram_address

	mov	byte	bl, [rdi]
	mov byte	[rax], bl

	pop qword	rcx
	inc qword	rcx

	jmp			instruction_loop

; LD	A, (nn) | 11 111 010  - n - n | P87

instruction_ld_8_8 :

	push qword	rcx
	inc qword	rcx

	mov qword	rax, 0x0
	mov byte	ah, [rdx + rcx]
	inc qword	rcx
	mov byte	al, [rdx + rcx]
	call		get_ram_address

	mov	byte	bl, [rax]
	mov byte	[rdi], bl

	pop qword	rcx
	inc qword	rcx

	jmp			instruction_loop


; LD	(nn), A | 11 101 010  - n - n | P88

instruction_ld_8_9 :

	push qword	rcx
	inc qword	rcx

	mov qword	rax, 0x0
	mov byte	ah, [rdx + rcx]
	inc qword	rcx
	mov byte	al, [rdx + rcx]
	call		get_ram_address

	mov	byte	bl, [rdi]
	mov byte	[rax], bl

	pop qword	rcx
	inc qword	rcx

	jmp			instruction_loop

; LD	A, (HLI) | 00 101 010 | P88

instruction_ld_8_10 :

	mov qword	rax, 0x0
	mov byte	ah, [rdi + 0x6]
	mov byte	al, [rdi + 0x7]
	call		get_real_address

	mov qword	rbx, 0x0
	mov	byte	bl, [rax]
	mov byte	[rdi], bl

	inc byte	bl
	mov	byte	[rax], bl
	inc qword	rcx

	jmp			instruction_loop

; LD	A, (HLD) | 00 111 010 | P88

instruction_ld_8_11 :

	mov qword	rax, 0x0
	mov byte	ah, [rdi + 0x6]
	mov byte	al, [rdi + 0x7]
	call		get_real_address

	mov qword	rbx, 0x0
	mov	byte	bl, [rax]
	mov byte	[rdi], bl

	dec byte	bl
	mov	byte	[rax], bl
	inc qword	rcx

	jmp			instruction_loop

; LD	(BC), A | 00 000 010 | P88

instruction_ld_8_12 :

	mov qword	rax, 0x0
	mov byte	ah, [rdi + 0x2]
	mov byte	al, [rdi + 0x3]
	call		get_real_address

	mov qword	rbx, 0x0
	mov	byte	bl, [rdi]
	mov byte	[rax], bl
	inc qword	rcx

	jmp			instruction_loop

; LD	(DE), A | 00 010 010 | P88

instruction_ld_8_13 :

	mov qword	rax, 0x0
	mov byte	ah, [rdi + 0x4]
	mov byte	al, [rdi + 0x5]
	call		get_real_address

	mov qword	rbx, 0x0
	mov	byte	bl, [rdi]
	mov byte	[rax], bl
	inc qword	rcx

	jmp			instruction_loop

; LD	(HLI), A | 00 100 010 | P89

instruction_ld_8_14 :

	mov qword	rax, 0x0
	mov byte	ah, [rdi + 0x6]
	mov byte	al, [rdi + 0x7]
	call		get_real_address

	mov qword	rbx, 0x0
	mov	byte	bl, [rdi]
	mov byte	[rax], bl

	inc byte	bl
	mov	byte	[rax], bl
	inc qword	rcx

	jmp			instruction_loop

; LD	(HLD), A | 00 110 010 | P89

instruction_ld_8_15 :

	mov qword	rax, 0x0
	mov byte	ah, [rdi + 0x6]
	mov byte	al, [rdi + 0x7]
	call		get_real_address

	mov qword	rbx, 0x0
	mov	byte	bl, [rdi]
	mov byte	[rax], bl

	dec byte	bl
	mov	byte	[rax], bl
	inc qword	rcx

	jmp			instruction_loop


; LDHL SP e | 11 111 000 - e | P91

instruction_ldhl :

	inc qword	rcx
	push qword	rcx

	mov qword	rax, 0x0
	mov byte	al, [rdx + rcx]

	mov qword	rbx, 0x0
	mov byte	bh, [rdi + 0xA]
	mov byte	bl, [rdi + 0xB]
	add word	ax, bx

	call		set_flag_cy16
	call		set_flag_h16

	mov qword	rbx, 0x0
	mov qword	rcx, 0x0
	call		change_flag

	mov qword	rbx, 0x0
	mov qword	rcx, 0x1
	call		change_flag

	mov byte	[rdi + 0x6], ah
	mov byte	[rdi + 0x7], al

	pop qword	rcx
	inc qword	rcx

	jmp			instruction_loop

%endif