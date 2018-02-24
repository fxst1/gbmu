%ifndef	ASM_FLAG
%define ASM_FLAG

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;	Set CY flag depend EAX
;
;	EAX : value
;
set_flag_cy16:

	push qword	rbp
	mov qword	rbp, rsp

	push qword	rcx
	push qword	rbx
	push qword	rax

	mov qword	rbx, 0b
	mov word	cx, 1b
	jc			set_flag_cy16_on
	jo			set_flag_cy16_on
	cmp word	ax, 0x4000
	jge			set_flag_cy16_on
	jmp			set_flag_cy16_end

set_flag_cy16_on:

	mov qword	rbx, 1b

set_flag_cy16_end:

	mov word	cx, 0b			; Flag bit index
	call		change_flag

	pop qword	rax
	pop qword	rbx
	pop qword	rcx

	leave
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;	Set H flag depend EAX
;
;	EAX : value
;
set_flag_h16:

	push qword	rbp
	mov qword	rbp, rsp

	push qword	rcx
	push qword	rbx
	push qword	rax

	mov qword	rbx, 0b
	ja			set_flag_h16_on
	cmp qword	rax, 0x400
	jge			set_flag_h16_on
	jmp			set_flag_h16_end

set_flag_h16_on:

	mov qword	rbx, 1b

set_flag_h16_end:

	mov word	cx, 1b			; Flag bit index
	call		change_flag

	pop qword	rax
	pop qword	rbx
	pop qword	rcx

	leave
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;	Set H flag depend AX
;
;	EAX : value
;
set_flag_z16:

	push qword	rbp
	mov qword	rbp, rsp

	push qword	rcx
	push qword	rbx
	push qword	rax

	mov qword	rbx, 0b
	jz			set_flag_z16_on
	cmp word	ax, 0x0
	je			set_flag_z16_on
	jmp			set_flag_z16_end

set_flag_z16_on:

	mov qword	rbx, 1b

set_flag_z16_end:

	mov word	cx, 3			; Flag bit index
	call		change_flag

	pop qword	rax
	pop qword	rbx
	pop qword	rcx

	leave
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;	Set H flag depend AL
;
;	EAX : value
;
set_flag_z8:

	push qword	rbp
	mov qword	rbp, rsp

	push qword	rcx
	push qword	rbx
	push qword	rax

	mov qword	rbx, 0b
	jz			set_flag_z16_on
	cmp byte	al, 0x0
	je			set_flag_z16_on
	jmp			set_flag_z16_end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;	Change flag bit
;
;	EBX : new bit value
;	ECX : bit offset
;
change_flag:

	push qword	rbp
	mov qword	rbp, rsp

	push qword	rax
	push qword	rcx

	cmp byte	bl, 0x0
	je			change_flag_off

change_flag_on :

	mov byte	bl, 0x1
	shl byte	bl, cl
	or byte		[rdi + 0x1], bl
	mov byte	bl, [rdi + 0x1]
	jmp			change_flag_end

change_flag_off :

	mov byte	bl, 0x1
	shl byte	bl, cl
	not byte	bl
	and byte	[rdi + 0x1], bl
	mov byte	bl, [rdi + 0x1]


change_flag_end :

	pop qword	rcx
	pop qword	rax

	leave
	ret

get_flag_cy:

	push qword	rbp
	mov qword	rbp, rsp

	mov qword	rax, 0x0
	mov byte	al, [rdi + 0x1]
	and byte	al, 1000b
	shl byte	al, 3

	leave
	ret


condition_parse :

	push qword	rbp
	mov qword	rbp, rsp

	cmp byte	al, 0x0
	je			condition_parse_nz
	cmp byte	al, 0x1
	je			condition_parse_z
	cmp byte	al, 0x2
	je			condition_parse_nc
	jmp			condition_parse_c

condition_parse_nz :

	mov byte	al, [rdi + 0x1]
	and byte	al, 1000b
	cmp byte	al, 0x0
	je			condition_parse_true
	jmp			condition_parse_false

condition_parse_z :

	mov byte	al, [rdi + 0x1]
	and byte	al, 1000b
	cmp byte	al, 0x1
	je			condition_parse_true
	jmp			condition_parse_false

condition_parse_nc :

	mov byte	al, [rdi + 0x1]
	and byte	al, 1b
	cmp byte	al, 0x0
	je			condition_parse_true
	jmp			condition_parse_false

condition_parse_c :

	mov byte	al, [rdi + 0x1]
	and byte	al, 1b
	cmp byte	al, 0x1
	je			condition_parse_true
	jmp			condition_parse_false

condition_parse_false :

	mov byte	al, 0x0
	jmp			condition_parse_end

condition_parse_true :

	mov byte	al, 0x1
	jmp			condition_parse_end

condition_parse_end :

	leave
	ret

%endif