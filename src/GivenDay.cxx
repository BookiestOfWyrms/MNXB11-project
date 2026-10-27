#include "Analysis.h"

#include <TCanvas.h>
#include <TH1D.h>

#include <algorithm>
#include <cmath>
#include <iostream>

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
        if (temperatures[i] >= probabilityLow && temperatures[i] < probabilityHigh) {
            ++inInterval;
        }
    }

    std::cout << "\n3.1: Daily-mean temperature on " << calendarDate(slot)
              << " across " << summary.count << " observed years\n"
              << "Mean = " << summary.mean << " C\n"
              << "Descriptive SD = " << summary.standardDeviation << " C\n"
              << "Historical fraction in [" << probabilityLow << ", "
              << probabilityHigh << ") C = "
              << static_cast<double>(inInterval) / summary.count << " ("
              << inInterval << "/" << summary.count << ")\n";
    if (summary.count == 1) {
        std::cout << "Only one year: SD = 0 describes that one value; variability is unknown.\n";
    }

    // Choose the INITIAL range from the actual selected values, including all
    // of them. No cuts, smoothing, rebinning, or later refinement is applied.
    double low = std::floor(*std::min_element(temperatures.begin(), temperatures.end())) - 1.0;
    double high = std::ceil(*std::max_element(temperatures.begin(), temperatures.end())) + 1.0;
    std::string title = "Daily mean on " + calendarDate(slot) +
                        ";Daily mean temperature [#circC];Observed years";
    TH1D* histogram = new TH1D("given_day", title.c_str(), 60, low, high);
    histogram->SetDirectory(nullptr);
    for (int i = 0; i < summary.count; ++i) {
        histogram->Fill(temperatures[i]);
    }
    new TCanvas("given_day_canvas", "3.1: Selected day", 900, 600);
    histogram->Draw("HIST");
}