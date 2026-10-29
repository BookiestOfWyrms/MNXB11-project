#include "Analysis.h"
#include "GraphStyle2.h"

#include <TCanvas.h>
#include <TH1D.h>
#include <TLegend.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

namespace {
void printModes(const TH1D* histogram, const char* label, int years) {
    double maximum = histogram->GetMaximum();
    std::cout << label << " most frequent calendar date(s): ";
    for (int slot = 1; slot <= 366; ++slot) {
        // Tolerance is only for accumulated floating-point histogram weights.
        if (std::abs(histogram->GetBinContent(slot) - maximum) < 1.e-9) {
            std::cout << calendarDate(slot) << ' ';
        }
    }
    std::cout << "\nModal date's historical weight / observed years = "
              << maximum / years << '\n';
}

void printDates(const std::vector<int>& dates) {
    for (int i = 0; i < static_cast<int>(dates.size()); ++i) {
        std::cout << calendarDate(dates[i]) << ' ';
    }
}
}

void analyzeYearExtremes(const TemperatureData& data) {
    TH1D* warm = new TH1D("warmest_dates",
        ";Days of year (leap-year scale);Years (counts)",
        366, 0.5, 366.5);
    TH1D* cold = new TH1D("coldest_dates", "Coldest dates", 366, 0.5, 366.5);
    TH1D* winter = new TH1D("coldest_dates_july_axis",
        ";Days of year offset from July 1;Years (counts)",
        366, -0.5, 365.5);
    warm->SetDirectory(nullptr);
    cold->SetDirectory(nullptr);
    winter->SetDirectory(nullptr);
    // Tied extrema receive fractional weights, so double-valued bins are used.
    warm->Sumw2();
    cold->Sumw2();
    winter->Sumw2();

    std::cout << "\n3.3: Annual extremes of OBSERVED DAILY MEANS\n";
    for (const auto& year : data) {
        int coverage = observedDays(year.second);
        int expected = isLeapYear(year.first) ? 366 : 365;

        std::vector<int> warmDates;
        std::vector<int> coldDates;
        double maximum = 0.0;
        double minimum = 0.0;
        for (int slot = 1; slot <= 366; ++slot) {
            if (year.second[slot].count == 0) {
                continue;
            }
            double temperature = year.second[slot].mean();
            if (warmDates.empty() || temperature > maximum) {
                maximum = temperature;
                warmDates.clear();
                warmDates.push_back(slot);
            } else if (temperature == maximum) {
                warmDates.push_back(slot);
            }
            if (coldDates.empty() || temperature < minimum) {
                minimum = temperature;
                coldDates.clear();
                coldDates.push_back(slot);
            } else if (temperature == minimum) {
                coldDates.push_back(slot);
            }
        }

        // Each year contributes TOTAL weight 1 to each distribution, even
        // when several days share the extreme. Equality here is exact numeric
        // equality of calculated daily means; no temperature tolerance is set.
        double warmWeight = 1.0 / warmDates.size();
        double coldWeight = 1.0 / coldDates.size();
        for (int i = 0; i < static_cast<int>(warmDates.size()); ++i) {
            warm->Fill(warmDates[i], warmWeight);
        }
        for (int i = 0; i < static_cast<int>(coldDates.size()); ++i) {
            int slot = coldDates[i];
            cold->Fill(slot, coldWeight);
            // Jul 1 = slot 183 -> offset 0. Dec 31 -> 183; Jan 1 -> 184.
            // December and January are now adjacent, away from the axis edge.
            int shifted = (slot - 183 + 366) % 366;
            winter->Fill(shifted, coldWeight);
        }

        std::cout << year.first << ": " << coverage << '/' << expected
                  << " observed days; warmest mean " << maximum << " C on ";
        printDates(warmDates);
        std::cout << "; coldest mean " << minimum << " C on ";
        printDates(coldDates);
        std::cout << '\n';
    }

    int years = static_cast<int>(data.size());
    printModes(warm, "Warmest", years);
    printModes(cold, "Coldest", years);

    warm->SetMaximum(1.15 * std::max(warm->GetMaximum(), cold->GetMaximum()));

    TCanvas* canvas = new TCanvas("extremes_canvas", "3.3: Annual extreme dates", 1000, 850);
    canvas->Divide(1, 2);
    canvas->cd(1);
    
    GraphStyle2(warm);

    warm->SetLineColor(kRed + 1);
    cold->SetLineColor(kBlue + 1);
    winter->SetLineColor(kBlue + 1);

    warm->Draw("HIST");
    cold->Draw("HIST SAME");

    TLegend* legend = new TLegend(0.70, 0.72, 0.90, 0.89);
    legend->AddEntry(warm, "Warmest daily mean", "l");
    legend->AddEntry(cold, "Coldest daily mean", "l");
    legend->Draw();

    canvas->cd(2);
    
    GraphStyle2(winter);

    winter->Draw("HIST");
}