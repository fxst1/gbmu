%ifndef	ASM_ROT
%define ASM_ROT
%include 'src/cpu/register.s'
%include 'src/cpu/instruction.s'

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; RLCA | 00 000 111 | P98

instruction_rlca:

	push qword	rcx

	; Get A register
	mov byte	al, [rdi]

	; Get 8th bit
	mov byte	ah, al
	shr byte	ah, 7

	; Set it into CY
	mov qword	rbx, 0x0
	mov byte	bl, al
	mov qword	rcx, 0
	call		change_flag

	; A <- (A << 1)
	shl word	[rdi], 1

	; Place bit 0
	or byte		[rdi], ah

	; Save flags
	mov qword	rbx, 0
	mov qword	rcx, 1
	call		change_flag
	mov qword	rbx, 0
	mov qword	rcx, 2
	call		change_flag
	mov qword	rbx, 0
	mov qword	rcx, 3
	call		change_flag

	pop qword	rcx
	inc qword	rcx

	jmp			instruction_loop

; RLA | 00 010 111 | P98

instruction_rla:

	push qword	rcx

	; Get A register
	mov byte	al, [rdi]

	; Get 8th bit
	mov byte	ah, al
	shr byte	ah, 7

	; Get CY bit from flags
	mov byte	bh, [rdi + 0x1]
	and byte	bh, 1

	; A <- (A << 1)
	shl word	[rdi], 1

	; Place bit 0
	or byte		[rdi], bh

	; Set it into CY
	mov qword	rbx, 0x0
	mov byte	bl, ah
	mov qword	rcx, 0
	call		change_flag


	; Save flags
	mov qword	rbx, 0
	mov qword	rcx, 1
	call		change_flag
	mov qword	rbx, 0
	mov qword	rcx, 2
	call		change_flag
	mov qword	rbx, 0
	mov qword	rcx, 3
	call		change_flag

	pop qword	rcx
	inc qword	rcx

	jmp			instruction_loop

; RRCA | 00 001 111 | P98

instruction_rrca:

	push qword	rcx

	; Get A register
	mov byte	al, [rdi]

	; Get 1st bit
	mov byte	ah, al
	and byte	ah, 1
	shl byte	ah, 7	; 1st bit to 8th bit

	; Set it into CY
	mov qword	rbx, 0x0
	mov byte	bl, ah
	mov qword	rcx, 0
	call		change_flag

	; A <- (A >> 1)
	shr word	[rdi], 1

	; Place bit 8
	or byte		[rdi], ah

	; Save flags
	mov qword	rbx, 0
	mov qword	rcx, 1
	call		change_flag
	mov qword	rbx, 0
	mov qword	rcx, 2
	call		change_flag
	mov qword	rbx, 0
	mov qword	rcx, 3
	call		change_flag

	pop qword	rcx
	inc qword	rcx

	jmp			instruction_loop

; RRA | 00 011 111 | P98

instruction_rra:

	push qword	rcx

	; Get A register
	mov byte	al, [rdi]

	; Get 1st bit
	mov byte	ah, al
	and byte	ah, 1
	shl byte	ah, 7		; 1st bit to 8th bit

	; Get CY bit from flags
	mov byte	bh, [rdi + 0x1]
	and byte	bh, 1

	; A <- (A >> 1)
	shr word	[rdi], 1

	; Place bit 8
	or byte		[rdi], bh

	; Set it into CY
	mov qword	rbx, 0x0
	mov byte	bl, ah
	mov qword	rcx, 0
	call		change_flag


	; Save flags
	mov qword	rbx, 0
	mov qword	rcx, 1
	call		change_flag
	mov qword	rbx, 0
	mov qword	rcx, 2
	call		change_flag
	mov qword	rbx, 0
	mov qword	rcx, 3
	call		change_flag

	pop qword	rcx
	inc qword	rcx

	jmp			instruction_loop

; RLC	m | 11 001 011 - 00 000 r | P99

instruction_rlc :

	inc qword	rcx
	push qword	rcx
	push qword	rdx

	; Get m register (eax <- code)
	mov qword	rax, [rdx + rcx]	; opcode
	mov qword	rbx, 3				; 3 bits
	mov qword	rcx, 2				; @ 3th bit
	call		get_register_code

	cmp byte	al, 110b			; HL code
	je			instruction_rlc_HL

	; SPECIFIC 8 BIT REGISTER
	call		get_register_8bit	; eax <- &m
	mov word	dx, [rax]			; Save address into edx

	; Get 8th bit
	mov byte	dh, dl
	shr byte	dh, 7

	; Set it into CY
	mov qword	rbx, 0x0
	mov byte	bl, dh
	mov qword	rcx, 0
	call		change_flag

	; [m] <- ([m] << 1)
	shl byte	[rax], 1

	; Place bit 0
	or byte		[rax], dh
	mov	byte	al, [rax]
	call		set_flag_z8

	; Save flags
	mov qword	rbx, 0
	mov qword	rcx, 1
	call		change_flag
	mov qword	rbx, 0
	mov qword	rcx, 2
	call		change_flag

	pop qword	rdx
	pop qword	rcx
	inc qword	rcx

	jmp			instruction_loop

instruction_rlc_HL :

	; Get address
	mov qword	rax, 0x0
	mov byte	ah, [rdi + 0x6]
	mov byte	al, [rdi + 0x7]

	; Load real address on stack
	call		get_real_address
	mov qword	rdx, 0x0
	mov byte	dl, [rax]
	mov byte	dh, dl

	; Get 8th bit
	shr byte	dh, 7

	; Set it into CY
	mov qword	rbx, 0x0
	mov byte	bl, dh
	mov qword	rcx, 0
	call		change_flag

	; [HL] <- ([HL] << 1)
	shl byte	dl, 1

	; Place bit 0
	or byte		dl, dh	

	; Set value at [rax]
	mov	byte	[rax], dl
	mov	byte	al, [rax]
	call		set_flag_z8

	; Save flags
	mov qword	rbx, 0
	mov qword	rcx, 1
	call		change_flag

	; Save flags
	mov qword	rbx, 0
	mov qword	rcx, 2
	call		change_flag


	pop qword	rdx
	pop qword	rcx
	inc qword	rcx

	jmp			instruction_loop

; RL	m | 11 001 011 - 00 010 r | P99
instruction_rl :

	inc qword	rcx
	push qword	rcx
	push qword	rdx

	; Get m register (eax <- code)
	mov qword	rax, [rdx + rcx]	; opcode
	mov qword	rbx, 3				; 3 bits
	mov qword	rcx, 2				; @ 3th bit
	call		get_register_code

	cmp byte	al, 110b			; HL code
	je			instruction_rl_HL

	; SPECIFIC 8 BIT REGISTER
	call		get_register_8bit	; eax <- &m
	mov word	dx, [rax]			; Save address into edx

	; Get 8th bit
	mov byte	dh, dl
	shr byte	dh, 7

	; Get CY bit from flags
	mov byte	bh, [rdi + 0x1]
	and byte	bh, 1


	; [m] <- ([m] << 1)
	shl byte	[rax], 1

	; Place bit 0
	or byte		[rax], bh

	; Set into CY
	mov qword	rbx, 0x0
	mov byte	bl, dh
	mov qword	rcx, 0
	call		change_flag

	mov	byte	al, [rax]
	call		set_flag_z8

	; Save flags
	mov qword	rbx, 0
	mov qword	rcx, 1
	call		change_flag
	mov qword	rbx, 0
	mov qword	rcx, 2
	call		change_flag

	pop qword	rdx
	pop qword	rcx
	inc qword	rcx

	jmp			instruction_loop

instruction_rl_HL :

	; Get address
	mov qword	rax, 0x0
	mov byte	ah, [rdi + 0x6]
	mov byte	al, [rdi + 0x7]

	; Load real address on stack
	call		get_real_address
	mov qword	rdx, 0x0
	mov byte	dl, [rax]
	mov byte	dh, dl

	; Get 8th bit
	shr byte	dh, 7

	; Get CY bit from flags
	mov byte	bh, [rdi + 0x1]
	and byte	bh, 1

	; Set into CY
	push qword	rbx
	mov qword	rbx, 0x0
	mov byte	bl, dh
	mov qword	rcx, 0
	call		change_flag
	pop qword	rbx

	; [HL] <- ([HL] << 1)
	shl byte	dl, 1

	; Place bit 0
	or byte		dl, bh	

	; Set value at [rax]
	mov	byte	[rax], dl
	mov	byte	al, dl
	call		set_flag_z8

	; Save flags
	mov qword	rbx, 0
	mov qword	rcx, 1
	call		change_flag

	; Save flags
	mov qword	rbx, 0
	mov qword	rcx, 2
	call		change_flag


	pop qword	rdx
	pop qword	rcx
	inc qword	rcx

	jmp			instruction_loop

; RRC	m | 11 001 011 - 00 001 r | P100

instruction_rrc :

	inc qword	rcx
	push qword	rcx
	push qword	rdx

	; Get m register (eax <- code)
	mov qword	rax, [rdx + rcx]	; opcode
	mov qword	rbx, 3				; 3 bits
	mov qword	rcx, 2				; @ 3th bit
	call		get_register_code

	cmp byte	al, 110b			; HL code
	je			instruction_rcc_HL

	; SPECIFIC 8 BIT REGISTER
	call		get_register_8bit	; eax <- &m
	mov word	dx, [rax]			; Save address into edx

	; Get 1st bit
	mov byte	dh, dl
	and byte	dh, 1

	; Set it into CY
	mov qword	rbx, 0x0
	mov byte	bl, dh
	mov qword	rcx, 0
	call		change_flag

	; [m] <- ([m] >> 1)
	shr byte	[rax], 1

	; Place bit 8
	shl byte	dh, 7
	or byte		[rax], dh
	mov	byte	al, [rax]
	call		set_flag_z8

	; Save flags
	mov qword	rbx, 0
	mov qword	rcx, 1
	call		change_flag
	mov qword	rbx, 0
	mov qword	rcx, 2
	call		change_flag

	pop qword	rdx
	pop qword	rcx
	inc qword	rcx

	jmp			instruction_loop

instruction_rcc_HL :

	; Get address
	mov qword	rax, 0x0
	mov byte	ah, [rdi + 0x6]
	mov byte	al, [rdi + 0x7]

	; Load real address on stack
	call		get_real_address
	mov qword	rdx, 0x0
	mov byte	dl, [rax]
	mov byte	dh, dl

	; Get 1st bit
	and byte	dh, 1

	; Set it into CY
	mov qword	rbx, 0x0
	mov byte	bl, dh
	mov qword	rcx, 0
	call		change_flag

	; [HL] <- ([HL] >> 1)
	shr byte	dl, 1

	; Place bit 8
	shl byte	dh, 7
	or byte		dl, dh	

	; Set value at [rax]
	mov	byte	[rax], dl
	mov	byte	al, [rax]
	call		set_flag_z8

	; Save flags
	mov qword	rbx, 0
	mov qword	rcx, 1
	call		change_flag

	; Save flags
	mov qword	rbx, 0
	mov qword	rcx, 2
	call		change_flag


	pop qword	rdx
	pop qword	rcx
	inc qword	rcx

	jmp			instruction_loop


; RR	m | 11 001 011 - 00 011 r | P100

instruction_rr :

	inc qword	rcx
	push qword	rcx
	push qword	rdx

	; Get m register (eax <- code)
	mov qword	rax, [rdx + rcx]	; opcode
	mov qword	rbx, 3				; 3 bits
	mov qword	rcx, 2				; @ 3th bit
	call		get_register_code

	cmp byte	al, 110b			; HL code
	je			instruction_rr_HL

	; SPECIFIC 8 BIT REGISTER
	call		get_register_8bit	; eax <- &m
	mov word	dx, [rax]			; Save address into edx

	; Get 1st bit
	mov byte	dh, dl
	and byte	dh, 1

	; Get CY bit from flags
	mov byte	bh, [rdi + 0x1]
	and byte	bh, 1

	; [m] <- ([m] >> 1)
	shr byte	[rax], 1

	; Place bit 8
	shl byte	bh, 7
	or byte		[rax], bh

	; Set into CY
	mov qword	rbx, 0x0
	mov byte	bl, dh
	mov qword	rcx, 0
	call		change_flag

	mov	byte	al, [rax]
	call		set_flag_z8

	; Save flags
	mov qword	rbx, 0
	mov qword	rcx, 1
	call		change_flag
	mov qword	rbx, 0
	mov qword	rcx, 2
	call		change_flag

	pop qword	rdx
	pop qword	rcx
	inc qword	rcx

instruction_rr_HL :

	; Get address
	mov qword	rax, 0x0
	mov byte	ah, [rdi + 0x6]
	mov byte	al, [rdi + 0x7]

	; Load real address on stack
	call		get_real_address
	mov qword	rdx, 0x0
	mov byte	dl, [rax]
	mov byte	dh, dl

	; Get 1st bit
	and byte	dh, 1

	; Get CY bit from flags
	mov byte	bh, [rdi + 0x1]
	and byte	bh, 1

	; Set into CY
	push qword	rbx
	mov qword	rbx, 0x0
	mov byte	bl, dh
	mov qword	rcx, 0
	call		change_flag
	pop qword	rbx

	; [HL] <- ([HL] >> 1)
	shr byte	dl, 1

	; Place bit 8
	shl byte	bh, 7
	or byte		dl, bh	

	; Set value at [rax]
	mov	byte	[rax], dl
	mov	byte	al, dl
	call		set_flag_z8

	; Save flags
	mov qword	rbx, 0
	mov qword	rcx, 1
	call		change_flag

	; Save flags
	mov qword	rbx, 0
	mov qword	rcx, 2
	call		change_flag


	pop qword	rdx
	pop qword	rcx
	inc qword	rcx

%endif