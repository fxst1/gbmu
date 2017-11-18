;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; INC	ss | 00 ss0 011 | P97
instruction_inc16:

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
	mov byte	bl, [eax + 0x1]		; Get sl value

	; bx++
	inc word	bx

	; ss <- bx
	mov byte	[eax], bh			; sh <- bh
	mov byte	[eax + 0x1], bl		; sl <- bl

	inc dword	ecx

	jmp			instruction_loop
