
#include <iostream>
#include <vector>
#include "Measurement.h"
#include "DataExtraction.h"
#include "Analysis.h"

#include <TApplication.h>
#include <TROOT.h>

#include <exception>
#include <iomanip>
#include <string>

int main(int argc, char* argv[]){
  //const std::string csv_filename = "/home/macsch/MNXB11/git/MNXB11-project/cleaner/baredata_smhi-opendata_1_53430_20231007_155558_Lund.csv";
  //const std::string output_filename = "measurements.root";


  std::vector<Measurement> measurements = read_measurements("./cleaner/baredata_smhi-opendata_1_53430_20231007_155558_Lund.csv");

  std::cout << "Number of measurements read" << measurements.size() << '\n';

  persist_measurements(measurements, "measurements.root");

  
  // CHANGE YOUR ANALYSIS SETTINGS HERE, then run make again.
    const int selectedMonth = 8;
    const int selectedDay = 23;
    const double probabilityLow = 15.0;  // Inclusive interval boundary, C.
    const double probabilityHigh = 20.0; // Exclusive interval boundary, C.
    const int earlierYear = 1900;
    const int laterYear = 2000;

    const char* filename = "measurements.root";
    bool batch = false;
    bool filenameGiven = false;
    for (int argument = 1; argument < argc; ++argument) {
        if (std::string(argv[argument]) == "--batch") {
            batch = true;
        } else if (!filenameGiven) {
            filename = argv[argument];
            filenameGiven = true;
        } else {
            std::cerr << "Usage: " << argv[0] << " [measurements.root] [--batch]\n";
            return 1;
        }
    }

    // ROOT receives its own small argument list, so it cannot interpret the
    // measurement filename or our --batch option as ROOT startup commands.
    if (batch) {
        gROOT->SetBatch(true);
    }
    int rootArgc = 1;
    char* rootArgv[] = {argv[0], nullptr};
    TApplication application("temperature_analysis", &rootArgc, rootArgv);

    try {
        std::cout << std::fixed << std::setprecision(4);
        TemperatureData data = readTemperatures(filename);
        analyzeGivenDay(data, selectedMonth, selectedDay,
                        probabilityLow, probabilityHigh);
        analyzeEveryDay(data);
        analyzeYearExtremes(data);
        // compareYears(data, earlierYear, laterYear);
        if (!batch) {
            std::cout << "\nPlots are open. Press Ctrl+C in the terminal to finish.\n";
            application.Run();  // ROOT's event loop keeps the windows responsive.
        }
    } catch (const std::exception& error) {
        std::cerr << "Analysis stopped: " << error.what() << '\n';
        return 1;
    }
    return 0;
}

