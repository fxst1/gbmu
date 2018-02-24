%ifndef	ASM_SHIFT
%define ASM_SHIFT
%include 'src/cpu/register.s'
%include 'src/cpu/instruction.s'

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; SLA	r | 11 001 011 - 00 100 r | P101

instruction_sla :

	inc qword	rcx
	push qword	rcx
	push qword	rdx

	; Get m register (eax <- code)
	mov qword	rax, [rdx + rcx]	; opcode
	mov qword	rbx, 3				; 3 bits
	mov qword	rcx, 2				; @ 3th bit
	call		get_register_code

	cmp byte	al, 110b			; HL code
	je			instruction_sla_HL

	; SPECIFIC 8 BIT REGISTER
	call		get_register_8bit	; eax <- &m
	mov word	dx, [rax]			; Save address into edx

	; Get 8th bit
	mov byte	dh, dl
	shr byte	dh, 7

	; Set into CY
	mov qword	rbx, 0x0
	mov byte	bl, dh
	mov qword	rcx, 0
	call		change_flag

	; [m] <- ([m] << 1)
	shl byte	[rax], 1

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

instruction_sla_HL :

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

	; Set into CY
	mov qword	rbx, 0x0
	mov byte	bl, dh
	mov qword	rcx, 0
	call		change_flag

	; [HL] <- ([HL] << 1)
	shl byte	dl, 1

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

; SRA	r | 11 001 011 - 00 101 r | P101

instruction_sra :

	inc qword	rcx
	push qword	rcx
	push qword	rdx

	; Get m register (eax <- code)
	mov qword	rax, [rdx + rcx]	; opcode
	mov qword	rbx, 3				; 3 bits
	mov qword	rcx, 2				; @ 3th bit
	call		get_register_code

	cmp byte	al, 110b			; HL code
	je			instruction_sra_HL

	; SPECIFIC 8 BIT REGISTER
	call		get_register_8bit	; eax <- &m
	mov word	dx, [rax]			; Save address into edx

	; Get 1st bit
	mov byte	dh, dl
	and byte	dh, 1

	; Set into CY
	mov qword	rbx, 0x0
	mov byte	bl, dh
	mov qword	rcx, 0
	call		change_flag

	; Get 8th bit only
	mov byte	dh, dl
	and byte	dh, 10000000b

	; [m] <- ([m] >> 1) | dh
	shr byte	[rax], 1
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

instruction_sra_HL :

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

	; Set into CY
	mov qword	rbx, 0x0
	mov byte	bl, dh
	mov qword	rcx, 0
	call		change_flag

	; Get 8th bit only
	mov byte	dh, dl
	and byte	dh, 10000000b

	; [HL] <- ([HL] >> 1) | dh
	shr byte	dl, 1
	or byte		dl, dh

	; Set value at [rax]
	mov	byte	[rax], dl

	; Set Z flag
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

; SRL	r | 11 001 011 - 00 111 r | P102

instruction_srl :

	inc qword	rcx
	push qword	rcx
	push qword	rdx

	; Get m register (eax <- code)
	mov qword	rax, [rdx + rcx]	; opcode
	mov qword	rbx, 3				; 3 bits
	mov qword	rcx, 2				; @ 3th bit
	call		get_register_code

	cmp byte	al, 110b			; HL code
	je			instruction_srl_HL

	; SPECIFIC 8 BIT REGISTER
	call		get_register_8bit	; eax <- &m
	mov word	dx, [rax]			; Save address into edx

	; Get 1st bit
	mov byte	dh, dl
	and byte	dh, 1

	; Set into CY
	mov qword	rbx, 0x0
	mov byte	bl, dh
	mov qword	rcx, 0
	call		change_flag

	; [m] <- ([m] >> 1)
	shr byte	[rax], 1

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

instruction_srl_HL :

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

	; Set into CY
	mov qword	rbx, 0x0
	mov byte	bl, dh
	mov qword	rcx, 0
	call		change_flag

	; [HL] <- ([HL] >> 1)
	shr byte	dl, 1

	; Set value at [rax]
	mov	byte	[rax], dl

	; Set Z flag
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

%endif