.globl	main

.data

str_tf:		.string "tfloat = %3.20Lf\n"
str_d:		.string "double = %3.20lf\n"
str_f:		.string "float = %3.20f\n"

cw:	.word 0

f_tf:	.tfloat	0.0
f_d:	.double	0.0
f_f:	.float		0.0
f_2:	.float		2.0
f_nan:	.long 0x7f800001	# NaN
f_inf:	.long 0x7f800000	# +inf

.text

main:

finit

# 1) Ustaw odpowiednia precyzje obliczen oraz sposob zaokraglania.
# 2) Wlacz ("odmaskuj") sygnalizwoanie wyjatkow.

fstcw	cw
andw	$0xF0FF , cw
orw	$0x0000 , cw
fldcw	cw

# 1) Wykonaj dzialanie sqrt(2.0) * sqrt(2.0) - 2.0
# 2) Wymus zgloszenie (wybranego) wyjatku


# Wydrukuj wynik z rejestru %st(0)

call	print_tfloat

xor	%eax , %eax
ret

#####################################################

print_tfloat:
sub	$16 , %rsp
fstpt	(%rsp)
mov	$str_tf , %rdi
xor	%eax , %eax
call	printf
add	$16 , %rsp
ret

#####################################################

print_double:
fstl	f_d
movsd	f_d , %xmm0
mov	$str_d , %rdi
mov	$1 , %eax
call	printf
ret

#####################################################

print_float:
fsts	f_f
movss	f_f , %xmm0
cvtss2sd %xmm0 , %xmm0
mov	$str_f , %rdi
mov	$1 , %eax
call	printf
ret

#####################################################

