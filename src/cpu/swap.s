%ifndef	ASM_SWAP
%define ASM_SWAP
%include 'src/cpu/register.s'
%include 'src/cpu/instruction.s'

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; SWAP	r | 11 001 011 - 00 110 r | P102
instruction_swap :

	inc qword	rcx
	push qword	rcx
	push qword	rdx

	; Get m register (eax <- code)
	mov qword	rax, [rdx + rcx]	; opcode
	mov qword	rbx, 3				; 3 bits
	mov qword	rcx, 2				; @ 3th bit
	call		get_register_code

	cmp byte	al, 110b			; HL code
	je			instruction_swap_HL

	; SPECIFIC 8 BIT REGISTER
	call		get_register_8bit	; eax <- &m
	mov word	dx, [rax]			; Save address into edx

	mov byte	dh, dl

	; dh <- ([rax] & 0xF0) >> 4
	and byte	dh, 0xF0
	shr byte	dh, 4

	; dl <- ([rax] & 0x0F) << 4
	and byte	dl, 0x0F
	shl byte	dl, 4

	; dl <- dl | dh
	or byte	dl, dh

	; [rax] <- dl
	mov byte	[rax], dl

	mov	byte	al, [rax]
	call		set_flag_z8

	; Save flags
	mov qword	rbx, 0
	mov qword	rcx, 0
	call		change_flag
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

instruction_swap_HL :

	; Get address
	mov qword	rax, 0x0
	mov byte	ah, [rdi + 0x6]
	mov byte	al, [rdi + 0x7]

	; Load real address on stack
	call		get_real_address
	mov qword	rdx, 0x0
	mov byte	dl, [rax]
	mov byte	dh, dl

	; dh <- ([rax] & 0xF0) >> 4
	and byte	dh, 0xF0
	shr byte	dh, 4

	; dl <- ([rax] & 0x0F) << 4
	and byte	dl, 0x0F
	shl byte	dl, 4

	; dl <- dl | dh
	or byte	dl, dh

	; Set value at [rax]
	mov	byte	[rax], dl

	; Set Z flag
	mov	byte	al, dl
	call		set_flag_z8

	; Save flags
	mov qword	rbx, 0
	mov qword	rcx, 0
	call		change_flag
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

%endif