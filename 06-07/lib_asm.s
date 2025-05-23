.section  .note.GNU-stack, "", @progbits

.globl print_call
.globl print_ret

##########################################################

.data
counter:	.long	0
call_str:	.ascii	"rip = %lx\trsp = %lx\tcall counter = %d\n\0"
ret_str:	.ascii	"rip = %lx\trsp = %lx\tret  counter = %d\n\0"

##########################################################

.text

.type	print_call,@function
.type	print_ret,@function

##########################################################

print_call:

lea	-7(%rip) , %rsi
incl	counter(%rip)
mov	counter(%rip) , %ecx
lea	call_str(%rip) , %rdi

jmp	print_registers

##########################################################

print_ret:

lea	-7(%rip) , %rsi
mov	counter(%rip) , %ecx
decl	counter(%rip)
lea	ret_str(%rip) , %rdi

##########################################################

print_registers:

sub	$8 , %rsp

mov	%rsp , %rdx
xor	%eax , %eax
call	printf

add	$8 , %rsp
ret

##########################################################

