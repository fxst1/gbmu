;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; LD	dd, nn | 00 dd0 001 - n - n | P90

instruction_ld_16_1 :

	push dword	edx
	push dword	ecx

	; Get dd address register into eax
	mov dword	eax, 0x0
	mov dword	eax, [edx + ecx]	; opcode
	mov dword	ebx, 2				; 2 bits
	mov dword	ecx, 5				; @ 5th bit
	call		get_register_code
	call		get_register_16bit

	; Get nn value
	pop dword	ecx
	inc dword	ecx
	mov byte	bl, [edx + ecx]
	mov byte	[eax], bl
	inc dword	ecx
	mov byte	bl, [edx + ecx]
	mov byte	[eax + 0x1], bl

	pop dword	edx
	inc dword	ecx

	jmp			instruction_loop

; LD	SP, HL | 11 111 001 | P90

instruction_ld_16_2 :

	mov byte	al, [edi + 0x6]
	mov byte	ah, [edi + 0x7]
	mov byte	[edi + 0x9], ax
	inc dword	ecx
	jmp			instruction_loop

; LD	(nn), SP | 11 111 001 | P91

instruction_ld_16_3 :

	push dword	edx
	push dword	ecx

	; Get nn address into eax
	mov dword	eax, 0x0
	inc dword	ecx
	mov byte	al, [edx + ecx]
	inc dword	ecx
	mov byte	ah, [edx + ecx]

	; Store SP in [eax]
	call		get_real_address
	mov dword	ebx, 0x0
	mov byte	bx, [edi + 0x9]
	mov byte	[eax], bl
	mov byte	[eax + 0x1], bh

	inc dword	ecx

	jmp			instruction_loop
