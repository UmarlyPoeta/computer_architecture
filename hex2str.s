#################################################################
#
# Laboratorium 3/4a. Konwersja liczby calkowitej bez znaku (8 - 64 bitow)
# do postaci ciagu tekstowego (system szesnastkowy).

.globl _start

#################################################################
#
# Alokacja pamieci - zmienne statyczne, 8/16/32/64-bitowe,
# z nadana wartoscia poczatkowa.

.data

# en.wikipedia.org/wiki/Hexspeak
# en.wikipedia.org/wiki/Magic_number_(programming)

var8:	.byte	237
var16:	.word	57005
var32:	.long	4276994270
var64:	.quad	13464654573299691533

str:	.ascii	"value = 0x0000000000000000\n"
.equ	strlen, . - str

#################################################################
#
# Program glowny.
  
 .text
	
_start:

	xor	%eax , %eax
	
# Przekaz tylko niezbedne argumenty do funkcji konwertujacej:
#
# w %rax (odpowiedniej dlugosci) - liczbe podlegajaca konwersji,
# w %rdi - adres bufora (pozycji odpowiadajacej cyfrze jednosci).

#	...
	mov var32, %eax
	mov $4, %ecx
	mov $str + 16, %edi

# W przypadku konwersji typow wielobajtowych przekaz
# w rejestrze %ecx - rozmiar konwertowanego typu danych w bajtach: (1),2,4,8.

#	...


# Wywolaj funkcje konwertujaca.

	call	convert	

# Wyswietl wynik (system_call nr 1).

	mov	$1 , %eax
	mov	%eax , %edi
	mov	$str , %rsi
	mov	$strlen,%edx
	syscall

# Zakoncz program (system_call nr 60).

	mov	$60,%eax
	xor	%edi,%edi
	syscall

#################################################################
#
# Zadanie --- 3 ---
#
# Konwertuj wielobajtowy typ danych.
#
# Argumenty:
# liczba w rejestrze %rax (odpowiedniej dlugosci),
# adres zapisu w %rdi,
# liczba bajtow do konwersji w %ecx.
#
# Funkcja moze wywolywac "convert_byte" z zadania 2.
# Ew. funkcje z zadan 2. i 3. mozna scalic w jedna i zoptymalizowac.
#
# Zwracana wartosc: - (zapis w pamieci).
# Nadpisywane rejestry: ustalic.

convert:
	mov %eax, %ebx;
	and $0xFF, %eax;
	mov lut(,%eax,2), %ax
	mov %ax, (%edi, %ecx, 2)
	mov %ebx, %eax
	shr $8, %eax
	dec %ecx
	jnz convert
	ret
