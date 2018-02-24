%ifndef	ASM_ADD
%define ASM_ADD
%include 'src/cpu/flag.s'
%include 'src/cpu/register.s'
%include 'src/cpu/instruction.s'

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; ADD	A, r | 10 000 r | P92

instruction_add_8_1 :

	push qword		rcx

	mov qword	rax, [rdx + rcx]
	mov qword	rbx, 0x3
	mov qword	rcx, 0x2
	call			get_register_code

	cmp byte		al, 110b
	je				instruction_add_8_1_HL

	call			get_register_8bit

	mov byte		al, [rax]
	mov byte		ah, 0x0
	add	byte		al, [rdi]

	call			set_flag_cy16
	call			set_flag_h16
	call			set_flag_z16

	mov qword	rbx, 0x0
	mov qword	rcx, 2
	call			change_flag

	mov byte		[rdi], al

	pop qword		rcx
	inc qword	rcx

	jmp				instruction_loop

instruction_add_8_1_HL :

	mov qword	rax, 0x0
	mov byte		ah, [rdi + 0x6]
	mov byte		al, [rdi + 0x7]
	call			get_real_address

	mov byte		al, [rax]
	mov byte		ah, 0x0
	add	byte		al, [rdi]

	call			set_flag_cy16
	call			set_flag_h16
	call			set_flag_z16

	mov qword	rbx, 0x0
	mov qword	rcx, 2
	call			change_flag

	mov byte		[rdi], al

	pop qword		rcx
	inc qword	rcx

	jmp				instruction_loop

; ADD	A, n | 11 000 110 - n | P92

instruction_add_8_2 :

	inc qword	rcx

	mov qword	rax, 0x0
	mov byte		al, [rdx + rcx]
	add	byte		al, [rdi]

	call			set_flag_cy16
	call			set_flag_h16
	call			set_flag_z16

	push qword		rcx
	mov qword	rbx, 0x0
	mov qword	rcx, 2
	call			change_flag
	pop qword		rcx

	mov byte		[rdi], al

	inc qword	rcx

	jmp				instruction_loop


; ADC	A, r | 10 001 r | P92

instruction_adc_1 :

	push qword		rcx

	mov qword	rax, [rdx + rcx]
	mov qword	rbx, 0x3
	mov qword	rcx, 0x2
	call			get_register_code

	cmp byte		al, 110b
	je				instruction_adc_1_HL

	call			get_register_8bit

	push qword		rax
	call			get_flag_cy
	mov qword		rbx, rax
	pop qword		rax

	mov byte		ah, [rax]
	mov byte		ah, 0x0
	add	byte		al, bl
	add	byte		al, [rdi]

	call			set_flag_cy16
	call			set_flag_h16
	call			set_flag_z16

	mov qword	rbx, 0x0
	mov qword	rcx, 2
	call			change_flag

	mov byte		[rdi], al

	pop qword		rcx
	inc qword	rcx

	jmp				instruction_loop

instruction_adc_1_HL :

	mov qword		rax, 0x0
	mov byte		ah, [rdi + 0x6]
	mov byte		al, [rdi + 0x7]
	call			get_real_address

	push qword		rax
	call			get_flag_cy
	mov qword		rbx, rax
	pop qword		rax

	mov byte		ah, [rax]
	mov byte		ah, 0x0
	add	byte		al, bl
	add	byte		al, [rdi]

	call			set_flag_cy16
	call			set_flag_h16
	call			set_flag_z16

	mov qword		rbx, 0x0
	mov qword		rcx, 2
	call			change_flag

	mov byte		[rdi], al

	pop qword		rcx
	inc qword		rcx

	jmp				instruction_loop

; ADC	A, n | 11 001 110 - n | P92

instruction_adc_2 :

	inc qword		rcx
	push qword		rcx

	call			get_flag_cy
	mov qword		rbx, rax

	mov qword		rax, 0x0
	mov byte		al, [rdi + rcx]

	mov qword		rcx, 0x0
	mov byte		cl, [rdi]
	add	byte		cl, al
	add	byte		cl, bl
	mov	word		ax, cx

	call			set_flag_cy16
	call			set_flag_h16
	call			set_flag_z16

	mov qword		rbx, 0x0
	mov qword		rcx, 2
	call			change_flag

	mov byte		[rdi], al

	pop qword		rcx
	inc qword		rcx

	jmp				instruction_loop


;;;;;;;;;;;;;;;;;;;;;;;
; ADD	HL, ss | 00 ss1 001 | P97

instruction_add_16_1 :

	push qword		rcx

	mov qword		rax, [rdx + rcx]
	mov qword		rbx, 0x3
	mov qword		rcx, 0x5
	call			get_register_code
	call			get_register_16bit

	mov word		ax, [rax]
	add	word		ax, [rdi + 0x6]

	call			set_flag_cy16
	call			set_flag_h16

	mov qword		rbx, 0x0
	mov qword		rcx, 1
	call			change_flag

	mov word		[rdi + 0x6], ax

	pop qword		rcx
	inc qword		rcx

	jmp				instruction_loop

; ADD	SP, e | 00 101 000 - e | P97

instruction_add_16_2 :

	inc qword		rcx
	push qword		rcx

	mov qword		rax, [rdx + rcx]

	mov word		ax, [rax]
	add	word		ax, [rdi + 0xA]

	call			set_flag_cy16
	call			set_flag_h16

	mov qword		rbx, 0x0
	mov qword		rcx, 0x1
	call			change_flag

	mov qword		rbx, 0x0
	mov qword		rcx, 0x0
	call			change_flag

	mov word		[rdi + 0xA], ax

	pop qword		rcx
	inc qword		rcx

	jmp				instruction_loop

%endif