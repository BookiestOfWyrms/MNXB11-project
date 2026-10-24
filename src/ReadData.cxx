#include "Analysis.h"

#include <TBranch.h>
#include <TFile.h>
#include <TLeaf.h>
#include <TTree.h>

#include <cmath>
#include <iostream>
#include <stdexcept>

namespace {
TLeaf* numericLeaf(TTree* tree, const char* name) {
    TLeaf* leaf = tree->GetLeaf(name);
    if (leaf == nullptr) {
        throw std::runtime_error(std::string("Missing numeric leaf: ") + name);
    }
    if (leaf->GetLeafCount() != nullptr || leaf->GetLenStatic() != 1) {
        throw std::runtime_error(std::string("Expected scalar leaf: ") + name);
    }

    // GetValue() reads a numeric leaf as a double, so Float_t and Double_t 
    //since I don't know which.
    // temperatures both work without guessing SetBranchAddress() types, so better.
    return leaf;
}

int integerValue(TLeaf* leaf, int low, int high) {
    double value = leaf->GetValue();
    if (!std::isfinite(value) || value < low || value > high ||
        std::floor(value) != value) {
        throw std::runtime_error(std::string("Invalid date component: ") +
                                 leaf->GetName());
    }
    return static_cast<int>(value);
}
}

TemperatureData readTemperatures(const char* filename) {
    TFile input(filename, "READ");
    if (input.IsZombie()) {
        throw std::runtime_error(std::string("Cannot open ROOT file: ") + filename);
    }

    TTree* tree = nullptr;
    input.GetObject("MNXB11", tree);
    if (tree == nullptr) {
        throw std::runtime_error("No TTree named MNXB11 in the input file.");
    }

    TLeaf* yearLeaf = numericLeaf(tree, "year");
    TLeaf* monthLeaf = numericLeaf(tree, "month");
    TLeaf* dayLeaf = numericLeaf(tree, "day");
    TLeaf* tempLeaf = numericLeaf(tree, "temp");

    // These analyses need calendar dates and temperatures only.
    // And additional editing when data is taken into consideration.
    tree->SetBranchStatus("*", 0);
    tree->SetBranchStatus(yearLeaf->GetBranch()->GetName(), 1);
    tree->SetBranchStatus(monthLeaf->GetBranch()->GetName(), 1);
    tree->SetBranchStatus(dayLeaf->GetBranch()->GetName(), 1);
    tree->SetBranchStatus(tempLeaf->GetBranch()->GetName(), 1);

    TemperatureData data;
    Long64_t entries = tree->GetEntries();
    for (Long64_t entry = 0; entry < entries; ++entry) {
        try {
            if (tree->GetEntry(entry) <= 0) {
                throw std::runtime_error("Failed to read tree entry.");
            }
            int year = integerValue(yearLeaf, 1, 9999);
            int month = integerValue(monthLeaf, 1, 12);
            int day = integerValue(dayLeaf, 1, 31);
            double temperature = tempLeaf->GetValue();

            // Stop on structurally unusable values. Do NOT silently remove,
            // repair, quality-filter, or impose temperature cuts on records.
            if (!validDate(year, month, day) || !std::isfinite(temperature)) {
                throw std::runtime_error("Invalid date or non-finite temperature.");
            }

            // operator[] creates an empty vector for a newly encountered year.
            YearTemperatures& days = data[year];
            if (days.empty()) {
                days.resize(367);
            }
            days[calendarDay(month, day)].add(temperature);
        } catch (const std::exception& error) {
            throw std::runtime_error("Tree entry " + std::to_string(entry) +
                                     ": " + error.what());
        }
    }
    if (data.empty()) {
        throw std::runtime_error("The tree contains no measurements.");
    }

    std::cout << "Read " << entries << " readings in " << data.size()
              << " observed years. All supplied readings were included.\n"
              << "Date grouping uses the stored year/month/day, with no timezone conversion.\n"
              << "Assumptions: temperatures are in degrees C; one intended station;\n"
              << "quality suitability has been established outside this analysis.\n";
    return data;
    // input is a local object: its destructor closes the file automatically.
    // The returned map contains numbers, not pointers into the closed file.
}
