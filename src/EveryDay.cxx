#include "Analysis.h"

#include <TAxis.h>
#include <TCanvas.h>
#include <TGraphErrors.h>

#include <iostream>

void analyzeEveryDay(const TemperatureData& data) {
    TGraphErrors* graph = new TGraphErrors();
    graph->SetName("calendar_means");
    graph->SetTitle("Daily means across years;Calendar slot (leap-year scale);Temperature [#circC]");
    graph->SetMarkerStyle(20);
    graph->SetMarkerSize(0.4);

    int point = 0;
    int singleYearDates = 0;
    std::cout << "\n3.2: Calendar date, observed years, mean [C], descriptive SD [C]\n";
    for (int slot = 1; slot <= 366; ++slot) {
        std::vector<double> temperatures;
        for (const auto& year : data) {
            if (year.second[slot].count > 0) {
                temperatures.push_back(year.second[slot].mean());
            }
        }
        if (temperatures.empty()) {
            continue;  // No fake zero-valued points for unobserved dates.
        }
        Summary summary = summarize(temperatures);
        graph->SetPoint(point, slot, summary.mean);
        // These bars show year-to-year SD, NOT SD/sqrt(N) or measurement error.
        graph->SetPointError(point, 0.0, summary.standardDeviation);
        ++point;
        if (summary.count == 1) {
            ++singleYearDates;
        }
        std::cout << calendarDate(slot) << "  " << summary.count << "  "
                  << summary.mean << "  " << summary.standardDeviation << '\n';
    }

    std::cout << point << " calendar dates plotted; " << singleYearDates
              << " have only one observed year (zero descriptive SD).\n";
    new TCanvas("every_day_canvas", "3.2: Every calendar date", 1100, 600);
    graph->Draw("AP");
    graph->GetXaxis()->SetLimits(0.5, 366.5);
}