
#ifndef _DATE
#define _DATE

class Date {
private:

int day;
int month;
int year;

int julianDay(Date d)
{
 int a, y, m;

 a = (14-d.getMonth())/12;
 y = d.getYear()+4800-a;
 m = d.getMonth()+12*a-3;
 return d.getDay() + (153*m+2)/5+365*y + y/4 - y/100 + y/400 - 32045;
}

public:

Date(void) {day = 1; month = 1; year = 2000;}
int getDay(void) {return day;}
int getMonth(void) {return month;}
int getYear(void) {return year;}

void setDate(int d, int m, int y)
{
 if (y<1 || m<1 || m>12 || d<1) return;
 if ( m==1 || m==3 || m==5 || m==7 || m==8 || m==10 || m==12)
   if (d>31) return;
 if (m==4 || m==6 || m==9 || m==11)
   if (d>30) return;
 if (m==2 && y%4==0) // bisestile
   if (d>29) return;
 if (m==2 && y%4!=0) // non bisestile
   if (d>28) return;
 year = y;
 month = m;
 day = d;
}

int diff(Date d) {
 int my_jd, jd;

 my_jd = julianDay(*this);
 jd = julianDay(d);
 return (jd - my_jd);
}
};

#endif
