
#include <iostream>
#include <vector>
#include "Measurement.h"
#include "DataExtraction.h"

int main() {
  //const std::string csv_filename = "/home/macsch/MNXB11/git/MNXB11-project/cleaner/baredata_smhi-opendata_1_53430_20231007_155558_Lund.csv";
  //const std::string output_filename = "measurements.root";


  std::vector<Measurement> measurements = read_measurements("/home/macsch/MNXB11/git/MNXB11-project/cleaner/baredata_smhi-opendata_1_53430_20231007_155558_Lund.csv");

  std::cout << "Number of measurements read" << measurements.size() << '\n';

  persist_measurements(measurements, "measurements.root");
  return 0;
}
