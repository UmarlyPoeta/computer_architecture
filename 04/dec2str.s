#################################################################
#
# Laboratorium 3/4b. Konwersja liczby calkowitej bez znaku (8 - 64 bitow)
# do postaci ciagu tekstowego w systemie dziesietnym.

.globl _start

#################################################################
#
# Alokacja pamieci - zmienne statyczne, 8/16/32/64-bitowe,
# z nadana wartoscia poczatkowa.

.data

var8:	.byte	0b01011101
var16:	.word	0xACAB
var32:	.long	0x0D15EA5E
var64:	.quad	0xFEEDFACECAFEBEEF

str:	.ascii	"val = 00000000000000000000\n"
.equ	strlen, . - str

#################################################################
#
# Program glowny.

 .text

_start:

	xor	%eax , %eax

# Przekaz tylko niezbedne argumenty do funkcji konwertujacej:
# w %rax (odpowiedniej dlugosci) - liczbe podlegajaca konwersji,
# w rejestrze %rdi - adres bufora (pozycji odpowiadajacej cyfrze jednosci).

#	...
	mov var64, %rax
	mov	$str+25 , %rdi

# Wywolaj funkcje konwertujaca.

	call	convert_dec

# Wyswietl wynik (system_call nr 1).

	mov	$1 , %eax
	mov	$1 , %edi
	mov	$str , %rsi
	mov	$strlen , %edx
	syscall

# Zakoncz program (system_call nr 60).

	mov	$60 , %eax
	xor	%edi , %edi
	syscall

#################################################################
#
# Konwertuj wielobajtowy typ danych na ciag tekstowy
# (reprezentacja liczby w systemie dziesietnym).
# Argumenty: liczba w rejestrze %rax (odpowiedniej dlugosci), adres zapisu w %rdi.
#
# Zwracana wartosc: -
# Nadpisywane rejestry: ustalic

convert_dec:

#	...
	
	mov $10, %ecx
	d:
	xor %edx, %edx
	div %rcx
	add $48, %dl
	mov %dl, (%rdi)
	dec %rdi
	sub $0, %rax
	jnz d
	ret

#################################################################
