#################################################################
#
# Laboratorium 5.
# Switch-case-break, adresowanie tablic,
# przekazanie argumentow i wywolanie funkcji jezyka C.
#
# Program ma wykonac wybrana operacje logiczna (arytm.) oraz wyswietlic:
# jej nazwe, argumenty oraz wynik.
#
# Argumenty do programu nalezy przekazac jako parametry z linii komend:
#
# ./swcs arg1 arg2 nr_operacji_logicznej

.globl	main

.section  .note.GNU-stack, "", @progbits

#################################################################
#
# Alokacja pamieci - zmienne statyczne z nadana wartoscia poczatkowa.

.data

str_and:	.asciz	"%u AND %u = %u\n"
str_or:		.asciz	"%u OR %u = %u\n"
str_xor:	.asciz	"%u XOR %u = %u\n"
str_add:	.asciz	"%u + %u = %u\n"
str_def:	.asciz	"DEFAULT\n"

arg_1:		.long	0
arg_2:		.long	0
result:		.long	0
case_no:	.long	0

# --- 2b --- uzupelnic
# Tablica skokow - adresow kolejnych sekcji switcha.

jump_table: .quad c_def, c_def, c_and, c_or, c_def, c_xor, c_def, c_add

#################################################################
#
# program glowny

.text

main:

# W celu zachowania zgodnosci z ABI i funkcjami z GNU C Library
# wierzcholek stosu powinien byc wyrownany do granicy 16 bajtow
# (8-bajtowy adres powrotu i 8-bajtowa ramka stosu).
# Ramki stosu nie tworzymy wiec %rsp nalezy obnizyc o 8 bajtow.

sub	$8 , %rsp


# --- 1b --- Opcjonalnie:
#
# sprawdz, czy z linii komend przekazano trzy parametry,
# jesli nie - wyjdz (opcjonalnie zwracajac -1 w %eax).
cmp $4, %edi
je dalej
mov $(-1), %eax
ret

dalej:
sub $8, %rsp
# ...

convert_argv:


# --- 1a --- Przekazywanie parametrow z linii komend.
#
# Konwertuj przekazane parametry (argv) z ciagu tekstowego na liczbe calkowita,
# np. jedna z funkcji biblioteki stdlib.h (np. atoi, strtol).

mov %rsi, %r15
mov	8(%rsi) , %rdi
call	atoi
mov	%eax , arg_1

# Kolejne parametry:
mov	16(%r15) , %rdi
call	atoi
mov	%eax , arg_2

mov	24(%r15) , %rdi
call	atoi

# ...

# --- 2a --- Switch - Case.
#
# Sprawdz, czy podany nr przypadku miesci sie w stablicowanym zakresie.


cmp	$7 , %eax
ja	c_def



# Czesc wspolna dla wszystkich przypadkow (za wyjatkiem default - mozna zoptymalizowac).

mov	arg_1 , %esi
mov	arg_2 , %edx
mov	%edx , %ecx

# --- 2c ---
#
# Skok posredni - do adresu odczytanego z tablicy (do odpowiedniego przypadku).
jmp *jump_table(,%eax,8)
# ...

# W kazdym z przypadkow (oprocz default) wykonaj odpowiednia operacje
# logiczna/arytmetyczna oraz przekaz niezbedne argumenty do funkcji printf.

c_add:
add	%esi,%ecx
mov	%ecx, result
mov	$str_add,%rdi
jmp	brk

c_and:
and	%esi,%ecx
mov	%ecx, result
mov	$str_and,%rdi
jmp	brk

c_or:
or	%esi,%ecx
mov	%ecx, result
mov	$str_or,%rdi
jmp	brk

c_xor:
xor	%esi,%ecx
mov	%ecx, result
mov	$str_xor,%rdi
jmp	brk

c_def:

mov	$str_def,%rdi

brk:	# break

# Czesc wspolna dla wszystkich przypadkow:
# wywolaj funkcje printf z uprzednio umieszczonymi w odpowiednich rejestrach (wg ABI):
# adresem ciagu, argumentami i wynikiem operacji.
# Nie sa uzywane typy zmiennoprzecinkowe i rejestry wektorowe (%xmm) wiec %eax=0.

xor	%eax , %eax
call	printf

# Przesun wskaznik stosu o 8 bajtow w gore aby "ret" prawidlowo sciagnal adres powrotu.
add	$8 , %rsp

# Powrot z main, w %eax kod bledu.

xor	%eax , %eax
ret

#################################################################
