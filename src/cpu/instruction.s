;Page 84 / 268
;
;	Execute a Gb program
;	size_t	instruction(unsigned char*, size_t len);
;
;	c : index
;	d : buffer
;

%include 'register.s'
;%include 'add.s'
%include 'inc.s'
%include 'dec.s'
;%include 'rotate.s'

instruction :

	push dword	ebp
	mov dword	ebp, esp

	mov dword	edx, ebp			;ax = buffer
	mov dword	ecx, [ebp + 8]		;cx = len

	xor dword	edi, edi
	;call		initialize_registers

instruction_loop:

	cmp dword	edx, ecx
	je			instruction_end
	mov dword	ebx, [edx + 0x0]	;get current instruction
	inc dword	edx

	; ld
	cmp	byte	bl, 00110110b
	je			instruction_ld8_1
	cmp byte	bl, 00001010b
	je			instruction_ld8_2
	cmp byte	bl, 00011010b
	je			instruction_ld8_3

	; add
	cmp byte	bl, 11000110b
	je			instruction_add8_2
	cmp byte	bl, 10000110b
	je			instruction_add8_3
	cmp byte	bl, 11101000b
	je			instruction_add16_2

	jmp			instruction_end


;LD		r, r'
instruction_ld8_1:

	push dword	eax
	push dword	ebx
	push dword	ecx
	xor dword	eax, eax
	xor dword	ecx, ecx

	; get src register value and save it
	xor byte	al, al
	mov	dword	eax, edx
	mov byte	cl, 2
	call		get_register_8bit
	push dword	eax

	; get dest register code
	xor byte	al, al
	mov	dword	eax, edx
	mov byte	cl, 5
	call		get_register_code

	; set dest register to src
	mov byte	bl, al
	pop dword	eax
	call		set_register_8bit

	pop dword	ecx
	pop dword	ebx
	pop dword	eax

	jmp			instruction_loop

;LD		r, n
instruction_ld8_2:

	push dword	eax
	push dword	ebx
	push dword	ecx
	xor dword	eax, eax
	xor dword	ecx, ecx

	; get value from next byte
	mov	dword	edx, [edx + 1]
	push dword	eax

	; get dest register code
	xor byte	al, al
	mov	dword	eax, edx
	mov byte	cl, 5
	call		get_register_code

	; set dest register to value
	mov byte	bl, al
	pop dword	eax
	call		set_register_8bit

	pop dword	ecx
	pop dword	ebx
	pop dword	eax

	inc dword	ecx

	jmp			instruction_loop

;LD		r, (HL)
instruction_ld8_3:

	push dword	eax
	push dword	ebx
	push dword	ecx
	xor dword	eax, eax
	xor dword	ecx, ecx

	; push (HL)
	call		load_register_HL
	push dword	eax

	; get dest register code
	xor byte	al, al
	mov	dword	eax, edx
	mov byte	cl, 5
	call		get_register_code

	; set dest register to value
	mov byte	bl, al
	pop dword	eax
	call		set_register_8bit

	pop dword	ecx
	pop dword	ebx
	pop dword	eax

	inc dword	ecx

	jmp			instruction_loop

;LD		(HL), r
instruction_ld8_4:

	push dword	eax
	push dword	ebx
	push dword	ecx
	xor dword	eax, eax
	xor dword	ecx, ecx

	; get src register value
	xor byte	al, al
	mov	dword	eax, edx
	mov byte	cl, 2
	call		get_register_8bit

	; set HL register to src
	xor byte	ah, ah
	call		save_register_HL

	pop dword	ecx
	pop dword	ebx
	pop dword	eax

	add dword	ecx, 2

	jmp			instruction_loop

;LD		(HL), r
instruction_ld8_5:

	push dword	eax
	push dword	ebx
	push dword	ecx
	xor dword	eax, eax
	xor dword	ecx, ecx

	; get src register value
	xor byte	al, al
	mov	dword	eax, edx
	mov byte	cl, 2
	call		get_register_8bit

	; set HL register to src
	xor byte	ah, ah
	call		save_register_HL

	pop dword	ecx
	pop dword	ebx
	pop dword	eax

	add dword	ecx, 2

	jmp			instruction_loop

;LD		(HL), n
instruction_ld8_6:

	push dword	eax
	push dword	ebx
	push dword	ecx
	xor dword	eax, eax
	xor dword	ecx, ecx

	; get src value
	mov	dword	edx, [edx + 1]

	; set HL register to src
	xor byte	ah, ah
	call		save_register_HL

	pop dword	ecx
	pop dword	ebx
	pop dword	eax

	add dword	ecx, 2

	jmp			instruction_loop

;LD		A, (BC)
instruction_ld8_7:

	lea dword	edi, [edi + 0x10]
	inc dword	ecx

	jmp			instruction_loop


;LD		A, (DE)
instruction_ld8_8:

	lea dword	edi, [edi + 0x20]
	inc dword	ecx

	jmp			instruction_loop

;LD		A, (C)
instruction_ld8_9:

	lea dword	edi, [edi + 0x18]
	inc dword	ecx

	jmp			instruction_loop

;LD		(C), A
instruction_ld8_10:

	push dword	eax

	mov dword	eax, edi
	add dword	eax, 0x18
	lea dword	eax, [edi + 0x0]
	inc dword	ecx

	pop dword	eax

	jmp			instruction_loop

;LD		A, (n)
instruction_ld8_11:

	push dword	eax

	mov dword	eax, 0x60
	add dword	eax, [edx + 1]

	lea	dword	edi, [edi + eax]
	add	dword	ecx, 2

	pop dword	eax

	jmp			instruction_loop

;LD		(n), A
instruction_ld8_12:

	push dword	eax

	mov dword	eax, 0x60
	add dword	eax, [edi + 1]
	lea	dword	eax, [edi + 0x0]
	add	dword	ecx, 2

	pop dword	eax
	jmp			instruction_loop

;LD		A, (nn)
instruction_ld8_13:

	push dword	eax

	xor dword	eax, eax

	; get 2 next bytes from buffer
	mov word	ax, [edx + 1]

	; load it from ram
	add dword	eax, 0x60
	lea dword	edi, [edi + eax]

	pop dword	eax
	add dword	ecx, 3

	jmp			instruction_loop

;LD		(nn), A
instruction_ld8_14:

	push dword	eax
	xor dword	eax, eax

	; get 2 next bytes from buffer
	mov word	ax, [edx + 1]

	; save into it
	add dword	eax, 0x60
	add dword	eax, edi
	lea dword	eax, [edi + 0x0]

	pop dword	eax
	add dword	ecx, 3

	jmp			instruction_loop

;LD		A, (nn)
instruction_ld8_15:

	push dword	eax
	push dword	ebx

	; get first byte from buffer and save it
	mov dword	ecx, [edi + 1]

	; get second byte from buffer
	mov dword	eax, [edi + 1]

	; create address
	mov byte	ah, cl

	; add ram offset and load it from ram
	add dword	eax, edi
	lea dword	edi, [eax + 0x0]

	pop dword	eax
	pop dword	ebx
	add dword	ecx, 3

	jmp			instruction_loop

;LD		A, (HLI)
instruction_ld8_16:

	push dword	eax
	push dword	ebx

	; get first byte from buffer and save it
	mov dword	ecx, [edi + 1]

	; get second byte from buffer
	mov dword	eax, [edi + 1]

	; create address
	mov byte	ah, cl

	; add ram offset and load it from ram
	lea dword	edi, [edi, eax]

	pop dword	eax
	pop dword	ebx
	add dword	ecx, 3

	jmp			instruction_loop

;LD		A, (HLI)
instruction_ld8_17:

	; save into A and HL++ + 0x0
	lea dword	edi, [edi + 0x30]
	inc	word	[edi + 0x30]
	inc dword	ecx

	jmp			instruction_loop
	
;LD		A, (HLD)
instruction_ld8_18:

	lea dword	edi, [edi + 0x30]
	dec	word	[edi + 0x30]
	inc dword	ecx

	jmp			instruction_loop

;LD		(BC), A
instruction_ld8_19:

	push dword	eax

	add dword	eax, edx
	add dword	eax, 0x30

	lea dword	eax, [edi + 0x0]
	inc dword	ecx

	pop dword	eax

	jmp			instruction_loop

;LD		(DE), A
instruction_ld8_20:

	push dword	eax

	add dword	eax, edx
	add dword	eax, 0x30

	lea dword	eax, [edi + 0x0]
	inc dword	ecx

	pop dword	eax

	jmp			instruction_loop


;LD		(HLI), A
instruction_ld8_21:

	push dword	eax

	add dword	eax, edx
	add dword	eax, 0x30

	; save into A and HL++
	lea dword	eax, [edi + 0x0]
	inc	word	[edi + 0x30]
	inc dword	ecx

	pop dword	eax

	jmp			instruction_loop

;LD		(HLD), A
instruction_ld8_22:

	push dword	eax

	add dword	eax, edx
	add dword	eax, 0x30

	lea dword	eax, [edi + 0x0]
	dec	word	[edi + 0x30]
	inc dword	ecx

	pop dword	eax
	jmp			instruction_loop


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;ADD	A, r
instruction_add8_1:

	jmp			instruction_loop

;ADD	A, n
instruction_add8_2:

	; al <- n
	mov byte	al, [edx + ecx]

	; al += A
	add byte	al, [edi]

	; A <- al
	mov byte	[edi], al

	inc dword	ecx

	jmp			instruction_loop

;ADD	A, (HL)
instruction_add8_3:

	; ax <- (HL)
	mov byte	ah, [edi + 0x30]
	mov byte	al, [edi + 0x38]

	; ax += A
	add word	ax, [edi]

	; A <- ah
	mov byte	[edi], ah

	inc dword	ecx

	jmp			instruction_loop

;ADD	HL, ss | 00ss1001
instruction_add16_1:

	push dword	ecx

	; Get ss register 16bit code
	xor dword	eax, eax
	mov dword	eax, [edx + ecx]	; opcode
	mov dword	ebx, 2				; code length
	mov dword	ecx, 5				; index high bit
	call		get_register_code	; eax <- ss

	pop dword	ecx					; Restore buffer index
	push dword	eax					; Save register code

	; Code is correct ?
	;call		is_register_16bit
	;cmp byte	al, 0b
	;je			error				; Code not exist => EXIT

	pop dword	eax
	call		get_register_16bit	; eax <- register address

	; bx <- ss
	mov byte	bh, [eax]			; Get sh value
	mov byte	bl, [eax + 0x8]		; Get sl value

	; bx += HL
	mov dword	eax, 0x0
	mov byte	ah, [edi + 0x30]
	mov byte	al, [edi + 0x38]
	add word	bx, ax

	; Save flags
	mov dword	eax, ebx
	call		set_flag_cy16		; Set CY flag
	call		set_flag_h16		; Set H flag

	; Set N flag to 0
	mov dword	ebx, 0
	mov dword	ecx, 2
	call		change_flag

	; HL <- bx
	mov byte	[edi + 0x30], ah	; H <- s0
	mov byte	[edi + 0x38], al	; L <- s1

	inc dword	ecx

	jmp			instruction_loop

;ADD	SP, e | 11 101 000 - e (8bit) | P97
instruction_add16_2:

	inc dword	ecx					; Go to next byte

	; Get e 8bit value
	xor dword	eax, eax
	mov byte	al, [edx + ecx]	; 8bit value

	; bx <- SP
	mov byte	bh, [edi + 0x50]	; Get SPh value
	mov byte	bl, [edi + 0x58]	; Get SPl value

	; bx += e
	add word	bx, ax

	; Save flags
	mov dword	eax, ebx
	call		set_flag_cy16		; Set CY flag
	call		set_flag_h16		; Set H flag

	; Set N flag to 0
	mov dword	ebx, 0
	mov dword	ecx, 2
	call		change_flag

	; Set Z flag to 0
	mov dword	ebx, 0
	mov dword	ecx, 3
	call		change_flag

	; HL <- bx
	mov byte	[edi + 0x50], ah	; SPh <- s0
	mov byte	[edi + 0x58], al	; SPl <- s1

	inc dword	ecx

	jmp			instruction_loop

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
instruction_end_pop:

	pop dword	ecx
	pop	dword	eax

instruction_end:

	mov dword	esp, ebp
	pop dword	ebp
	ret
