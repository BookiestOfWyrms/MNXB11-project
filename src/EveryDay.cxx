#include "Analysis.h"

#include <TAxis.h>
#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TStyle.h>
#include <TLegend.h>

#include <iostream>

void analyzeEveryDay(const TemperatureData& data) {
    TGraphErrors* graph = new TGraphErrors();
    graph->SetName("calendar_means");
    graph->SetTitle(";Day of year;Temperature (#circC)");
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
    auto cv =new TCanvas("every_day_canvas", "3.2: Every calendar date", 1100, 600);
    cv->SetGridy();

    graph->GetXaxis()->CenterTitle();
    graph->GetYaxis()->CenterTitle();
    graph->GetXaxis()->SetLimits(0.5, 366.5);

    graph->SetFillColor(kAzure-5);
    graph->Draw("A3");

    TGraph* line=new TGraph(*graph);
    line->SetLineWidth(1.8);
    line->SetLineColor(kBlack);
    line->Draw("L SAME");

    auto legend = new TLegend(0.74, 0.82, 0.93, 0.94);
    legend->AddEntry(graph, "Standard deviation", "f");
    legend->AddEntry(line,"Mean temperature", "l");
    legend->Draw();
}