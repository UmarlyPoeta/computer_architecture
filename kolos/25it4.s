#################################################################
#
# Program ma zliczyc i wydrukowac w oknie terminala (funkcja printf)
#
# na maks. 10 punktow:
#
# - liczbe wszystkich elementow tablicy na odcinku od elementu poczatkowego (indeks zero) do pierwszego
# napotkanego elementu o wartosci ujemnej (bez niego).
#
# Zadanie ratunkowe - na maks. 4 punkty: wydrukowanie tekstu "napis"
# z wartosciami podanymi przez prowadzacego.
#
#################################################################

.globl	main

.data

napisa:		.asciz  "%u elementow odczytanych\n"

tablica:	.long	2, 4, 3, 1, 0, 8, 7, 9, -2, 6, 4, 6, 2, -1, 1

licznik_el:	.long	0
licznik_parz:	.long	0


#################################################################

.text

main:
sub	$8 , %rsp

# Przykladowe kroki.

# Inicjuj zmienne wartosciami poczatkowymi.
mov $0, %edx



petla:

# Usun bledy i odczytaj w prawidlowy sposob element tablicy.
incl licznik_el
# mov	tablica(, %edx, 8) , %eax
mov tablica(,%edx, 4), %eax


# Sprawdz czy wartosc jest ujemna (jesli tak -> wyjdz z petli).
cmp $0, %eax
js koniec
inc %edx
jmp petla
# Zaktualizuj odpowiednie liczniki elementow.

# jmp	petla

koniec:

# Wyswietl wynik (printf) zgodnie z formatowaniem ciagu "str"
# przekazujac argumenty zgodnie ABI.
mov $napisa, %rdi
mov licznik_el, %esi
xor %eax, %eax

call	printf

# Koniec funkcji main.

add	$8 , %rsp
xor	%eax , %eax
ret

#################################################################

