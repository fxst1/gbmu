%ifndef	ASM_BITOP
%define ASM_BITOP
%include 'src/cpu/register.s'
%include 'src/cpu/instruction.s'

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; BIT	b, r | 11 001 011 - 01 b r | P103
instruction_bit :

	inc qword	rcx
	push qword	rcx

	; Get r register (eax <- r)
	mov qword	rax, 0x0
	mov qword	rax, [rdx + rcx]	; opcode
	mov qword	rbx, 3				; 3 bits
	mov qword	rcx, 2				; @ 3th bit
	call		get_register_code

	cmp byte	al, 110b
	je			instruction_bit_HL

	; SPECIFIC 8 BIT REGISTER
	call		get_register_8bit	; eax <- &m
	mov byte	bl, [rax]			; Save address into bl

	; Get b value (eax <- b)
	mov qword	rcx, [rsp]
	mov qword	rax, [rdx + rcx]
	push qword	rbx
	mov qword	rbx, 3
	mov qword	rcx, 5
	call		get_register_code
	pop qword	rbx

	; dh = b, dl = [r]
	mov byte	bh, al

	; get bit at b
	mov byte	cl, bh

	; Z = ((!bl) >> b & 1)
	not byte	bl
	shr byte	bl, cl
	and byte	bl, 1

	; Set Z flag
	mov qword	rcx, 3
	call		change_flag

	; Save flags
	mov qword	rbx, 1
	mov qword	rcx, 1
	call		change_flag
	mov qword	rbx, 0
	mov qword	rcx, 2
	call		change_flag

	pop qword	rcx
	inc qword	rcx

	jmp			instruction_loop

instruction_bit_HL :
	
	; Get address
	mov qword	rax, 0x0
	mov byte	ah, [rdi + 0x6]
	mov byte	al, [rdi + 0x7]

	; Load real address on stack
	call		get_real_address
	mov qword	rbx, 0x0
	mov byte	bl, [rax]

	; Get b value (eax <- b)
	mov qword	rcx, [rsp]
	mov qword	rax, [rdx + rcx]
	push qword	rbx
	mov qword	rbx, 3
	mov qword	rcx, 5
	call		get_register_code
	pop qword	rbx

	; bh = b, bl = [r]
	mov byte	bh, al

	; Z = ((!bl) >> b & 1)
	mov byte	cl, bh
	not byte	bl
	shr byte	bl, cl
	and byte	bl, 1

	; Set Z flag
	mov qword	rcx, 3
	call		change_flag

	; Save flags
	mov qword	rbx, 1
	mov qword	rcx, 1
	call		change_flag
	mov qword	rbx, 0
	mov qword	rcx, 2
	call		change_flag

	pop qword	rcx
	inc qword	rcx

	jmp			instruction_loop

; SET	b, r | 11 001 011 - 11 b r | P103 - P104

instruction_set :

	inc qword	rcx
	push qword	rcx

	; Get b value (eax <- b)
	mov qword	rax, 0x0
	mov qword	rax, [rdx + rcx]	; opcode
	mov qword	rbx, 3				; 3 bits
	mov qword	rcx, 5				; @ 3th bit
	call		get_register_code

	; bh = b, bl = [r]
	mov byte	bh, al

	; Get r register (eax <- r)
	mov qword	rcx, [rsp]
	mov qword	rax, [rdx + rcx]
	push qword	rbx
	mov qword	rbx, 3
	mov qword	rcx, 2
	call		get_register_code
	pop qword	rbx

	cmp byte	al, 110b
	je			instruction_set_HL

	; SPECIFIC 8 BIT REGISTER
	call		get_register_8bit	; eax <- &m

	; r <- r | (1 << b)
	push qword	rdx
	mov qword	rdx, 1
	mov byte	cl, bh
	shl byte	dl, cl
	or byte		[rax], dl
	pop qword	rdx

	pop qword	rcx
	inc qword	rcx

	jmp			instruction_loop

instruction_set_HL :

	; Get address
	mov qword	rax, 0x0
	mov byte	ah, [rdi + 0x6]
	mov byte	al, [rdi + 0x7]

	; Load real address on stack
	call		get_real_address

	; r <- r | (1 << b)
	push qword	rdx
	mov qword	rdx, 1
	mov byte	cl, bh
	shl byte	dl, cl
	or byte		[rax], dl
	pop qword	rdx

	pop qword	rcx
	inc qword	rcx

	jmp			instruction_loop


; RES	b, r | 11 001 011 - 10 b r | P104

instruction_res :

	inc qword	rcx
	push qword	rcx

	; Get b value (eax <- b)
	mov qword	rax, 0x0
	mov qword	rax, [rdx + rcx]	; opcode
	mov qword	rbx, 3				; 3 bits
	mov qword	rcx, 5				; @ 3th bit
	call		get_register_code

	; bh = b, bl = [r]
	mov byte	bh, al

	; Get r register (eax <- r)
	mov qword	rcx, [rsp]
	mov qword	rax, [rdx + rcx]
	push qword	rbx
	mov qword	rbx, 3
	mov qword	rcx, 2
	call		get_register_code
	pop qword	rbx

	cmp byte	al, 110b
	je			instruction_res_HL

	; SPECIFIC 8 BIT REGISTER
	call		get_register_8bit	; eax <- &m

	; r <- r & (~(1 << b))
	push qword	rdx
	mov qword	rdx, 1
	mov byte	cl, bh
	shl byte	dl, cl
	not byte	dl
	and byte	[rax], dl
	pop qword	rdx

	pop qword	rcx
	inc qword	rcx

	jmp			instruction_loop


instruction_res_HL :

	; Get address
	mov qword	rax, 0x0
	mov byte	ah, [rdi + 0x6]
	mov byte	al, [rdi + 0x7]

	; Load real address on stack
	call		get_real_address

	; r <- r & (~(1 << b))
	push qword	rdx
	mov qword	rdx, 1
	mov byte	cl, bh
	shl byte	dl, cl
	not byte	dl
	and byte	[rax], dl
	pop qword	rdx

	pop qword	rcx
	inc qword	rcx

	jmp			instruction_loop

%endif