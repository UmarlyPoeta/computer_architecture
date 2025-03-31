.globl _start

.data
    str: .ascii "Hello World!\n"  # Dodałem \n na końcu, żeby była nowa linia
    .equ strlen, . - str          # Teraz jest po deklaracji `str`

.text

_start:
    mov $1, %eax          # syscall: sys_write (1)
    mov $1, %edi          # stdout (1)
    lea str(%rip), %rsi   # Załaduj adres `str` do %rsi
    mov $strlen, %edx     # Długość stringa
    syscall               # Wywołaj syscall

    mov $60, %eax         # syscall: sys_exit (60)
    xor %edi, %edi        # Kod wyjścia 0
    syscall               # Wywołaj syscall
