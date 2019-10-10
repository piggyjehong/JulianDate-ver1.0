#include <iostream>
#include <time.h>
#include <string>
#include <cmath>
using namespace std;

class JulianDate {
private:
    static int EPOCH; //	int mon, day, year, hour, min, second;
    double jday; // number of days since epoch
    int yLeap =0;
    int mLeap =0;
    const static int cumulativeDaysUpToMonth[13];
    const static int cumulativeDaysUpToMonthLeap[13];
    const static string monthNames[12];
    JulianDate(double jday): jday(jday){}

public:
    friend ostream& operator <<(ostream& s, JulianDate d){
        return s << d.getYear() << "-" << d.getMonth()<< "-" << d.getDay() << "-" <<d.getHour() << '-' << d.getMin() << "-" <<d.getSec();
    }

    friend JulianDate operator + (JulianDate d, int days){
        JulianDate res(d.jday + days);
        res.yLeap=d.yLeap;
        res.mLeap=d.mLeap;
        return res;
    }

    friend JulianDate operator - (JulianDate d, int days){
        JulianDate res(d.jday - days);
        res.yLeap=d.yLeap;
        res.mLeap=d.mLeap;
        return res;
    }

    friend int operator - (JulianDate d1, JulianDate d2){
        return d1.jday - d2.jday;
    }

    JulianDate(){
        time_t t = time(NULL);
        struct tm *local = localtime(&t);
        new(this)JulianDate(local-> tm_year + 1900, local-> tm_mon +1, local->tm_mday, local-> tm_hour, local-> tm_min, local->tm_sec);
    }

    JulianDate(int year, int mon, int day, int hour, int min, int sec) {
        int dy = year - EPOCH;
        int days = 365 * dy - 1;
        for(int i = EPOCH; i < year; i++){
            if((i % 4 == 0 && i % 100 != 0) || i % 400 == 0){
                days ++;
                yLeap ++;
                mLeap ++;
            }
        }
        if ((year%4 == 0 && year %100 != 0) || year % 400 ==0){
            if(mon >= 3){
                days ++;
                yLeap ++;
            }
        }

        double seconds = (hour * 60.0 * 60.0 + min * 60.0 + sec) / 86400.0;
        days += cumulativeDaysUpToMonth[mon-1];
        jday = days + day + seconds;
    }

    int getYear() const{
        int i = this-> jday;
        int j = (i - yLeap)/ 365;
        return j + EPOCH;
    }

    string getMonth() const{
        int i = this->jday;
        int j = 0;
        int dy = getYear()-EPOCH;
        j = jday - dy * 365 - mLeap;

        if ((getYear()%4 == 0 && getYear() %100 != 0) || getYear() % 400 ==0){
            for (int k = 0; k < 12; ++k) {
                if(j >= cumulativeDaysUpToMonthLeap[k] and j < cumulativeDaysUpToMonthLeap[k+1]){
                    return monthNames[k];
                }
            }
        }
        else{
            for (int k = 0; k < 12; ++k) {
                if(j >= cumulativeDaysUpToMonth[k] and j < cumulativeDaysUpToMonth[k+1]){
                    return monthNames[k];
                }
            }
        }
        return "";
    }

    int getDay() const{
        int i = this->jday;
        int j = 0;
        int dy = getYear()-EPOCH;
        j = jday - dy * 365 - mLeap;
        if ((getYear()%4 == 0 && getYear() %100 != 0) || getYear() % 400 ==0){
            for (int k = 0; k < 12; ++k) {
                if (getMonth() == monthNames[k]){
                    return j - cumulativeDaysUpToMonthLeap[k] + 1;
                }
            }
        }
        else{
            for (int k = 0; k < 12; ++k) {
                if (getMonth() == monthNames[k]){
                    return j - cumulativeDaysUpToMonth[k] + 1;
                }
            }
        }
        return 0;
    }

    int getHour() const{
        double i = this->jday;
        int j = i/1;
        int k = round((i - j)* 86400);
        return k / 3600;
    }

    int getMin() const{
        double i = this->jday;
        int j = i/1;
        int k = round((i - j)* 86400) - getHour()*3600;
        return k / 60;
    }

    int getSec() const{
        double i = this->jday;
        int j = i/1;
        int k = round((i - j)* 86400) - getHour()*3600 - getMin()*60;
        return k;
    }
};

const int JulianDate::cumulativeDaysUpToMonth[13] =
        {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
const int JulianDate::cumulativeDaysUpToMonthLeap[13] =
        {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366};
const string JulianDate::monthNames[12] =
        {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

int JulianDate::EPOCH = 2000;

int main() {
    JulianDate newyear(2018, 1, 1, 0,0,0);
    JulianDate valentine(2018, 2, 14, 12,0,0); // 0.5
    JulianDate today; // get it from the system time: time(nullptr)
    // localtime

    double days = valentine - newyear;
    JulianDate due = today + 7;
    cout << due << '\n';

    cout << "\nyear: " << newyear.getYear()
         << "\nmonth: " << newyear.getMonth()
         << "\nday: " << newyear.getDay()
         << "\nhour: " << newyear.getHour()
         << "\nmin: " << newyear.getMin()
         << "\nsec: " << newyear.getSec() << '\n';
    JulianDate d1(2019, 1, 1, 0,0,0);
    for (int i = 0; i < 100; i++)
        cout << d1 + i;
}


