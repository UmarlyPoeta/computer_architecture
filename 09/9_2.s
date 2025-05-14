.globl	main

.data

tab:	.double	1.0, 2.0, 4.0, 5.0, 6.0, 7.0
result:	.double	0.0

nelrec:	.double	0.16666666666666666666666666666666666666666666667

nel:	.long	6

cw:	.word	0

outstr:	.string "avg = %3.20lf\n"

.text

main:

sub	$8 , %rsp

finit

# Ustaw podwojna precyzje obliczen, zaokraglanie "to nearest even".

fstcw	cw
andw	$0xFCFF , cw
orw	$0x0200 , cw
fldcw	cw

# Oblicz srednia arytemtyczna elementow tablicy:
# a) dzielac przez liczbe elementow,
# b) mnozac przez jej odwrotnosc.
#
# Punkty a i b wykonac dla liczby elementow "nel" dobranej tak, aby:
# - 1/nel mialo dokladna reprezentacje binarna (np. 1/4)
# - nie bylo mozliwe dokladne zapisanie wartosci 1/nel uzywajac typu double (np. 1/6)

mov	nel , %edi

# Nadaj wartosc poczatkowa sumie elementow.
fldz

sum_loop:

# Zsumuj wartosci elementow tablicy.
faddl tab-8( , %edi, 8)


dec	%edi
jnz	sum_loop

# Podziel (pomnoz) przez liczbe (odwrotnosc liczby) elementow.

#fildl nel
#fdivrp

fldl nelrec
fmulp


# Zapisz wynik.

fstpl	result

# Wydrukuj wynik.

movsd	result , %xmm0
mov	$outstr , %rdi
xor	%eax , %eax
inc	%eax
call	printf

add	$8 , %rsp
ret
