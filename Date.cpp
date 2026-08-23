#include "Date.h"

#include<ctime>
#include<iostream>

using namespace std;

void Date::execute(istream& in,ostream& out) {
    time_t t = time(nullptr);

    tm localTime{};
    localtime_s(&localTime, &t);

    int year = localTime.tm_year + 1900;
    int mon = localTime.tm_mon + 1;
    int day = localTime.tm_mday;

    out << day << "." << mon << "." << year << "." << endl;
}