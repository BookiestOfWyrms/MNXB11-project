#ifndef MNXB11_ANALYSIS_H
#define MNXB11_ANALYSIS_H

#include <map>
#include <string>
#include <vector>

// Each object accumulates the readings for ONE actual date in ONE year.
// If the input already contains daily means, count will normally be 1.
struct DailyTemperature {
  // long double keeps extra precision in the daily sum on COSMOS.
  long double sum = 0.0L;
  long long count = 0;

  void add(double temperature);
  double mean() const;  // Only call when count > 0.
};

// A vector has 367 elements. Index 0 is unused; 1..366 are calendar slots.
// Every year uses the same leap-year calendar: Jan 1 = 1, Feb 29 = 60,
// Mar 1 = 61, Jul 1 = 183, Dec 31 = 366. Non-leap years have slot 60 empty.
// This aligns month/day between years WITHOUT deleting February 29.
typedef std::vector<DailyTemperature> YearTemperatures;
// The map key is the year, e.g., 2020. The value is the vector of daily
// rreadings. I don't want to use a vector because I don't know what format the
// data will be in.
typedef std::map<int, YearTemperatures> TemperatureData;

struct Summary {
  int count = 0;
  double mean = 0.0;
  double standardDeviation = 0.0;
};

bool isLeapYear(int year);
bool validDate(int year, int month, int day);
int calendarDay(int month, int day);
std::string calendarDate(int slot);
int observedDays(const YearTemperatures& year);
Summary summarize(const std::vector<double>& values);

// Read the cleaned-up data, hopefully in the expected format so no debugging
// headaches Ahahahaha. Return false with a printed message when the input
// cannot be read.
bool readTemperatures(const char* filename, TemperatureData& data);

// The three requested subsections, followed by the independent fourth idea.
void analyzeGivenDay(const TemperatureData& data, int month, int day,
                     double probabilityLow, double probabilityHigh);
void analyzeEveryDay(const TemperatureData& data);
void analyzeYearExtremes(const TemperatureData& data);
void compareYears(const TemperatureData& data, int earlierYear, int laterYear);

#endif
