;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; SLA	r | 11 001 011 - 00 100 r | P101

instruction_sla :

	inc dword	ecx
	push dword	ecx
	push dword	edx

	; Get m register (eax <- code)
	mov dword	eax, [edx + ecx]	; opcode
	mov dword	ebx, 3				; 3 bits
	mov dword	ecx, 2				; @ 3th bit
	call		get_register_code

	cmp byte	al, 110b			; HL code
	je			instruction_sla_HL

	; SPECIFIC 8 BIT REGISTER
	call		get_register_8bit	; eax <- &m
	mov word	dx, [eax]			; Save address into edx

	; Get 8th bit
	mov byte	dh, dl
	shr byte	dh, 7

	; Set into CY
	mov dword	ebx, 0x0
	mov byte	bl, dh
	mov dword	ecx, 0
	call		change_flag

	; [m] <- ([m] << 1)
	shl byte	[eax], 1

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

instruction_sla_HL :

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

	; Set into CY
	mov dword	ebx, 0x0
	mov byte	bl, dh
	mov dword	ecx, 0
	call		change_flag

	; [HL] <- ([HL] << 1)
	shl byte	dl, 1

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

; SLA	r | 11 001 011 - 00 101 r | P101

instruction_sra :

	inc dword	ecx
	push dword	ecx
	push dword	edx

	; Get m register (eax <- code)
	mov dword	eax, [edx + ecx]	; opcode
	mov dword	ebx, 3				; 3 bits
	mov dword	ecx, 2				; @ 3th bit
	call		get_register_code

	cmp byte	al, 110b			; HL code
	je			instruction_sra_HL

	; SPECIFIC 8 BIT REGISTER
	call		get_register_8bit	; eax <- &m
	mov word	dx, [eax]			; Save address into edx

	; Get 1st bit
	mov byte	dh, dl
	and byte	dh, 1

	; Set into CY
	mov dword	ebx, 0x0
	mov byte	bl, dh
	mov dword	ecx, 0
	call		change_flag

	; Get 8th bit only
	mov byte	dh, dl
	and byte	dh, 10000000b

	; [m] <- ([m] >> 1) | dh
	shr byte	[eax], 1
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

instruction_sra_HL :

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

	; Set into CY
	mov dword	ebx, 0x0
	mov byte	bl, dh
	mov dword	ecx, 0
	call		change_flag

	; Get 8th bit only
	mov byte	dh, dl
	and byte	dh, 10000000b

	; [HL] <- ([HL] >> 1) | dh
	shr byte	dl, 1
	or byte		dl, dh

	; Set value at [eax]
	mov	byte	[eax], dl

	; Set Z flag
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

; SLA	r | 11 001 011 - 00 111 r | P102

instruction_srl :

	inc dword	ecx
	push dword	ecx
	push dword	edx

	; Get m register (eax <- code)
	mov dword	eax, [edx + ecx]	; opcode
	mov dword	ebx, 3				; 3 bits
	mov dword	ecx, 2				; @ 3th bit
	call		get_register_code

	cmp byte	al, 110b			; HL code
	je			instruction_srl_HL

	; SPECIFIC 8 BIT REGISTER
	call		get_register_8bit	; eax <- &m
	mov word	dx, [eax]			; Save address into edx

	; Get 1st bit
	mov byte	dh, dl
	and byte	dh, 1

	; Set into CY
	mov dword	ebx, 0x0
	mov byte	bl, dh
	mov dword	ecx, 0
	call		change_flag

	; [m] <- ([m] >> 1)
	shr byte	[eax], 1

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

instruction_srl_HL :

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

	; Set into CY
	mov dword	ebx, 0x0
	mov byte	bl, dh
	mov dword	ecx, 0
	call		change_flag

	; [HL] <- ([HL] >> 1)
	shr byte	dl, 1

	; Set value at [eax]
	mov	byte	[eax], dl

	; Set Z flag
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
