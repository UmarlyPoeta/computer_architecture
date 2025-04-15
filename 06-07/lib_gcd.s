.section  .note.GNU-stack, "", @progbits

.globl gcd

.text

.type	gcd,@function

gcd:
sub	$8 , %rsp

# Zabezpiecz odpowiednie rejestry przed nadpisaniem
# i wywolaj print_call.

# ...
push %rdi
push %rsi
call print_call
pop %rsi
pop %rdi

# Algorytm Euklidesa:
#
# unsigned int GCD(unsigned int a, unsigned int b) 
# { 
#   if (b==0) return a; 
#   else GCD(b, a % b); 
# } 

# ...
or %esi, %esi
jz gcd_exit

xor %edx, %edx
mov %edi, %eax  # dzielna
div %esi

mov %esi, %edi
mov %edx, %esi
call gcd

gcd_exit:
# Zabezpiecz odpowiednie rejestry przed nadpisaniem
# i wywolaj print_ret.

# ...
push %rdi
push %rsi
call print_ret
pop %rsi
pop %rdi

# Zwroc obliczona wartosc w %eax.

# ...
mov %edi, %eax

add	$8 , %rsp
ret

