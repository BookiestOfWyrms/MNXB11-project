#include <TCanvas.h>
#include <TH1D.h>
#include <TLegend.h>

#include <algorithm>
#include <cmath>
#include <iostream>

#include "Analysis.h"
#include "GraphStyle1.h"

void analyzeGivenDay(const TemperatureData& data, int month, int day,
                     double probabilityLow, double probabilityHigh) {
  if (!validDate(2000, month, day)) {
    std::cerr << "Selected calendar date is invalid.\n";
    return;
  }
  int slot = calendarDay(month, day);
  if (!std::isfinite(probabilityLow) || !std::isfinite(probabilityHigh) ||
      probabilityLow >= probabilityHigh) {
    std::cerr << "Probability interval must have finite low < high.\n";
    return;
  }

  std::vector<double> temperatures;
  for (const auto& year : data) {
    if (year.second[slot].count > 0) {
      temperatures.push_back(year.second[slot].mean());
    }
  }
  if (temperatures.empty()) {
    std::cout << "\n3.1: No observations for " << calendarDate(slot) << ".\n";
    return;
  }

  Summary summary = summarize(temperatures);
  int inInterval = 0;
  for (int i = 0; i < summary.count; ++i) {
    if (temperatures[i] >= probabilityLow &&
        temperatures[i] < probabilityHigh) {
      ++inInterval;
    }
  }

  std::cout << "\n3.1: Daily-mean temperature on " << calendarDate(slot)
            << " across " << summary.count << " observed years\n"
            << "Mean = " << summary.mean << " C\n"
            << "Descriptive SD = " << summary.standardDeviation << " C\n"
            << "Historical fraction in [" << probabilityLow << ", "
            << probabilityHigh
            << ") C = " << static_cast<double>(inInterval) / summary.count
            << " (" << inInterval << "/" << summary.count << ")\n";
  // Create the graph
  std::string title = " ;Mean Temperature (#circC); Counts (years)";
  TH1D* histogram = new TH1D("given_day", title.c_str(), 60, 0, 30);
  histogram->SetDirectory(nullptr);
  for (int i = 0; i < summary.count; ++i) {
    histogram->Fill(temperatures[i]);
  }
  new TCanvas("given_day_canvas", "3.1: Selected day", 900, 600);

  histogram->SetFillColor(kAzure - 8);
  histogram->SetLineColor(kAzure + 2);
  GraphStyle1(histogram);
  histogram->Draw("HIST");

  auto legend = new TLegend(0.78, 0.85, 0.86, 0.91);
  legend->AddEntry(histogram, calendarDate(slot).c_str(), "f");
  legend->Draw();
}