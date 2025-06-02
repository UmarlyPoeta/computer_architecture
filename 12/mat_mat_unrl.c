#include <stdio.h>
#include <math.h>
#include "eval_time.h"

#include <x86intrin.h>

#define SIZE 2000			//rozmiar glownych macierzy

static double a[SIZE*SIZE];
static double b[SIZE*SIZE];
static double c[SIZE*SIZE];
static double d[SIZE*SIZE];
static double at[SIZE*SIZE];

void dgemm_unroll_2x1(int,double*,double*,double*);
void dgemm_unroll_2x2(int,double*,double*,double*);
void dgemm_unroll_4x1(int,double*,double*,double*);
void dgemm_unroll_4x4(int,double*,double*,double*);
void dgemm_unroll_8x1(int,double*,double*,double*);
void dgemm_unroll_16x1(int,double*,double*,double*);

void dgemm_sse_2x1(int,double*,double*,double*);
void dgemm_sse_2x2(int,double*,double*,double*);
void dgemm_sse_4x1(int,double*,double*,double*);
void dgemm_sse_4x4(int,double*,double*,double*);
void dgemm_sse_8x1(int,double*,double*,double*);
void dgemm_sse_16x1(int,double*,double*,double*);

void dgemm_avx_4x1(int,double*,double*,double*);
void dgemm_avx_4x4(int,double*,double*,double*);
void dgemm_avx_8x1(int,double*,double*,double*);
void dgemm_avx_8x8(int,double*,double*,double*);
void dgemm_avx_16x1(int,double*,double*,double*);

void r2c(int,double*,double*);


int main(void)
{

unsigned int i,j,n;
unsigned long f;

double time_tabl[3];

n=SIZE;

//Liczba operacji zmiennoprzecinkowych:
f=2*(unsigned long)n*(unsigned long)n*(unsigned long)n;


for (i=0;i<n;++i)			//wypelnij a i b jakimis wartosciami poczatkowymi
    for (j=0;j<n;++j)
    {
	a[j+i*n]=(double)(i+j*n);
	b[j+i*n]=(double)(j+i*n);
    }

//c i d zostaw wyzerowane


init_time();
dgemm_sse_8x1(SIZE,a,b,c);
read_time(time_tabl);

printf("ver. 1      = %lf s\n",time_tabl[1]);
printf("GFLOPS      = %lf\n\n",(double)f/time_tabl[1]*1.0e-9);

//-------------------------------------------------------------------------------

init_time();
dgemm_sse_16x1(SIZE,a,b,d);
read_time(time_tabl);

printf("ver. 2      = %lf s\n",time_tabl[1]);
printf("GFLOPS      = %lf\n\n",(double)f/time_tabl[1]*1.0e-9);


//sprawdzenie czy oba algorytmy daly ten sam wynik

for (i=0;i<SIZE*SIZE;++i)
    if (fabs(c[i]-d[i])>1.0e-9) {printf("Error!\n"); goto rtrn;}

rtrn:
return(0);
}

//-------------------------------------------------------------------------------

void dgemm_unroll_2x1(int n, double* A, double* B, double* C)
{
register int i,j,k;
register double ci0j0,ci1j0;

for(i=0;i<n;i+=2)
    for(j=0;j<n;++j)
    {
	ci0j0=C[i+0+j*n];
	ci1j0=C[i+1+j*n];

	for(k=0;k<n;++k){
	    ci0j0+=A[i+0+k*n]*B[k+j*n];
	    ci1j0+=A[i+1+k*n]*B[k+j*n];
	    }
	C[i+0+j*n]=ci0j0;
	C[i+1+j*n]=ci1j0;

    }
}

//-------------------------------------------------------------------------------

void dgemm_unroll_2x2(int n, double* A, double* B, double* C)
{
register int i,j,k;
register double ci0j0,ci1j0,ci0j1,ci1j1;

for(i=0;i<n;i+=2)
    for(j=0;j<n;j+=2)
    {
	ci0j0=C[i+0+(j+0)*n];
	ci1j0=C[i+1+(j+0)*n];
	ci0j1=C[i+0+(j+1)*n];
	ci1j1=C[i+1+(j+1)*n];

	for(k=0;k<n;++k)
	{
	    ci0j0+=A[i+0+k*n]*B[k+(j+0)*n];
	    ci1j0+=A[i+1+k*n]*B[k+(j+0)*n];
	    ci0j1+=A[i+0+k*n]*B[k+(j+1)*n];
	    ci1j1+=A[i+1+k*n]*B[k+(j+1)*n];
	}

	C[i+0+(j+0)*n]=ci0j0;
	C[i+1+(j+0)*n]=ci1j0;
	C[i+0+(j+1)*n]=ci0j1;
	C[i+1+(j+1)*n]=ci1j1;

    }
}

//-------------------------------------------------------------------------------

void dgemm_unroll_4x1(int n, double* A, double* B, double* C)
{
register int i,j,k;
register double ci0j0,ci1j0,ci2j0,ci3j0;

for(i=0;i<n;i+=4)
    for(j=0;j<n;++j)
    {
	ci0j0=C[i+0+j*n];
	ci1j0=C[i+1+j*n];
	ci2j0=C[i+2+j*n];
	ci3j0=C[i+3+j*n];

	for(k=0;k<n;++k)
	{
	    ci0j0+=A[i+0+k*n]*B[k+j*n];
	    ci1j0+=A[i+1+k*n]*B[k+j*n];
	    ci2j0+=A[i+2+k*n]*B[k+j*n];
	    ci3j0+=A[i+3+k*n]*B[k+j*n];
	}

	C[i+0+j*n]=ci0j0;
	C[i+1+j*n]=ci1j0;
	C[i+2+j*n]=ci2j0;
	C[i+3+j*n]=ci3j0;

    }
}

//-------------------------------------------------------------------------------

void dgemm_unroll_4x4(int n, double* A, double* B, double* C)
{
register int i,j,k;

register double ci0j0,ci0j1,ci0j2,ci0j3;
register double ci1j0,ci1j1,ci1j2,ci1j3;
register double ci2j0,ci2j1,ci2j2,ci2j3;
register double ci3j0,ci3j1,ci3j2,ci3j3;

for(i=0;i<n;i+=4)
    for(j=0;j<n;j+=4)
    {
	ci0j0=C[i+0+(j+0)*n];
	ci1j0=C[i+1+(j+0)*n];
	ci2j0=C[i+2+(j+0)*n];
	ci3j0=C[i+3+(j+0)*n];
	ci0j1=C[i+0+(j+1)*n];
	ci1j1=C[i+1+(j+1)*n];
	ci2j1=C[i+2+(j+1)*n];
	ci3j1=C[i+3+(j+1)*n];
	ci0j2=C[i+0+(j+2)*n];
	ci1j2=C[i+1+(j+2)*n];
	ci2j2=C[i+2+(j+2)*n];
	ci3j2=C[i+3+(j+2)*n];
	ci0j3=C[i+0+(j+3)*n];
	ci1j3=C[i+1+(j+3)*n];
	ci2j3=C[i+2+(j+3)*n];
	ci3j3=C[i+3+(j+3)*n];

	for(k=0;k<n;++k)
	{
	    ci0j0+=A[i+0+k*n]*B[k+(j+0)*n];
	    ci0j1+=A[i+0+k*n]*B[k+(j+1)*n];
	    ci0j2+=A[i+0+k*n]*B[k+(j+2)*n];
	    ci0j3+=A[i+0+k*n]*B[k+(j+3)*n];
	    ci1j0+=A[i+1+k*n]*B[k+(j+0)*n];
	    ci1j1+=A[i+1+k*n]*B[k+(j+1)*n];
	    ci1j2+=A[i+1+k*n]*B[k+(j+2)*n];
	    ci1j3+=A[i+1+k*n]*B[k+(j+3)*n];
	    ci2j0+=A[i+2+k*n]*B[k+(j+0)*n];
	    ci2j1+=A[i+2+k*n]*B[k+(j+1)*n];
	    ci2j2+=A[i+2+k*n]*B[k+(j+2)*n];
	    ci2j3+=A[i+2+k*n]*B[k+(j+3)*n];
	    ci3j0+=A[i+3+k*n]*B[k+(j+0)*n];
	    ci3j1+=A[i+3+k*n]*B[k+(j+1)*n];
	    ci3j2+=A[i+3+k*n]*B[k+(j+2)*n];
	    ci3j3+=A[i+3+k*n]*B[k+(j+3)*n];
	}

	C[i+0+(j+0)*n]=ci0j0;
	C[i+1+(j+0)*n]=ci1j0;
	C[i+2+(j+0)*n]=ci2j0;
	C[i+3+(j+0)*n]=ci3j0;
	C[i+0+(j+1)*n]=ci0j1;
	C[i+1+(j+1)*n]=ci1j1;
	C[i+2+(j+1)*n]=ci2j1;
	C[i+3+(j+1)*n]=ci3j1;
	C[i+0+(j+2)*n]=ci0j2;
	C[i+1+(j+2)*n]=ci1j2;
	C[i+2+(j+2)*n]=ci2j2;
	C[i+3+(j+2)*n]=ci3j2;
	C[i+0+(j+3)*n]=ci0j3;
	C[i+1+(j+3)*n]=ci1j3;
	C[i+2+(j+3)*n]=ci2j3;
	C[i+3+(j+3)*n]=ci3j3;
    }
}

//-------------------------------------------------------------------------------

void dgemm_unroll_8x1(int n, double* A, double* B, double* C)
{
register int i,j,k;
register double ci0j0,ci1j0,ci2j0,ci3j0,ci4j0,ci5j0,ci6j0,ci7j0;

for(i=0;i<n;i+=8)
    for(j=0;j<n;++j)
    {
	ci0j0=C[i+0+j*n];
	ci1j0=C[i+1+j*n];
	ci2j0=C[i+2+j*n];
	ci3j0=C[i+3+j*n];
	ci4j0=C[i+4+j*n];
	ci5j0=C[i+5+j*n];
	ci6j0=C[i+6+j*n];
	ci7j0=C[i+7+j*n];

	for(k=0;k<n;++k)
	{
	    ci0j0+=A[i+0+k*n]*B[k+j*n];
	    ci1j0+=A[i+1+k*n]*B[k+j*n];
	    ci2j0+=A[i+2+k*n]*B[k+j*n];
	    ci3j0+=A[i+3+k*n]*B[k+j*n];
	    ci4j0+=A[i+4+k*n]*B[k+j*n];
	    ci5j0+=A[i+5+k*n]*B[k+j*n];
	    ci6j0+=A[i+6+k*n]*B[k+j*n];
	    ci7j0+=A[i+7+k*n]*B[k+j*n];
	}

	C[i+0+j*n]=ci0j0;
	C[i+1+j*n]=ci1j0;
	C[i+2+j*n]=ci2j0;
	C[i+3+j*n]=ci3j0;
	C[i+4+j*n]=ci4j0;
	C[i+5+j*n]=ci5j0;
	C[i+6+j*n]=ci6j0;
	C[i+7+j*n]=ci7j0;

    }
}

//-------------------------------------------------------------------------------

void dgemm_unroll_16x1(int n, double* A, double* B, double* C)
{
register int i,j,k;
register double ci0j0,ci1j0,ci2j0,ci3j0,ci4j0,ci5j0,ci6j0,ci7j0;
register double ci8j0,ci9j0,ci10j0,ci11j0,ci12j0,ci13j0,ci14j0,ci15j0;

for(i=0;i<n;i+=16)
    for(j=0;j<n;++j)
    {
	ci0j0=C[i+0+j*n];
	ci1j0=C[i+1+j*n];
	ci2j0=C[i+2+j*n];
	ci3j0=C[i+3+j*n];
	ci4j0=C[i+4+j*n];
	ci5j0=C[i+5+j*n];
	ci6j0=C[i+6+j*n];
	ci7j0=C[i+7+j*n];
	ci8j0=C[i+8+j*n];
	ci9j0=C[i+9+j*n];
	ci10j0=C[i+10+j*n];
	ci11j0=C[i+11+j*n];
	ci12j0=C[i+12+j*n];
	ci13j0=C[i+13+j*n];
	ci14j0=C[i+14+j*n];
	ci15j0=C[i+15+j*n];

	for(k=0;k<n;++k)
	{
	    ci0j0+=A[i+0+k*n]*B[k+j*n];
	    ci1j0+=A[i+1+k*n]*B[k+j*n];
	    ci2j0+=A[i+2+k*n]*B[k+j*n];
	    ci3j0+=A[i+3+k*n]*B[k+j*n];
	    ci4j0+=A[i+4+k*n]*B[k+j*n];
	    ci5j0+=A[i+5+k*n]*B[k+j*n];
	    ci6j0+=A[i+6+k*n]*B[k+j*n];
	    ci7j0+=A[i+7+k*n]*B[k+j*n];
	    ci8j0+=A[i+8+k*n]*B[k+j*n];
	    ci9j0+=A[i+9+k*n]*B[k+j*n];
	    ci10j0+=A[i+10+k*n]*B[k+j*n];
	    ci11j0+=A[i+11+k*n]*B[k+j*n];
	    ci12j0+=A[i+12+k*n]*B[k+j*n];
	    ci13j0+=A[i+13+k*n]*B[k+j*n];
	    ci14j0+=A[i+14+k*n]*B[k+j*n];
	    ci15j0+=A[i+15+k*n]*B[k+j*n];

	}

	C[i+0+j*n]=ci0j0;
	C[i+1+j*n]=ci1j0;
	C[i+2+j*n]=ci2j0;
	C[i+3+j*n]=ci3j0;
	C[i+4+j*n]=ci4j0;
	C[i+5+j*n]=ci5j0;
	C[i+6+j*n]=ci6j0;
	C[i+7+j*n]=ci7j0;
	C[i+8+j*n]=ci8j0;
	C[i+9+j*n]=ci9j0;
	C[i+10+j*n]=ci10j0;
	C[i+11+j*n]=ci11j0;
	C[i+12+j*n]=ci12j0;
	C[i+13+j*n]=ci13j0;
	C[i+14+j*n]=ci14j0;
	C[i+15+j*n]=ci15j0;

    }
}

//-------------------------------------------------------------------------------

void dgemm_sse_2x1 (int n, double* A, double* B, double* C)
{
register int i,j,k;
__m128d Cij;


for(i=0;i<n;i+=2)
    for (j=0;j<n;j++)
    {
	Cij = _mm_load_pd(C+i+j*n);
	for(k=0;k<n;k++)
		Cij = _mm_add_pd(Cij , _mm_mul_pd( _mm_load_pd(A+i+k*n) , _mm_load1_pd(B+k+j*n) ) );

	_mm_store_pd(C+i+j*n, Cij);
    }
}

//-------------------------------------------------------------------------------

void dgemm_sse_2x2 (int n, double *A, double *B, double *C)
{
register int i,j,k,x;

__m128d Ci0j0,Ci0j1,bk0j0,bk0j1;

for (i=0;i<n;i+=2)
    for (j=0;j<n;j+=2)
    {
	Ci0j0 = _mm_load_pd(C+i+(j+0)*n);
	Ci0j1 = _mm_load_pd(C+i+(j+1)*n);

	for(k=0;k<n;++k)
	{
	    bk0j0 = _mm_load1_pd(B+k+(j+0)*n);
	    bk0j1 = _mm_load1_pd(B+k+(j+1)*n);

	    Ci0j0 = _mm_add_pd(Ci0j0,_mm_mul_pd(_mm_load_pd(A+i+n*k), bk0j0));
	    Ci0j1 = _mm_add_pd(Ci0j1,_mm_mul_pd(_mm_load_pd(A+i+n*k), bk0j1));
	}

	_mm_store_pd(C+i+(j+0)*n, Ci0j0);
	_mm_store_pd(C+i+(j+1)*n, Ci0j1);
    }
}

//-------------------------------------------------------------------------------

void dgemm_sse_4x1 (int n, double *A, double *B, double *C)
{
register int i,j,k,x;

__m128d Ci0j0,Ci2j0,bkj;

for (i=0;i<n;i+=4)
    for (j=0;j<n;++j)
    {
	Ci0j0 = _mm_load_pd(C+i+0+j*n);
	Ci2j0 = _mm_load_pd(C+i+2+j*n);

	for(k=0;k<n;++k)
	{
	    bkj = _mm_load1_pd(B+k+j*n);

	    Ci0j0 = _mm_add_pd(Ci0j0,_mm_mul_pd(_mm_load_pd(A+i+n*k+0), bkj));
	    Ci2j0 = _mm_add_pd(Ci2j0,_mm_mul_pd(_mm_load_pd(A+i+n*k+2), bkj));
	}

	_mm_store_pd(C+i+0+j*n, Ci0j0);
	_mm_store_pd(C+i+2+j*n, Ci2j0);
    }
}

//-------------------------------------------------------------------------------

void dgemm_sse_4x4 (int n, double *A, double *B, double *C)
{
register int i,j,k,x;

__m128d a0,a2,Ci0j0,Ci0j1,Ci0j2,Ci0j3,Ci2j0,Ci2j1,Ci2j2,Ci2j3,bk0j0,bk0j1,bk0j2,bk0j3;

for (i=0;i<n;i+=4)
    for (j=0;j<n;j+=4)
    {
	Ci0j0 = _mm_load_pd(C+i+(j+0)*n);
	Ci0j1 = _mm_load_pd(C+i+(j+1)*n);
	Ci0j2 = _mm_load_pd(C+i+(j+2)*n);
	Ci0j3 = _mm_load_pd(C+i+(j+3)*n);
	Ci2j0 = _mm_load_pd(C+i+2+(j+0)*n);
	Ci2j1 = _mm_load_pd(C+i+2+(j+1)*n);
	Ci2j2 = _mm_load_pd(C+i+2+(j+2)*n);
	Ci2j3 = _mm_load_pd(C+i+2+(j+3)*n);

	for(k=0;k<n;++k)
	{
	    a0 = _mm_load_pd(A+i+0+n*k);
	    a2 = _mm_load_pd(A+i+2+n*k);

	    bk0j0 = _mm_load1_pd(B+k+(j+0)*n);
	    bk0j1 = _mm_load1_pd(B+k+(j+1)*n);
	    bk0j2 = _mm_load1_pd(B+k+(j+2)*n);
	    bk0j3 = _mm_load1_pd(B+k+(j+3)*n);

	    Ci0j0 = _mm_add_pd(Ci0j0,_mm_mul_pd(a0, bk0j0));
	    Ci0j1 = _mm_add_pd(Ci0j1,_mm_mul_pd(a0, bk0j1));
	    Ci0j2 = _mm_add_pd(Ci0j2,_mm_mul_pd(a0, bk0j2));
	    Ci0j3 = _mm_add_pd(Ci0j3,_mm_mul_pd(a0, bk0j3));
	    Ci2j0 = _mm_add_pd(Ci2j0,_mm_mul_pd(a2, bk0j0));
	    Ci2j1 = _mm_add_pd(Ci2j1,_mm_mul_pd(a2, bk0j1));
	    Ci2j2 = _mm_add_pd(Ci2j2,_mm_mul_pd(a2, bk0j2));
	    Ci2j3 = _mm_add_pd(Ci2j3,_mm_mul_pd(a2, bk0j3));
	}

	_mm_store_pd(C+i+(j+0)*n, Ci0j0);
	_mm_store_pd(C+i+(j+1)*n, Ci0j1);
	_mm_store_pd(C+i+(j+2)*n, Ci0j2);
	_mm_store_pd(C+i+(j+3)*n, Ci0j3);
	_mm_store_pd(C+i+2+(j+0)*n, Ci2j0);
	_mm_store_pd(C+i+2+(j+1)*n, Ci2j1);
	_mm_store_pd(C+i+2+(j+2)*n, Ci2j2);
	_mm_store_pd(C+i+2+(j+3)*n, Ci2j3);

    }
}

//-------------------------------------------------------------------------------

void dgemm_sse_8x1 (int n, double *A, double *B, double *C)
{
register int i,j,k,x;

__m128d Ci0j0,Ci2j0,Ci4j0,Ci6j0,bkj;

for (i=0;i<n;i+=8)
    for (j=0;j<n;++j)
    {
	Ci0j0 = _mm_load_pd(C+i+0+j*n);
	Ci2j0 = _mm_load_pd(C+i+2+j*n);
	Ci4j0 = _mm_load_pd(C+i+4+j*n);
	Ci6j0 = _mm_load_pd(C+i+6+j*n);

	for(k=0;k<n;++k)
	{
	    bkj = _mm_load1_pd(B+k+j*n);

	    Ci0j0 = _mm_add_pd(Ci0j0,_mm_mul_pd(_mm_load_pd(A+i+n*k+0), bkj));
	    Ci2j0 = _mm_add_pd(Ci2j0,_mm_mul_pd(_mm_load_pd(A+i+n*k+2), bkj));
	    Ci4j0 = _mm_add_pd(Ci4j0,_mm_mul_pd(_mm_load_pd(A+i+n*k+4), bkj));
	    Ci6j0 = _mm_add_pd(Ci6j0,_mm_mul_pd(_mm_load_pd(A+i+n*k+6), bkj));

	}

	_mm_store_pd(C+i+0+j*n, Ci0j0);
	_mm_store_pd(C+i+2+j*n, Ci2j0);
	_mm_store_pd(C+i+4+j*n, Ci4j0);
	_mm_store_pd(C+i+6+j*n, Ci6j0);

    }
}

//-------------------------------------------------------------------------------

void dgemm_sse_16x1 (int n, double *A, double *B, double *C)
{
register int i,j,k,x;

__m128d Ci0j0,Ci2j0,Ci4j0,Ci6j0,Ci8j0,Ci10j0,Ci12j0,Ci14j0,bkj;

for (i=0;i<n;i+=16)
    for (j=0;j<n;++j)
    {
	Ci0j0 = _mm_load_pd(C+i+0+j*n);
	Ci2j0 = _mm_load_pd(C+i+2+j*n);
	Ci4j0 = _mm_load_pd(C+i+4+j*n);
	Ci6j0 = _mm_load_pd(C+i+6+j*n);
	Ci8j0 = _mm_load_pd(C+i+8+j*n);
	Ci10j0 = _mm_load_pd(C+i+10+j*n);
	Ci12j0 = _mm_load_pd(C+i+12+j*n);
	Ci14j0 = _mm_load_pd(C+i+14+j*n);

	for(k=0;k<n;++k)
	{
	    bkj = _mm_load1_pd(B+k+j*n);

	    Ci0j0 = _mm_add_pd(Ci0j0,_mm_mul_pd(_mm_load_pd(A+i+n*k+0), bkj));
	    Ci2j0 = _mm_add_pd(Ci2j0,_mm_mul_pd(_mm_load_pd(A+i+n*k+2), bkj));
	    Ci4j0 = _mm_add_pd(Ci4j0,_mm_mul_pd(_mm_load_pd(A+i+n*k+4), bkj));
	    Ci6j0 = _mm_add_pd(Ci6j0,_mm_mul_pd(_mm_load_pd(A+i+n*k+6), bkj));
	    Ci8j0 = _mm_add_pd(Ci8j0,_mm_mul_pd(_mm_load_pd(A+i+n*k+8), bkj));
	    Ci10j0 = _mm_add_pd(Ci10j0,_mm_mul_pd(_mm_load_pd(A+i+n*k+10), bkj));
	    Ci12j0 = _mm_add_pd(Ci12j0,_mm_mul_pd(_mm_load_pd(A+i+n*k+12), bkj));
	    Ci14j0 = _mm_add_pd(Ci14j0,_mm_mul_pd(_mm_load_pd(A+i+n*k+14), bkj));
	}

	_mm_store_pd(C+i+0+j*n, Ci0j0);
	_mm_store_pd(C+i+2+j*n, Ci2j0);
	_mm_store_pd(C+i+4+j*n, Ci4j0);
	_mm_store_pd(C+i+6+j*n, Ci6j0);
	_mm_store_pd(C+i+8+j*n, Ci8j0);
	_mm_store_pd(C+i+10+j*n, Ci10j0);
	_mm_store_pd(C+i+12+j*n, Ci12j0);
	_mm_store_pd(C+i+14+j*n, Ci14j0);

    }
}

//-------------------------------------------------------------------------------

void	dgemm_avx_4x1 (int n, double *A, double *B, double *C)
{
register int i,j,k;
__m256d Ci0j0,bkj;

for (i=0;i<n;i+=4)
    for (j=0;j<n;++j)
    {
	Ci0j0 = _mm256_load_pd(C+i+0+j*n);

	for (k=0;k<n;++k)
	{
	    bkj = _mm256_broadcast_sd(B+k+j*n);

            Ci0j0 = _mm256_fmadd_pd(_mm256_load_pd(A+i+0+k*n),bkj,Ci0j0);
	}

	_mm256_store_pd(C+i+0+j*n, Ci0j0);

    }
}

//-------------------------------------------------------------------------------

void dgemm_avx_4x4 (int n, double *A, double *B, double *C)
{
register int i,j,k,x;

__m256d a0,Ci0j0,Ci0j1,Ci0j2,Ci0j3,bk0j0,bk0j1,bk0j2,bk0j3;

for (i=0;i<n;i+=4)
    for (j=0;j<n;j+=4)
    {
	Ci0j0 = _mm256_load_pd(C+i+(j+0)*n);
	Ci0j1 = _mm256_load_pd(C+i+(j+1)*n);
	Ci0j2 = _mm256_load_pd(C+i+(j+2)*n);
	Ci0j3 = _mm256_load_pd(C+i+(j+3)*n);

	for(k=0;k<n;++k)
	{
	    a0 = _mm256_load_pd(A+i+k*n);

	    bk0j0 = _mm256_broadcast_sd(B+k+(j+0)*n);
	    bk0j1 = _mm256_broadcast_sd(B+k+(j+1)*n);
	    bk0j2 = _mm256_broadcast_sd(B+k+(j+2)*n);
	    bk0j3 = _mm256_broadcast_sd(B+k+(j+3)*n);

            Ci0j0 = _mm256_fmadd_pd(a0,bk0j0,Ci0j0);
            Ci0j1 = _mm256_fmadd_pd(a0,bk0j1,Ci0j1);
            Ci0j2 = _mm256_fmadd_pd(a0,bk0j2,Ci0j2);
            Ci0j3 = _mm256_fmadd_pd(a0,bk0j3,Ci0j3);
	}

	_mm256_store_pd(C+i+(j+0)*n, Ci0j0);
	_mm256_store_pd(C+i+(j+1)*n, Ci0j1);
	_mm256_store_pd(C+i+(j+2)*n, Ci0j2);
	_mm256_store_pd(C+i+(j+3)*n, Ci0j3);

    }
}

//-------------------------------------------------------------------------------

void	dgemm_avx_8x1 (int n, double *A, double *B, double *C)
{
register int i,j,k;
__m256d Ci0j0,Ci4j0,bkj;

for (i=0;i<n;i+=8)
    for (j=0;j<n;++j)
    {
	Ci0j0 = _mm256_load_pd(C+i+0+j*n);
	Ci4j0 = _mm256_load_pd(C+i+4+j*n);

	for (k=0;k<n;++k)
	{
	    bkj = _mm256_broadcast_sd(B+k+j*n);

            Ci0j0 = _mm256_fmadd_pd(_mm256_load_pd(A+i+0+k*n),bkj,Ci0j0);
            Ci4j0 = _mm256_fmadd_pd(_mm256_load_pd(A+i+4+k*n),bkj,Ci4j0);
	}

	_mm256_store_pd(C+i+0+j*n, Ci0j0);
	_mm256_store_pd(C+i+4+j*n, Ci4j0);

    }
}

//-------------------------------------------------------------------------------

void dgemm_avx_8x8 (int n, double *A, double *B, double *C)
{
register int i,j,k,x;

__m256d	Ci0j0,Ci0j1,Ci0j2,Ci0j3,Ci0j4,Ci0j5,Ci0j6,Ci0j7,bk0j0,bk0j1,bk0j2,bk0j3,a0;
__m256d	Ci4j0,Ci4j1,Ci4j2,Ci4j3,Ci4j4,Ci4j5,Ci4j6,Ci4j7,bk0j4,bk0j5,bk0j6,bk0j7,a4;

for (i=0;i<n;i+=8)
    for (j=0;j<n;j+=8)
    {
	Ci0j0 = _mm256_load_pd(C+i+(j+0)*n);
	Ci0j1 = _mm256_load_pd(C+i+(j+1)*n);
	Ci0j2 = _mm256_load_pd(C+i+(j+2)*n);
	Ci0j3 = _mm256_load_pd(C+i+(j+3)*n);
	Ci0j4 = _mm256_load_pd(C+i+(j+4)*n);
	Ci0j5 = _mm256_load_pd(C+i+(j+5)*n);
	Ci0j6 = _mm256_load_pd(C+i+(j+6)*n);
	Ci0j7 = _mm256_load_pd(C+i+(j+7)*n);
	Ci4j0 = _mm256_load_pd(C+i+4+(j+0)*n);
	Ci4j1 = _mm256_load_pd(C+i+4+(j+1)*n);
	Ci4j2 = _mm256_load_pd(C+i+4+(j+2)*n);
	Ci4j3 = _mm256_load_pd(C+i+4+(j+3)*n);
	Ci4j4 = _mm256_load_pd(C+i+4+(j+4)*n);
	Ci4j5 = _mm256_load_pd(C+i+4+(j+5)*n);
	Ci4j6 = _mm256_load_pd(C+i+4+(j+6)*n);
	Ci4j7 = _mm256_load_pd(C+i+4+(j+7)*n);

	for(k=0;k<n;++k)
	{
	    a0 = _mm256_load_pd(A+i+k*n);
	    a4 = _mm256_load_pd(A+i+4+k*n);

	    bk0j0 = _mm256_broadcast_sd(B+k+(j+0)*n);
	    bk0j1 = _mm256_broadcast_sd(B+k+(j+1)*n);
	    bk0j2 = _mm256_broadcast_sd(B+k+(j+2)*n);
	    bk0j3 = _mm256_broadcast_sd(B+k+(j+3)*n);
	    bk0j4 = _mm256_broadcast_sd(B+k+(j+4)*n);
	    bk0j5 = _mm256_broadcast_sd(B+k+(j+5)*n);
	    bk0j6 = _mm256_broadcast_sd(B+k+(j+6)*n);
	    bk0j7 = _mm256_broadcast_sd(B+k+(j+7)*n);

            Ci0j0 = _mm256_fmadd_pd(a0,bk0j0,Ci0j0);
            Ci0j1 = _mm256_fmadd_pd(a0,bk0j1,Ci0j1);
            Ci0j2 = _mm256_fmadd_pd(a0,bk0j2,Ci0j2);
            Ci0j3 = _mm256_fmadd_pd(a0,bk0j3,Ci0j3);
            Ci0j4 = _mm256_fmadd_pd(a0,bk0j4,Ci0j4);
            Ci0j5 = _mm256_fmadd_pd(a0,bk0j5,Ci0j5);
            Ci0j6 = _mm256_fmadd_pd(a0,bk0j6,Ci0j6);
            Ci0j7 = _mm256_fmadd_pd(a0,bk0j7,Ci0j7);
            Ci4j0 = _mm256_fmadd_pd(a4,bk0j0,Ci4j0);
            Ci4j1 = _mm256_fmadd_pd(a4,bk0j1,Ci4j1);
            Ci4j2 = _mm256_fmadd_pd(a4,bk0j2,Ci4j2);
            Ci4j3 = _mm256_fmadd_pd(a4,bk0j3,Ci4j3);
            Ci4j4 = _mm256_fmadd_pd(a4,bk0j4,Ci4j4);
            Ci4j5 = _mm256_fmadd_pd(a4,bk0j5,Ci4j5);
            Ci4j6 = _mm256_fmadd_pd(a4,bk0j6,Ci4j6);
            Ci4j7 = _mm256_fmadd_pd(a4,bk0j7,Ci4j7);

	}

	_mm256_store_pd(C+i+(j+0)*n, Ci0j0);
	_mm256_store_pd(C+i+(j+1)*n, Ci0j1);
	_mm256_store_pd(C+i+(j+2)*n, Ci0j2);
	_mm256_store_pd(C+i+(j+3)*n, Ci0j3);
	_mm256_store_pd(C+i+(j+4)*n, Ci0j4);
	_mm256_store_pd(C+i+(j+5)*n, Ci0j5);
	_mm256_store_pd(C+i+(j+6)*n, Ci0j6);
	_mm256_store_pd(C+i+(j+7)*n, Ci0j7);
	_mm256_store_pd(C+i+4+(j+0)*n, Ci4j0);
	_mm256_store_pd(C+i+4+(j+1)*n, Ci4j1);
	_mm256_store_pd(C+i+4+(j+2)*n, Ci4j2);
	_mm256_store_pd(C+i+4+(j+3)*n, Ci4j3);
	_mm256_store_pd(C+i+4+(j+4)*n, Ci4j4);
	_mm256_store_pd(C+i+4+(j+5)*n, Ci4j5);
	_mm256_store_pd(C+i+4+(j+6)*n, Ci4j6);
	_mm256_store_pd(C+i+4+(j+7)*n, Ci4j7);

    }
}

//-------------------------------------------------------------------------------

void	dgemm_avx_16x1 (int n, double *A, double *B, double *C)
{
register int i,j,k;
__m256d Ci0j0,Ci4j0,Ci8j0,Ci12j0,bkj;

for (i=0;i<n;i+=16)
    for (j=0;j<n;++j)
    {
	Ci0j0 = _mm256_load_pd(C+i+0+j*n);
	Ci4j0 = _mm256_load_pd(C+i+4+j*n);
	Ci8j0 = _mm256_load_pd(C+i+8+j*n);
	Ci12j0 = _mm256_load_pd(C+i+12+j*n);

	for (k=0;k<n;++k)
	{
	    bkj = _mm256_broadcast_sd(B+k+j*n);

            Ci0j0 = _mm256_fmadd_pd(_mm256_load_pd(A+i+k*n),bkj,Ci0j0);
            Ci4j0 = _mm256_fmadd_pd(_mm256_load_pd(A+i+k*n+4),bkj,Ci4j0);
            Ci8j0 = _mm256_fmadd_pd(_mm256_load_pd(A+i+k*n+8),bkj,Ci8j0);
            Ci12j0 = _mm256_fmadd_pd(_mm256_load_pd(A+i+k*n+12),bkj,Ci12j0);
	}

	_mm256_store_pd(C+i+0+j*n, Ci0j0);
	_mm256_store_pd(C+i+4+j*n, Ci4j0);
	_mm256_store_pd(C+i+8+j*n, Ci8j0);
	_mm256_store_pd(C+i+12+j*n, Ci12j0);

    }
}

//-------------------------------------------------------------------------------

void	r2c (int n, double *A, double *AT)
{
register int i,j;

for (i=0;i<n;++i)
    for (j=0;j<n;++j)
	AT[j+i*n]=A[i+j*n];
}

//-------------------------------------------------------------------------------
