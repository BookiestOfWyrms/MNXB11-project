#include "Analysis.h"

#include <TFile.h>
#include <TTree.h>

#include <iostream>

bool readTemperatures(const char* filename, TemperatureData& data) {
    TFile input(filename, "READ");
    if (input.IsZombie()) {
        std::cerr << "Cannot open ROOT file: " << filename << '\n';
        return false;
    }

    TTree* tree = nullptr;
    input.GetObject("MNXB11", tree);
    if (tree == nullptr) {
        std::cerr << "No TTree named MNXB11 in " << filename << '\n';
        return false;
    }

    // Confirmed in measurements_lund.root: dates are Int_t; temp is Double_t.
    // SetBranchAddress tells ROOT where to put each entry's values in memory.
    Int_t year = 0;
    Int_t month = 0;
    Int_t day = 0;
    Double_t temperature = 0.0;

    // Read only the four branches needed for daily-temperature analyses.
    // This changes reading behaviour, not the contents of the input file.
    tree->SetBranchStatus("*", 0);
    const char* branches[] = {"year", "month", "day", "temp"};
    for (const char* branch : branches) {
        if (tree->GetBranch(branch) == nullptr) {
            std::cerr << "Missing branch: " << branch << '\n';
            return false;
        }
        tree->SetBranchStatus(branch, 1);
    }
    if (tree->SetBranchAddress("year", &year) < 0 ||
        tree->SetBranchAddress("month", &month) < 0 ||
        tree->SetBranchAddress("day", &day) < 0 ||
        tree->SetBranchAddress("temp", &temperature) < 0) {
        std::cerr << "Branch types do not match the Lund file's expected types.\n";
        return false;
    }

    // The supplied Lund file was checked: all dates are valid, all temperatures
    // are finite. No generic type conversion or per-entry cleaning is needed.
    // These assumptions apply to this dataset, not arbitrary replacement files.
    data.clear();
    Long64_t entries = tree->GetEntries();
    for (Long64_t entry = 0; entry < entries; ++entry) {
        if (tree->GetEntry(entry) <= 0) {
            std::cerr << "Could not read tree entry " << entry << '\n';
            return false;
        }
        YearTemperatures& days = data[year];
        if (days.empty()) {
            days.resize(367);
        }
        days[calendarDay(month, day)].add(temperature);
    }
    if (data.empty()) {
        std::cerr << "The tree contains no measurements.\n";
        return false;
    }

    return true;
    // Closing input is automatic. The returned data holds numbers, not file pointers.
}
