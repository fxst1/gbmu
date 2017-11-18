;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; PUSH	qq | 11 qq0 101 | P90

instruction_push :

	push dword	ecx

	; Get qq register into eax
	mov dword	eax, 0x0
	mov byte	al, [edx + ecx]
	mov dword	ebx, 2
	mov dword	ecx, 5
	call		get_register_code
	call		get_register_16bit_2

	push dword	eax

	; Get real stack address into ebx
	call		get_stack_top
	mov dword	ebx, eax
	pop dword	eax

	; Store qq register content into address at SP - 1 and SP - 2
	mov dword	ecx, [eax]
	mov byte	[ebx + 0x1], cl
	mov byte	[ebx + 0x2], ch
	sub word	[edi + 0x9], 0x2

	pop dword	ecx
	inc dword	ecx

; POP	qq | 11 qq0 001 | P91

instruction_pop :

	push dword	ecx

	; Get qq register into eax
	mov dword	eax, 0x0
	mov byte	al, [edx + ecx]
	mov dword	ebx, 2
	mov dword	ecx, 5
	call		get_register_code
	call		get_register_16bit_2

	push dword	eax

	; Get real stack address into ebx
	call		get_stack_top
	mov dword	ebx, eax
	pop dword	eax


	; Store qq register content into address at SP - 1 and SP - 2
	mov dword	ecx, [eax]
	mov byte	cl, [ebx + 0x1]
	mov byte	ch, [ebx + 0x2]
	add word	[edi + 0x9], 0x2

	mov byte	[eax], cl
	mov byte	[eax + 0x1], ch

	pop dword	ecx
	inc dword	ecx
