section .data
    num1 dd 5
    num2 dd 7

section .text
global _start

_start:
    ; Load num1 into EAX
    mov eax, [num1]

    ; Add num2 to EAX
    add eax, [num2]

    ; Exit with the result in EAX
    mov ebx, eax
    mov eax, 1          ; syscall number for exit
    int 0x80            ; call kernel

section .bss
