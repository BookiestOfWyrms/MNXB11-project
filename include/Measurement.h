#ifndef MEASUREMENT_H
#define MEASUREMENT_H
#include <cmath>
#include <stdexcept>
class Measurement {
 public:
 Measurement(int year,int month, int day, int hour, int minute, 
  int second, double temp, char quality);

  bool is_valid() const;
  int get_year() const;
  int get_month() const;
  int get_day() const;
  int get_hour() const;
  int get_minute() const;
  int get_second() const;
  double get_temp() const;
  char get_quality() const;

 private:
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int second;
  double temp;
  char quality;
};

#endif /* MEASUREMENT_H */