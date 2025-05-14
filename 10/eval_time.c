//#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<sys/time.h>
#include<sys/resource.h>
#include "eval_time.h"

static struct rusage rp;
static struct timeval tp;
static struct timezone tzp;


void init_time()
{
  getrusage(RUSAGE_SELF, &rp);
  gettimeofday(&tp,&tzp);
}


void read_time(double* time_tabl)
{ 
  
  struct rusage rk;
  struct timeval tk;
  double systime, usrtime,daytime;

  getrusage(RUSAGE_SELF, &rk);
  gettimeofday(&tk, &tzp);
  
  systime = ( rk.ru_stime.tv_usec - rp.ru_stime.tv_usec ) * 1.0e-6 ;
  systime += rk.ru_stime.tv_sec - rp.ru_stime.tv_sec;

  usrtime = ( rk.ru_utime.tv_usec - rp.ru_utime.tv_usec ) * 1.0e-6 ;
  usrtime += rk.ru_utime.tv_sec - rp.ru_utime.tv_sec;
  
  daytime = ( tk.tv_usec - tp.tv_usec ) * 1.0e-6 + tk.tv_sec - tp.tv_sec ;
  
  time_tabl[0]=systime;
  time_tabl[1]=usrtime;
  time_tabl[2]=daytime;  
}
