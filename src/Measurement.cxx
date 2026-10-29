#include "Measurement.h"
Measurement::Measurement(int year, int month, int day, int hour, int minute,
                         int second, double temp, char quality)
    : year{year},
      month{month},
      day{day},
      hour{hour},
      minute{minute},
      second{second},
      temp{temp},
      quality{quality} {}

int Measurement::get_year() const { return year; }

int Measurement::get_month() const { return month; }

int Measurement::get_day() const { return day; }

int Measurement::get_hour() const { return hour; }

int Measurement::get_minute() const { return minute; }

int Measurement::get_second() const { return second; }

double Measurement::get_temp() const { return temp; }

char Measurement::get_quality() const { return quality; }

bool Measurement::is_valid() const { return true; }