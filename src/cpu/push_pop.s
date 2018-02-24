%ifndef	ASM_PUSH
%define ASM_PUSH
%include 'src/cpu/register.s'
%include 'src/cpu/instruction.s'

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; PUSH	qq | 11 qq0 101 | P90

instruction_push :

	push qword	rcx

	; Get qq register into eax
	mov qword	rax, 0x0
	mov byte	al, [rdx + rcx]
	mov qword	rbx, 2
	mov qword	rcx, 5
	call		get_register_code
	call		get_register_16bit_2

	push qword	rax

	; Get real stack address into ebx
	call		get_stack_top
	mov qword	rbx, rax
	pop qword	rax

	; Store qq register content into address at SP - 1 and SP - 2
	mov qword	rcx, [rax]
	mov byte	[rbx + 0x1], cl
	mov byte	[rbx + 0x2], ch
	sub word	[rdi + 0x9], 0x2

	pop qword	rcx
	inc qword	rcx

; POP	qq | 11 qq0 001 | P91

instruction_pop :

	push qword	rcx

	; Get qq register into eax
	mov qword	rax, 0x0
	mov byte	al, [rdx + rcx]
	mov qword	rbx, 2
	mov qword	rcx, 5
	call		get_register_code
	call		get_register_16bit_2

	push qword	rax

	; Get real stack address into ebx
	call		get_stack_top
	mov qword	rbx, rax
	pop qword	rax


	; Store qq register content into address at SP - 1 and SP - 2
	mov qword	rcx, [rax]
	mov byte	cl, [rbx + 0x1]
	mov byte	ch, [rbx + 0x2]
	add word	[rdi + 0x9], 0x2

	mov byte	[rax], cl
	mov byte	[rax + 0x1], ch

	pop qword	rcx
	inc qword	rcx

%endif