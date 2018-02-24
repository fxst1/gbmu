%ifndef	ASM_INC
%define ASM_INC
%include 'src/cpu/register.s'
%include 'src/cpu/instruction.s'

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; INC	ss | 00 ss0 011 | P97
instruction_inc16:

	push qword	rcx

	; Get ss register 16bit code
	xor qword	rax, rax
	mov qword	rax, [rdx + rcx]	; opcode
	mov qword	rbx, 2				; code length
	mov qword	rcx, 5				; index high bit
	call		get_register_code	; eax <- ss

	pop qword	rcx					; Restore buffer index
	push qword	rax					; Save register code

	; Code is correct ?
	;call		is_register_16bit
	;cmp byte	al, 0b
	;je			error				; Code not exist => EXIT

	pop qword	rax
	call		get_register_16bit	; eax <- register address

	; bx <- ss
	mov byte	bh, [rax]			; Get sh value
	mov byte	bl, [rax + 0x1]		; Get sl value

	; bx++
	inc word	bx

	; ss <- bx
	mov byte	[rax], bh			; sh <- bh
	mov byte	[rax + 0x1], bl		; sl <- bl

	inc qword	rcx

	jmp			instruction_loop

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; INC	r | 00 r 100 | P95
instruction_inc8:

	push qword	rcx

	; Get ss register 16bit code
	xor qword	rax, rax
	mov qword	rax, [rdx + rcx]	; opcode
	mov qword	rbx, 2				; code length
	mov qword	rcx, 5				; index high bit
	call		get_register_code	; eax <- ss

	cmp byte	al, 110b
	je			instruction_inc8_HL

	call		get_register_8bit	; eax <- register address

	inc byte	[rax]
	mov byte	al, [rax]
	mov byte	ah, 0x0

	call		set_flag_h16
	call		set_flag_z16

	mov qword	rbx, 0x0
	mov qword	rcx, 0x1
	call		change_flag

	pop qword	rcx
	inc qword	rcx

	jmp			instruction_loop

instruction_inc8_HL :

	mov qword	rax, 0x0
	mov word	ax, [rdi + 0x6]
	call		get_real_address

	inc byte	[rax]
	mov byte	al, [rax]
	mov byte	ah, 0x0

	call		set_flag_h16
	call		set_flag_z16

	mov qword	rbx, 0x0
	mov qword	rcx, 0x1
	call		change_flag

	pop qword	rcx
	inc qword	rcx

	jmp			instruction_loop

%endif