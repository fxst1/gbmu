;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;	Set CY flag depend EAX
;
;	EAX : value
;
set_flag_cy16:

	push dword	ebp
	mov dword	ebp, esp

	push dword	ecx
	push dword	ebx
	push dword	eax

	mov dword	ebx, 0b
	mov word	cx, 1b
	jc			set_flag_cy16_on
	jo			set_flag_cy16_on
	cmp word	ax, 0x4000
	jge			set_flag_cy16_on
	jmp			set_flag_cy16_end

set_flag_cy16_on:

	mov dword	ebx, 1b

set_flag_cy16_end:

	mov word	cx, 0b			; Flag bit index
	call		change_flag

	pop dword	eax
	pop dword	ebx
	pop dword	ecx

	leave
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;	Set H flag depend EAX
;
;	EAX : value
;
set_flag_h16:

	push dword	ebp
	mov dword	ebp, esp

	push dword	ecx
	push dword	ebx
	push dword	eax

	mov dword	ebx, 0b
	ja			set_flag_h16_on
	cmp dword	eax, 0x400
	jge			set_flag_h16_on
	jmp			set_flag_h16_end

set_flag_h16_on:

	mov dword	ebx, 1b

set_flag_h16_end:

	mov word	cx, 1b			; Flag bit index
	call		change_flag

	pop dword	eax
	pop dword	ebx
	pop dword	ecx

	leave
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;	Set H flag depend AX
;
;	EAX : value
;
set_flag_z16:

	push dword	ebp
	mov dword	ebp, esp

	push dword	ecx
	push dword	ebx
	push dword	eax

	mov dword	ebx, 0b
	jz			set_flag_z16_on
	cmp word	ax, 0x0
	je			set_flag_z16_on
	jmp			set_flag_z16_end

set_flag_z16_on:

	mov dword	ebx, 1b

set_flag_z16_end:

	mov word	cx, 3			; Flag bit index
	call		change_flag

	pop dword	eax
	pop dword	ebx
	pop dword	ecx

	leave
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;	Set H flag depend AL
;
;	EAX : value
;
set_flag_z8:

	push dword	ebp
	mov dword	ebp, esp

	push dword	ecx
	push dword	ebx
	push dword	eax

	mov dword	ebx, 0b
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

	push dword	ebp
	mov dword	ebp, esp

	push dword	eax
	push dword	ecx

	cmp byte	bl, 0x0
	je			change_flag_off

change_flag_on :

	mov byte	bl, 0x1
	shl byte	bl, cl
	or byte		[edi + 0x1], bl
	mov byte	bl, [edi + 0x1]
	jmp			change_flag_end

change_flag_off :

	mov byte	bl, 0x1
	shl byte	bl, cl
	not byte	bl
	and byte	[edi + 0x1], bl
	mov byte	bl, [edi + 0x1]


change_flag_end :

	pop dword	ecx
	pop dword	eax

	leave
	ret