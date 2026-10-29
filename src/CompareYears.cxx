#include "Analysis.h"
#include "GraphStyle2.h"

#include <TAxis.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TLegend.h>
#include <TMultiGraph.h>
#include <TF1.h>
#include <TMath.h>

#include <iostream>
#include <vector>

namespace {
void describeYear(int year, const std::vector<double>& temperatures) {
    Summary summary = summarize(temperatures);
    std::cout << year << ": " << summary.count << '/'
              << (isLeapYear(year) ? 366 : 365) << " observed days; mean = "
              << summary.mean << " C; SD across daily means = "
              << summary.standardDeviation << " C\n";
}
}

void compareYears(const TemperatureData& data, int earlierYear, int laterYear) {
    if (earlierYear >= laterYear) {
        std::cerr << "Comparison needs earlierYear < laterYear.\n";
        return;
    }
    std::cout << "\nFourth analysis: " << earlierYear << " versus " << laterYear
              << " (" << laterYear - earlierYear << " years apart)\n";

    // find() does NOT insert a missing year; data[year] would insert one.
    auto earlier = data.find(earlierYear);
    auto later = data.find(laterYear);
    if (earlier == data.end() || later == data.end()) {
        std::cout << "Comparison unavailable: at least one requested year is absent.\n"
                  << "Available years:";
        for (const auto& year : data) {
            std::cout << ' ' << year.first;
        }
        std::cout << "\nChoose two available years in main.cxx. No substitute was chosen.\n";
        return;
    }

    TGraph* earlyGraph = new TGraph();
    TGraph* lateGraph = new TGraph();
    TGraph* differenceGraph = new TGraph();
    earlyGraph->SetName("earlier_year_temperatures");
    lateGraph->SetName("later_year_temperatures");
    differenceGraph->SetName("same_date_differences");
    std::vector<double> earlyValues;
    std::vector<double> lateValues;
    std::vector<double> pairedEarly;
    std::vector<double> pairedLate;
    std::vector<double> differences;
    std::vector<double> monthlyDifferences[12];

    // Loop over actual month/day labels. February 29 is valid on the common
    // reference calendar, but contributes a pair only if BOTH years contain it.
    for (int month = 1; month <= 12; ++month) {
        for (int day = 1; validDate(2000, month, day); ++day) {
            int slot = calendarDay(month, day);
            bool hasEarly = earlier->second[slot].count > 0;
            bool hasLate = later->second[slot].count > 0;
            if (hasEarly) {
                double value = earlier->second[slot].mean();
                earlyGraph->SetPoint(earlyGraph->GetN(), slot, value);
                earlyValues.push_back(value);
            }
            if (hasLate) {
                double value = later->second[slot].mean();
                lateGraph->SetPoint(lateGraph->GetN(), slot, value);
                lateValues.push_back(value);
            }
            if (hasEarly && hasLate) {
                double first = earlier->second[slot].mean();
                double second = later->second[slot].mean();
                double difference = second - first;
                pairedEarly.push_back(first);
                pairedLate.push_back(second);
                differences.push_back(difference);
                monthlyDifferences[month - 1].push_back(difference);
                differenceGraph->SetPoint(differenceGraph->GetN(), slot, difference);
            }
        }
    }

    describeYear(earlierYear, earlyValues);
    describeYear(laterYear, lateValues);
    if (differences.empty()) {
        std::cout << "No shared observed month/day dates: paired differences are undefined.\n";
    } else {
        Summary first = summarize(pairedEarly);
        Summary second = summarize(pairedLate);
        Summary difference = summarize(differences);
        std::cout << "Same-date comparison on " << difference.count << " shared dates:\n"
                  << "Earlier mean = " << first.mean << " C; later mean = "
                  << second.mean << " C\n"
                  << "Mean difference (later - earlier) = " << difference.mean << " C\n"
                  << "Descriptive SD of daily differences = "
                  << difference.standardDeviation << " C\n"
                  << "Month, paired dates, mean difference [C]\n";
        for (int month = 1; month <= 12; ++month) {
            Summary monthly = summarize(monthlyDifferences[month - 1]);
            std::cout << month << "  " << monthly.count << "  ";
            if (monthly.count == 0) {
                std::cout << "unavailable\n";
            } else {
                std::cout << monthly.mean << '\n';
            }
        }
    }
    std::cout << "All-observed-day means can have different calendar coverage.\n"
              << "Paired results use the same dates; no gaps are interpolated.\n"
              << "Reading times differ between years in this dataset; simple daily\n"
              << "averages can retain time-of-day sampling bias.\n"
              << "This compares two individual years, not a century-long climate trend.\n";
    

    TCanvas* canvas = new TCanvas("comparison_canvas", "Fourth analysis: Two years", 1100, 850);
    canvas->Divide(1, 2);

    earlyGraph->SetMarkerSize(10);
    earlyGraph->SetMarkerColor(kBlue + 1);
    lateGraph->SetMarkerSize(10);
    lateGraph->SetMarkerColor(kRed + 1);
    differenceGraph->SetMarkerSize(10);

    canvas->cd(1);

    std::string title = ";Day of year (leap-year scale);Mean temperature (#circC)";
    earlyGraph->SetTitle(title.c_str());
    GraphStyle2(earlyGraph);
    earlyGraph->GetXaxis()->SetLimits(0.5, 366.5);
    earlyGraph->GetYaxis()->SetLimits(0.5, 366.5);
    
    earlyGraph->Draw("AP");
    lateGraph->Draw("P SAME");

    // Fit both set of data to compare trends
    TF1* earlyFit = new TF1("earlyFit", "[0] + [1]*sin(2*TMath::Pi()*(x-[2])/366.)", 1, 366);
    TF1* lateFit  = new TF1("lateFit",  "[0] + [1]*sin(2*TMath::Pi()*(x-[2])/366.)", 1, 366);
    earlyFit->SetParameters(10, 15, 100);
    lateFit->SetParameters(10, 15, 100);
    earlyFit->SetLineColor(kBlue + 1);
    lateFit->SetLineColor(kRed + 1);

    earlyGraph->Fit(earlyFit, "0Q", "", 1, 366);
    lateGraph->Fit(lateFit, "0Q", "", 1, 366);
    earlyFit->Draw("SAME");
    lateFit->Draw("SAME");


    TLegend* legend = new TLegend(0.82, 0.75, 0.88, 0.89);
    legend->AddEntry(earlyGraph, std::to_string(earlierYear).c_str(), "p");
    legend->AddEntry(lateGraph, std::to_string(laterYear).c_str(), "p");
    legend->Draw();

    canvas->cd(2);
    differenceGraph->SetTitle(";Day of year (leap-year scale); Temperature difference (#circC)");
    
    differenceGraph->GetXaxis()->SetLimits(0.5, 366.5);
    GraphStyle2(differenceGraph);

    differenceGraph->Draw("AP");

        // Mean of difference
    TF1* meanFit = new TF1("meanFit", "pol0", 1, 366);
    meanFit->SetLineColor(kBlack);
    differenceGraph->Fit(meanFit, "0Q", "", 1, 366);
    meanFit->Draw("SAME");

    // Global Fit difference
    TF1* globalFit = new TF1("globalFit", "[0] + [1]*sin(2*TMath::Pi()*(x-[2])/366.)", 1, 366);
    globalFit->SetParameters(0., 3., 0.);
    globalFit->SetLineColor(kGreen + 2);
    differenceGraph->Fit(globalFit, "0Q", "", 1, 366);
    globalFit->Draw("SAME");
}
