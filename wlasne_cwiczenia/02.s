.globl _start

.data
    num1: .long 5
    num2: .long 7
    wynik: .ascii "x + x = x\n"
    .equ strlen, . - wynik
    .equ ad_num1, wynik
    .equ ad_num2, wynik + 4
    .equ ad_wynik, strlen - 1

.text

_start:
    mov $num1, %ebx
    add %ebx, num2 
    mov $1, %eax
    mov $1, %edi
    mov $