#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "time_ah.h"


int main(int argc, char *argv[])
{
 FILE *in, *out, *azfile;

 char buffer[200];
 char isotime[30];
 char stat[5];
 struct ah_time tm, dtm;
 double epotime, starttime, aztime;
// int dstart, dstop;
 double dstart, dstop;
 float dur;
 float coh, azi, cohp, azip;
 float samprate;

// strcpy(stat,argv[3]);
 in=fopen(argv[1],"r");
 out=fopen("det2time.out","w");

 while(fgets(buffer,200,in))
 {
   if(strstr(buffer,"starttime"))
   {
     sscanf(buffer,"%*s %s",isotime);
   }

   if(strstr(buffer,"sampling_rate"))
   {
      sscanf(buffer,"%*s %f",&samprate);
   }

   if(strstr(buffer,"processing"))
    break;
 }

sscanf(isotime,"%4d%*c%2d%*c%2d%*c%2d%*c%2d%*c%2f",&tm.yr,&tm.mo,&tm.day,&tm.hr,&tm.mn,&tm.sec);
htoe(&tm,&starttime);

while(fgets(buffer,200,in))
 {
   sscanf(buffer,"%lf %lf",&dstart,&dstop);
//   dur=(dstop-dstart)/samprate;
//   epotime=starttime+(dstart/samprate)-0.05;
//   epotime=starttime+(dstart/samprate);
   dur=(float)(dstop-dstart);
   epotime=dstart;
   etoh(&dtm,epotime);

//   if(dur>0.5)
    fprintf(out,"%lf %lf %02d.%02d.%4d %02d:%02d:%05.2f %6.2f\n",dstart,dstop,dtm.day,dtm.mo,dtm.yr,dtm.hr,dtm.mn,dtm.sec,dur);

 }

fclose(in);
fclose(out);
}

 
