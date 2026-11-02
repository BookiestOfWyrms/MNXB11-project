
#include <iostream>
#include <vector>
#include "Measurement.h"
#include "DataExtraction.h"

int main() {

  std::vector<Measurement> measurements = read_measurements("./cleaner/baredata_smhi-opendata_1_53430_20231007_155558_Lund.csv");

  std::cout << "Number of measurements read" << measurements.size() << '\n';

  persist_measurements(measurements, "measurements_lund.root");
  return 0;
}