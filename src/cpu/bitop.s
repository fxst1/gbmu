;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; BIT	b, r | 11 001 011 - 01 b r | P103
instruction_bit :

	inc dword	ecx
	push dword	ecx

	; Get r register (eax <- r)
	mov dword	eax, 0x0
	mov dword	eax, [edx + ecx]	; opcode
	mov dword	ebx, 3				; 3 bits
	mov dword	ecx, 2				; @ 3th bit
	call		get_register_code

	cmp byte	al, 110b
	je			instruction_bit_HL

	; SPECIFIC 8 BIT REGISTER
	call		get_register_8bit	; eax <- &m
	mov byte	bl, [eax]			; Save address into bl

	; Get b value (eax <- b)
	mov dword	ecx, [esp]
	mov dword	eax, [edx + ecx]
	push dword	ebx
	mov dword	ebx, 3
	mov dword	ecx, 5
	call		get_register_code
	pop dword	ebx

	; dh = b, dl = [r]
	mov byte	bh, al

	; get bit at b
	mov byte	cl, bh

	; Z = ((!bl) >> b & 1)
	not byte	bl
	shr byte	bl, cl
	and byte	bl, 1

	; Set Z flag
	mov dword	ecx, 3
	call		change_flag

	; Save flags
	mov dword	ebx, 1
	mov dword	ecx, 1
	call		change_flag
	mov dword	ebx, 0
	mov dword	ecx, 2
	call		change_flag

	pop dword	ecx
	inc dword	ecx

	jmp			instruction_loop

instruction_bit_HL :
	
	; Get address
	mov dword	eax, 0x0
	mov byte	ah, [edi + 0x6]
	mov byte	al, [edi + 0x7]

	; Load real address on stack
	call		get_real_address
	mov dword	ebx, 0x0
	mov byte	bl, [eax]

	; Get b value (eax <- b)
	mov dword	ecx, [esp]
	mov dword	eax, [edx + ecx]
	push dword	ebx
	mov dword	ebx, 3
	mov dword	ecx, 5
	call		get_register_code
	pop dword	ebx

	; bh = b, bl = [r]
	mov byte	bh, al

	; Z = ((!bl) >> b & 1)
	mov byte	cl, bh
	not byte	bl
	shr byte	bl, cl
	and byte	bl, 1

	; Set Z flag
	mov dword	ecx, 3
	call		change_flag

	; Save flags
	mov dword	ebx, 1
	mov dword	ecx, 1
	call		change_flag
	mov dword	ebx, 0
	mov dword	ecx, 2
	call		change_flag

	pop dword	ecx
	inc dword	ecx

	jmp			instruction_loop

; SET	b, r | 11 001 011 - 11 b r | P103 - P104

instruction_set :

	inc dword	ecx
	push dword	ecx

	; Get b value (eax <- b)
	mov dword	eax, 0x0
	mov dword	eax, [edx + ecx]	; opcode
	mov dword	ebx, 3				; 3 bits
	mov dword	ecx, 5				; @ 3th bit
	call		get_register_code

	; bh = b, bl = [r]
	mov byte	bh, al

	; Get r register (eax <- r)
	mov dword	ecx, [esp]
	mov dword	eax, [edx + ecx]
	push dword	ebx
	mov dword	ebx, 3
	mov dword	ecx, 2
	call		get_register_code
	pop dword	ebx

	cmp byte	al, 110b
	je			instruction_set_HL

	; SPECIFIC 8 BIT REGISTER
	call		get_register_8bit	; eax <- &m

	; r <- r | (1 << b)
	push dword	edx
	mov dword	edx, 1
	mov byte	cl, bh
	shl byte	dl, cl
	or byte		[eax], dl
	pop dword	edx

	pop dword	ecx
	inc dword	ecx

	jmp			instruction_loop

instruction_set_HL :

	; Get address
	mov dword	eax, 0x0
	mov byte	ah, [edi + 0x6]
	mov byte	al, [edi + 0x7]

	; Load real address on stack
	call		get_real_address

	; r <- r | (1 << b)
	push dword	edx
	mov dword	edx, 1
	mov byte	cl, bh
	shl byte	dl, cl
	or byte		[eax], dl
	pop dword	edx

	pop dword	ecx
	inc dword	ecx

	jmp			instruction_loop


; RES	b, r | 11 001 011 - 10 b r | P104

instruction_res :

	inc dword	ecx
	push dword	ecx

	; Get b value (eax <- b)
	mov dword	eax, 0x0
	mov dword	eax, [edx + ecx]	; opcode
	mov dword	ebx, 3				; 3 bits
	mov dword	ecx, 5				; @ 3th bit
	call		get_register_code

	; bh = b, bl = [r]
	mov byte	bh, al

	; Get r register (eax <- r)
	mov dword	ecx, [esp]
	mov dword	eax, [edx + ecx]
	push dword	ebx
	mov dword	ebx, 3
	mov dword	ecx, 2
	call		get_register_code
	pop dword	ebx

	cmp byte	al, 110b
	je			instruction_res_HL

	; SPECIFIC 8 BIT REGISTER
	call		get_register_8bit	; eax <- &m

	; r <- r & (~(1 << b))
	push dword	edx
	mov dword	edx, 1
	mov byte	cl, bh
	shl byte	dl, cl
	not byte	dl
	and byte	[eax], dl
	pop dword	edx

	pop dword	ecx
	inc dword	ecx

	jmp			instruction_loop


instruction_res_HL :

	; Get address
	mov dword	eax, 0x0
	mov byte	ah, [edi + 0x6]
	mov byte	al, [edi + 0x7]

	; Load real address on stack
	call		get_real_address

	; r <- r & (~(1 << b))
	push dword	edx
	mov dword	edx, 1
	mov byte	cl, bh
	shl byte	dl, cl
	not byte	dl
	and byte	[eax], dl
	pop dword	edx

	pop dword	ecx
	inc dword	ecx

	jmp			instruction_loop
