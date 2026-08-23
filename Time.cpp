#include "Time.h"

#include<ctime>
#include<iostream>
#include<iomanip>

using namespace std;

void Time::execute(istream& in, ostream& out) {
    time_t t = time(nullptr);
    tm localTime{};
    localtime_s(&localTime, &t);

    int hour = localTime.tm_hour;
    int min = localTime.tm_min;
    int sec = localTime.tm_sec;
    
    out << setw(2) << setfill('0') << hour << ":" << setw(2) << setfill('0') << min << ":" << setw(2) << setfill('0') << sec << endl;
}
