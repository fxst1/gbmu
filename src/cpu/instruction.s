;Page 84 / 268
;
;	Execute a Gb program
;
;	c : index
;	d : buffer
;
;	0x8000 - 0x150 = 7EB0 (32Kb)
;

%ifndef	ASM_INSTR
%define ASM_INSTR
%include 'src/cpu/flag.s'
%include 'src/cpu/register.s'
%include 'src/cpu/print.s'

%include 'src/cpu/function.s'
%include 'src/cpu/arth.s'
%include 'src/cpu/add.s'
%include 'src/cpu/sub.s'
%include 'src/cpu/ctrl.s'
%include 'src/cpu/add.s'
%include 'src/cpu/inc.s'
%include 'src/cpu/dec.s'
%include 'src/cpu/rotate.s'
%include 'src/cpu/bitop.s'
%include 'src/cpu/swap.s'
%include 'src/cpu/push_pop.s'
%include 'src/cpu/shift.s'
%include 'src/cpu/ld.s'
%include 'src/cpu/jmp.s'
%include 'src/cpu/ret_call.s'

instruction_loop_start:

	push qword	rbp
	mov qword	rbp, rsp

	jmp			instruction_request

instruction_jmp_loop :

	mov qword	rcx, 0x0
	mov word	cx, [rdi + 0x8]

	jmp			instruction_request

instruction_loop :

	mov word	[rdi + 0x8], cx

instruction_request :

	call	print

	push qword	rcx
	push qword	rdx
	push qword	rsi
	push qword	rdi

	mov qword	r12, rsi

	call		requestcpu WRT ..plt

	pop qword	rdi
	pop qword	rsi
	pop qword	rdx
	pop qword	rcx

	cmp qword	rax, 0x1
	je			instruction_request

instruction_loop_parse :

	cmp qword	rcx, 0x8000
	jge			instruction_loop_parse_3

	mov qword	rbx, [rdx + rcx]	;get current instruction

	push qword	rdi
	push qword	rsi
	push qword	rdx
	push qword	rcx

	; 2 bytes code
	mov dword	eax, 0x0
	mov word	ax, [rdx + rcx]

	push qword	rdi
	push qword	rsi
	mov qword	rdi, 2
	mov qword	rsi, rax
	call		codecpu WRT ..plt
	pop qword	rsi
	pop qword	rdi

	pop qword	rcx
	pop qword	rdx
	pop qword	rsi
	pop qword	rdi

	cmp qword	rax, 0
	je			instruction_loop_parse_2
	jmp			rax

instruction_loop_parse_2 :

	push qword	rdi
	push qword	rsi
	push qword	rdx
	push qword	rcx

	; 2 bytes code
	mov dword	eax, 0x0
	mov word	ax, [rdx + rcx]

	push qword	rdi
	push qword	rsi
	mov qword	rdi, 1
	mov qword	rsi, rax
	call		codecpu WRT ..plt
	pop qword	rsi
	pop qword	rdi

	pop qword	rcx
	pop qword	rdx
	pop qword	rsi
	pop qword	rdi

	cmp qword	rax, 0
	je			instruction_loop_parse_3
	jmp			rax

instruction_loop_parse_3 :

	leave
	ret


%endif