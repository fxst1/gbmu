;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; RLCA | 00 000 111 | P98

instruction_rlca:

	push dword	ecx

	; Get A register
	mov byte	al, [edi]

	; Get 8th bit
	mov byte	ah, al
	shr byte	ah, 7

	; Set it into CY
	mov dword	ebx, 0x0
	mov byte	bl, al
	mov dword	ecx, 0
	call		change_flag

	; A <- (A << 1)
	shl word	[edi], 1

	; Place bit 0
	or byte		[edi], ah

	; Save flags
	mov dword	ebx, 0
	mov dword	ecx, 1
	call		change_flag
	mov dword	ebx, 0
	mov dword	ecx, 2
	call		change_flag
	mov dword	ebx, 0
	mov dword	ecx, 3
	call		change_flag

	pop dword	ecx
	inc dword	ecx

	jmp			instruction_loop

; RLA | 00 010 111 | P98

instruction_rla:

	push dword	ecx

	; Get A register
	mov byte	al, [edi]

	; Get 8th bit
	mov byte	ah, al
	shr byte	ah, 7

	; Get CY bit from flags
	mov byte	bh, [edi + 0x1]
	and byte	bh, 1

	; A <- (A << 1)
	shl word	[edi], 1

	; Place bit 0
	or byte		[edi], bh

	; Set it into CY
	mov dword	ebx, 0x0
	mov byte	bl, ah
	mov dword	ecx, 0
	call		change_flag


	; Save flags
	mov dword	ebx, 0
	mov dword	ecx, 1
	call		change_flag
	mov dword	ebx, 0
	mov dword	ecx, 2
	call		change_flag
	mov dword	ebx, 0
	mov dword	ecx, 3
	call		change_flag

	pop dword	ecx
	inc dword	ecx

	jmp			instruction_loop

; RRCA | 00 001 111 | P98

instruction_rrca:

	push dword	ecx

	; Get A register
	mov byte	al, [edi]

	; Get 1st bit
	mov byte	ah, al
	and byte	ah, 1
	shl byte	ah, 7	; 1st bit to 8th bit

	; Set it into CY
	mov dword	ebx, 0x0
	mov byte	bl, ah
	mov dword	ecx, 0
	call		change_flag

	; A <- (A >> 1)
	shr word	[edi], 1

	; Place bit 8
	or byte		[edi], ah

	; Save flags
	mov dword	ebx, 0
	mov dword	ecx, 1
	call		change_flag
	mov dword	ebx, 0
	mov dword	ecx, 2
	call		change_flag
	mov dword	ebx, 0
	mov dword	ecx, 3
	call		change_flag

	pop dword	ecx
	inc dword	ecx

	jmp			instruction_loop

; RRA | 00 011 111 | P98

instruction_rra:

	push dword	ecx

	; Get A register
	mov byte	al, [edi]

	; Get 1st bit
	mov byte	ah, al
	and byte	ah, 1
	shl byte	ah, 7		; 1st bit to 8th bit

	; Get CY bit from flags
	mov byte	bh, [edi + 0x1]
	and byte	bh, 1

	; A <- (A >> 1)
	shr word	[edi], 1

	; Place bit 8
	or byte		[edi], bh

	; Set it into CY
	mov dword	ebx, 0x0
	mov byte	bl, ah
	mov dword	ecx, 0
	call		change_flag


	; Save flags
	mov dword	ebx, 0
	mov dword	ecx, 1
	call		change_flag
	mov dword	ebx, 0
	mov dword	ecx, 2
	call		change_flag
	mov dword	ebx, 0
	mov dword	ecx, 3
	call		change_flag

	pop dword	ecx
	inc dword	ecx

	jmp			instruction_loop

; RLC	m | 11 001 011 - 00 000 r | P99

instruction_rlc :

	inc dword	ecx
	push dword	ecx
	push dword	edx

	; Get m register (eax <- code)
	mov dword	eax, [edx + ecx]	; opcode
	mov dword	ebx, 3				; 3 bits
	mov dword	ecx, 2				; @ 3th bit
	call		get_register_code

	cmp byte	al, 110b			; HL code
	je			instruction_rlc_HL

	; SPECIFIC 8 BIT REGISTER
	call		get_register_8bit	; eax <- &m
	mov word	dx, [eax]			; Save address into edx

	; Get 8th bit
	mov byte	dh, dl
	shr byte	dh, 7

	; Set it into CY
	mov dword	ebx, 0x0
	mov byte	bl, dh
	mov dword	ecx, 0
	call		change_flag

	; [m] <- ([m] << 1)
	shl byte	[eax], 1

	; Place bit 0
	or byte		[eax], dh
	mov	byte	al, [eax]
	call		set_flag_z8

	; Save flags
	mov dword	ebx, 0
	mov dword	ecx, 1
	call		change_flag
	mov dword	ebx, 0
	mov dword	ecx, 2
	call		change_flag

	pop dword	edx
	pop dword	ecx
	inc dword	ecx

	jmp			instruction_loop

instruction_rlc_HL :

	; Get address
	mov dword	eax, 0x0
	mov byte	ah, [edi + 0x6]
	mov byte	al, [edi + 0x7]

	; Load real address on stack
	call		get_real_address
	mov dword	edx, 0x0
	mov byte	dl, [eax]
	mov byte	dh, dl

	; Get 8th bit
	shr byte	dh, 7

	; Set it into CY
	mov dword	ebx, 0x0
	mov byte	bl, dh
	mov dword	ecx, 0
	call		change_flag

	; [HL] <- ([HL] << 1)
	shl byte	dl, 1

	; Place bit 0
	or byte		dl, dh	

	; Set value at [eax]
	mov	byte	[eax], dl
	mov	byte	al, [eax]
	call		set_flag_z8

	; Save flags
	mov dword	ebx, 0
	mov dword	ecx, 1
	call		change_flag

	; Save flags
	mov dword	ebx, 0
	mov dword	ecx, 2
	call		change_flag


	pop dword	edx
	pop dword	ecx
	inc dword	ecx

	jmp			instruction_loop

; RL	m | 11 001 011 - 00 010 r | P99
instruction_rl :

	inc dword	ecx
	push dword	ecx
	push dword	edx

	; Get m register (eax <- code)
	mov dword	eax, [edx + ecx]	; opcode
	mov dword	ebx, 3				; 3 bits
	mov dword	ecx, 2				; @ 3th bit
	call		get_register_code

	cmp byte	al, 110b			; HL code
	je			instruction_rl_HL

	; SPECIFIC 8 BIT REGISTER
	call		get_register_8bit	; eax <- &m
	mov word	dx, [eax]			; Save address into edx

	; Get 8th bit
	mov byte	dh, dl
	shr byte	dh, 7

	; Get CY bit from flags
	mov byte	bh, [edi + 0x1]
	and byte	bh, 1


	; [m] <- ([m] << 1)
	shl byte	[eax], 1

	; Place bit 0
	or byte		[eax], bh

	; Set into CY
	mov dword	ebx, 0x0
	mov byte	bl, dh
	mov dword	ecx, 0
	call		change_flag

	mov	byte	al, [eax]
	call		set_flag_z8

	; Save flags
	mov dword	ebx, 0
	mov dword	ecx, 1
	call		change_flag
	mov dword	ebx, 0
	mov dword	ecx, 2
	call		change_flag

	pop dword	edx
	pop dword	ecx
	inc dword	ecx

	jmp			instruction_loop

instruction_rl_HL :

	; Get address
	mov dword	eax, 0x0
	mov byte	ah, [edi + 0x6]
	mov byte	al, [edi + 0x7]

	; Load real address on stack
	call		get_real_address
	mov dword	edx, 0x0
	mov byte	dl, [eax]
	mov byte	dh, dl

	; Get 8th bit
	shr byte	dh, 7

	; Get CY bit from flags
	mov byte	bh, [edi + 0x1]
	and byte	bh, 1

	; Set into CY
	push dword	ebx
	mov dword	ebx, 0x0
	mov byte	bl, dh
	mov dword	ecx, 0
	call		change_flag
	pop dword	ebx

	; [HL] <- ([HL] << 1)
	shl byte	dl, 1

	; Place bit 0
	or byte		dl, bh	

	; Set value at [eax]
	mov	byte	[eax], dl
	mov	byte	al, dl
	call		set_flag_z8

	; Save flags
	mov dword	ebx, 0
	mov dword	ecx, 1
	call		change_flag

	; Save flags
	mov dword	ebx, 0
	mov dword	ecx, 2
	call		change_flag


	pop dword	edx
	pop dword	ecx
	inc dword	ecx

	jmp			instruction_loop

; RRC	m | 11 001 011 - 00 001 r | P100

instruction_rrc :

	inc dword	ecx
	push dword	ecx
	push dword	edx

	; Get m register (eax <- code)
	mov dword	eax, [edx + ecx]	; opcode
	mov dword	ebx, 3				; 3 bits
	mov dword	ecx, 2				; @ 3th bit
	call		get_register_code

	cmp byte	al, 110b			; HL code
	je			instruction_rcc_HL

	; SPECIFIC 8 BIT REGISTER
	call		get_register_8bit	; eax <- &m
	mov word	dx, [eax]			; Save address into edx

	; Get 1st bit
	mov byte	dh, dl
	and byte	dh, 1

	; Set it into CY
	mov dword	ebx, 0x0
	mov byte	bl, dh
	mov dword	ecx, 0
	call		change_flag

	; [m] <- ([m] >> 1)
	shr byte	[eax], 1

	; Place bit 8
	shl byte	dh, 7
	or byte		[eax], dh
	mov	byte	al, [eax]
	call		set_flag_z8

	; Save flags
	mov dword	ebx, 0
	mov dword	ecx, 1
	call		change_flag
	mov dword	ebx, 0
	mov dword	ecx, 2
	call		change_flag

	pop dword	edx
	pop dword	ecx
	inc dword	ecx

	jmp			instruction_loop

instruction_rcc_HL :

	; Get address
	mov dword	eax, 0x0
	mov byte	ah, [edi + 0x6]
	mov byte	al, [edi + 0x7]

	; Load real address on stack
	call		get_real_address
	mov dword	edx, 0x0
	mov byte	dl, [eax]
	mov byte	dh, dl

	; Get 1st bit
	and byte	dh, 1

	; Set it into CY
	mov dword	ebx, 0x0
	mov byte	bl, dh
	mov dword	ecx, 0
	call		change_flag

	; [HL] <- ([HL] >> 1)
	shr byte	dl, 1

	; Place bit 8
	shl byte	dh, 7
	or byte		dl, dh	

	; Set value at [eax]
	mov	byte	[eax], dl
	mov	byte	al, [eax]
	call		set_flag_z8

	; Save flags
	mov dword	ebx, 0
	mov dword	ecx, 1
	call		change_flag

	; Save flags
	mov dword	ebx, 0
	mov dword	ecx, 2
	call		change_flag


	pop dword	edx
	pop dword	ecx
	inc dword	ecx

	jmp			instruction_loop


; RR	m | 00 001 011 - 00 011 r | P100

instruction_rr :

	inc dword	ecx
	push dword	ecx
	push dword	edx

	; Get m register (eax <- code)
	mov dword	eax, [edx + ecx]	; opcode
	mov dword	ebx, 3				; 3 bits
	mov dword	ecx, 2				; @ 3th bit
	call		get_register_code

	cmp byte	al, 110b			; HL code
	je			instruction_rr_HL

	; SPECIFIC 8 BIT REGISTER
	call		get_register_8bit	; eax <- &m
	mov word	dx, [eax]			; Save address into edx

	; Get 1st bit
	mov byte	dh, dl
	and byte	dh, 1

	; Get CY bit from flags
	mov byte	bh, [edi + 0x1]
	and byte	bh, 1

	; [m] <- ([m] >> 1)
	shr byte	[eax], 1

	; Place bit 8
	shl byte	bh, 7
	or byte		[eax], bh

	; Set into CY
	mov dword	ebx, 0x0
	mov byte	bl, dh
	mov dword	ecx, 0
	call		change_flag

	mov	byte	al, [eax]
	call		set_flag_z8

	; Save flags
	mov dword	ebx, 0
	mov dword	ecx, 1
	call		change_flag
	mov dword	ebx, 0
	mov dword	ecx, 2
	call		change_flag

	pop dword	edx
	pop dword	ecx
	inc dword	ecx

instruction_rr_HL :

	; Get address
	mov dword	eax, 0x0
	mov byte	ah, [edi + 0x6]
	mov byte	al, [edi + 0x7]

	; Load real address on stack
	call		get_real_address
	mov dword	edx, 0x0
	mov byte	dl, [eax]
	mov byte	dh, dl

	; Get 1st bit
	and byte	dh, 1

	; Get CY bit from flags
	mov byte	bh, [edi + 0x1]
	and byte	bh, 1

	; Set into CY
	push dword	ebx
	mov dword	ebx, 0x0
	mov byte	bl, dh
	mov dword	ecx, 0
	call		change_flag
	pop dword	ebx

	; [HL] <- ([HL] >> 1)
	shr byte	dl, 1

	; Place bit 8
	shl byte	bh, 7
	or byte		dl, bh	

	; Set value at [eax]
	mov	byte	[eax], dl
	mov	byte	al, dl
	call		set_flag_z8

	; Save flags
	mov dword	ebx, 0
	mov dword	ecx, 1
	call		change_flag

	; Save flags
	mov dword	ebx, 0
	mov dword	ecx, 2
	call		change_flag


	pop dword	edx
	pop dword	ecx
	inc dword	ecx