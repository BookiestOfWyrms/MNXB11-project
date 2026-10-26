#include "Analysis.h"

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