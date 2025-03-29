.globl _start

.equ sys_write, 1
.equ sys_exit, 60
.equ stdout, 1
.equ strlen, new_line - str + 1




.data
str: .ascii "Patryk"
new_line: .byte 0x0A



.text

_start:


mov $sys_write, %eax
mov $stdout, %edi
mov $str, %esi
mov $strlen, %edx
mov $sys_exit, %eax
mov $0, %edi
syscall