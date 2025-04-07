#include <stdlib.h>
#include <stdio.h>

int gcd(int,int);
void print_call(void);
void print_ret(void);

unsigned int a,b,c;

int main(int argc, char **argv){

if (argc!=3) return(-1);

print_call();

a=atoi(argv[1]);
b=atoi(argv[2]);

c=gcd(a,b);

print_ret();

printf("GCD(%u,%u)=%u\n",a,b,c);

return(0);
}