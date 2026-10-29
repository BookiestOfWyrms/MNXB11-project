#include <cmath>

#include "Analysis.h"

namespace {
// The reference calendar has 29 days in February.
const int monthLengths[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
}  // namespace

void DailyTemperature::add(double temperature) {
  sum += temperature;
  ++count;
}

double DailyTemperature::mean() const {
  // Precondition: count > 0; analysis functions check this before calling.
  return static_cast<double>(sum / count);
}

bool isLeapYear(int year) {
  return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
}

bool validDate(int year, int month, int day) {
  if (year < 1 || year > 9999 || month < 1 || month > 12 || day < 1) {
    return false;
  }
  int lastDay = monthLengths[month - 1];
  if (month == 2 && !isLeapYear(year)) {
    lastDay = 28;
  }
  return day <= lastDay;
}

int calendarDay(int month, int day) {
  // Precondition: a valid month/day on the reference leap calendar.
  int slot = day;
  for (int m = 1; m < month; ++m) {
    slot += monthLengths[m - 1];
  }
  return slot;
}

std::string calendarDate(int slot) {
  // Precondition: slot is between 1 and 366.
  int month = 1;
  while (slot > monthLengths[month - 1]) {
    slot -= monthLengths[month - 1];
    ++month;
  }
  return std::to_string(slot) + "/" + std::to_string(month);
}

int observedDays(const YearTemperatures& year) {
  int count = 0;
  for (int slot = 1; slot <= 366; ++slot) {
    if (year[slot].count > 0) {
      ++count;
    }
  }
  return count;
}

Summary summarize(const std::vector<double>& values) {
  Summary result;
  result.count = static_cast<int>(values.size());
  if (result.count == 0) {
    return result;  // The caller must not interpret empty results as data.
  }

  for (int i = 0; i < result.count; ++i) {
    result.mean += values[i];
  }
  result.mean /= result.count;

  // Two passes make the formula easy to compare with a textbook.
  // This is the DESCRIPTIVE (population) SD, with denominator N, not N-1.
  double squaredDeviations = 0.0;
  for (int i = 0; i < result.count; ++i) {
    double difference = values[i] - result.mean;
    squaredDeviations += difference * difference;
  }
  result.standardDeviation = std::sqrt(squaredDeviations / result.count);
  return result;
}
