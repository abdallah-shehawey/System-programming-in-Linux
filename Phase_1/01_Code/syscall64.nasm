global main

SECTION .data
msg: db "Hello World from x86_64 machine", 0Ah, 0h ; define byte "hello...."\n \0
len_msg: equ $ - msg   ; current location - &msg

SECTION .text
main:
	mov rax, 1      ; syscall number
	mov rdi, 1      ; argument 1 (std output) 
	mov rsi, msg    ; pointer of buffer 
	mov rdx, len_msg; count
	syscall         ; write(1, msg, len_msg);

	mov r15, rax	; temp
	mov rax, 60     ; syscall number
	mov rdi, r15    ; return val of first write inst in argument 1 (register) exit status
	syscall         ; exit(write_len)	
