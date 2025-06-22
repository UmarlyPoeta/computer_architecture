.section .note.GNU-stack, "", @progbits

.globl	main

.data

# Wartosci rezystorow (omy) oraz napięcia na wejsciu i wyjsciu dzielnika (V).

R1:	.float	3300	#
R2:	.float	0	# [omow]
Vi:	.float	5	# [V]
Vo:	.float	3.3	# [V]

cw:		.word 0

outstr:	.string "R2 = %f omow\n"

.text

main:

sub	$8,%rsp

# Wlacz FPU, ustaw podwojna precyzje obliczen, wylacz wyjatki

finit
fstcw   cw
andw $0xFCFF, cw
orw $0x0200, cw
fldcw cw

# ...

# Oblicz wartosc opornika R2 przeksztalcajac wzor: Vo = Vi * R2/(R1+R2)

flds Vo
flds R1
fmulp

flds Vo
flds Vi
fsubp


fdivrp


  # ...

# Zapisz wynik i wydrukuj jako double.

fstl R2
movsd R2, %xmm0
mov $outstr, %rdi
mov $1, %eax
call	printf

add	$8,%rsp
ret
