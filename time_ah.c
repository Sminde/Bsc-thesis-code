/*
   +++++++++++++++++++++    ah time routines         ++++++++++++++++=+++
*/

/*#include "complex.h"*/
#include "time_ah.h"

#define mod(a,b)	a - ((int)(a/b)) * b

static int days_in_month[]= {31,28,31,30,31,30,31,31,30,31,30,31,31};

void
etoh(tt, epoch)
/* fill the time structure t with values equivalent to epochal time epoch */
register struct ah_time *tt;
double epoch;
{
	int diy;
        struct ah_time t;
	double secleft;
	long days;
	int isleap();
	void month_day();

	days= (long) (epoch/86400.);
	secleft= mod(epoch, 86400.0);
	t.hr= t.mn= 0;
	t.sec= 0.;

	if (secleft) {			/* compute hours minutes seconds */
		if (secleft < 0) {	/* before 1970 */
			days--;		/* subtract a day */
			secleft+= 86400.;	/* add a day */
		}
		t.hr= (int) (secleft/3600.);
		secleft= mod(secleft, 3600.0);
		t.mn= (int) (secleft/60.);
		t.sec= mod(secleft, 60.0);

	}

	if (days >= 0){
		for (t.yr= 1970; ; t.yr++) {
			diy= isleap(t.yr) ? 366 : 365;
			if (days < diy) break;
			days-= diy;
		}
	}
	else {
		for (t.yr= 1969; ; t.yr--) {
			diy= isleap(t.yr) ? 366 : 365;
			days+= diy;
			if (days >= 0) break;
		}
	}
	days++;
	month_day(&t, (int) days);
        *tt=t;
	return;
}

void
month_day(t, jul_day)
register struct ah_time *t;
int jul_day;
{
	int i, dim, leap;
	int isleap();

	leap= isleap(t->yr);
	t->day= jul_day;
	for (i= 0; i < 12; i ++) {
		dim= days_in_month[i];
		if (leap && (i == 1)) dim++;
		if (t->day <= dim ) break;
		t->day-= dim;
	}
	t->mo= i+1;
	return;
}

int
julian(t)
register struct ah_time *t;
{
	int i, j, inc;

	j= 0;
	for (i= 0; i < t->mo-1; i++){
		inc= days_in_month[i];
		if ((i == 1) && isleap(t->yr)) inc++;
		j+= inc;
	}
	j+= t->day;
	return(j);
}

/*double*/
void
htoe(t, epoch)
/* calculate epochal equivalent of time t */
register struct ah_time *t;
double *epoch;
{
	long i, days;
	/*double epoch;*/

	days= 0;
	if (t->yr > 1970){
		for (i= 1970; i < t->yr; i++) {
			days+= 365;
			if (isleap(i))  days++;
		}
	}
	else if (t->yr < 1970) {
		for (i= t->yr; i < 1970; i++) {
			days-= 365;
			if (isleap(i)) days--;
		}
	}
	days+= (long) (julian(t)-1);
	*epoch= (((double) days)*86400.);
	*epoch+= (double) ((t->hr*3600.)+(t->mn*60.)+(t->sec));
	/*return(epoch);*/
}

isleap(year)
/* return true if leap year else false */
int year;
{
	return(year % 4 == 0 && year % 100 != 0 || year % 400 == 0);
}
