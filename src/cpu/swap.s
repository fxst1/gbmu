;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; SWAP	r | 11 001 011 - 00 110 r | P102
instruction_swap :

	inc dword	ecx
	push dword	ecx
	push dword	edx

	; Get m register (eax <- code)
	mov dword	eax, [edx + ecx]	; opcode
	mov dword	ebx, 3				; 3 bits
	mov dword	ecx, 2				; @ 3th bit
	call		get_register_code

	cmp byte	al, 110b			; HL code
	je			instruction_swap_HL

	; SPECIFIC 8 BIT REGISTER
	call		get_register_8bit	; eax <- &m
	mov word	dx, [eax]			; Save address into edx

	mov byte	dh, dl

	; dh <- ([eax] & 0xF0) >> 4
	and byte	dh, 0xF0
	shr byte	dh, 4

	; dl <- ([eax] & 0x0F) << 4
	and byte	dl, 0x0F
	shl byte	dl, 4

	; dl <- dl | dh
	or byte	dl, dh

	; [eax] <- dl
	mov byte	[eax], dl

	mov	byte	al, [eax]
	call		set_flag_z8

	; Save flags
	mov dword	ebx, 0
	mov dword	ecx, 0
	call		change_flag
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

instruction_swap_HL :

	; Get address
	mov dword	eax, 0x0
	mov byte	ah, [edi + 0x6]
	mov byte	al, [edi + 0x7]

	; Load real address on stack
	call		get_real_address
	mov dword	edx, 0x0
	mov byte	dl, [eax]
	mov byte	dh, dl

	; dh <- ([eax] & 0xF0) >> 4
	and byte	dh, 0xF0
	shr byte	dh, 4

	; dl <- ([eax] & 0x0F) << 4
	and byte	dl, 0x0F
	shl byte	dl, 4

	; dl <- dl | dh
	or byte	dl, dh

	; Set value at [eax]
	mov	byte	[eax], dl

	; Set Z flag
	mov	byte	al, dl
	call		set_flag_z8

	; Save flags
	mov dword	ebx, 0
	mov dword	ecx, 0
	call		change_flag
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