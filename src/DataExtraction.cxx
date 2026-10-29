#include "DataExtraction.h"

// Measurment is a class defined in Measurement.h
// define the function read_measurements with the name given by the filename
// parameter
std::vector<Measurement> read_measurements(const std::string& filename) {
  // std::ifstream is used to read the .csv file and create an input file stream
  // named file where {filename} initializes the stream
  std::ifstream file{filename};
  // creates empty measurements vector where objects from Measurement will be
  // stored
  std::vector<Measurement> measurements;

  // define variables needed
  int year, month, day;
  int hour, minute, second;
  double temp;
  char quality;

  char dash;
  char colon;

  // Extract values from the file stream in a while loop
  //  example: file >> variable will extract data from file and store the data
  //  in variable In this case it expects to read: year-month-day
  //  hour:minute:second temp quality
  while (file >> year >> dash >> month >> dash >> day >> hour >> colon >>
         minute >> colon >> second >> temp >> quality) {
    // create object measurement which is used to test if the measurement is
    // valid using the .is_valid member function that returns true if it is
    // valid
    Measurement measurement{year,   month,  day,  hour,
                            minute, second, temp, quality};
    if (measurement.is_valid()) {
      //.push_back member function is to add to the vector with evey iteration
      measurements.push_back(measurement);
    } else {
      std::cerr << "Warning: Skipping invalid measurement: (" << year << "-"
                << month << "-" << day << "," << hour << ":" << minute << ":"
                << second << "," << temp << "," << quality << ")" << std::endl;
      // std::endl is a stream manipulator that inserts a new line (used in
      // place of /n) and flushes the output stream (meaning it forces data in a
      // memory buffer to move)
    }
  }
  return measurements;
}

// save the measurements to a ROOT file
// hvaing the output file pass into the function allows to reuse this script
// with different output files
void persist_measurements(const std::vector<Measurement>& measurements,
                          const std::string& output_filename) {
  // create ROOT Tfile
  // c_str() is a function that returns a pointer (similar to using: const
  // char*)
  TFile output_file{output_filename.c_str(), "RECREATE"};
  // create TTree with the name "MNXB11" and create datasets with a branch for
  // each variable
  TTree tree{"MNXB11", "MNXB11"};
  Double_t temp;
  Int_t year;
  Int_t month;
  Int_t day;
  Int_t hour;
  Int_t minute;
  Int_t second;
  Int_t quality;
  tree.Branch("temp", &temp);
  tree.Branch("year", &year);
  tree.Branch("month", &month);
  tree.Branch("day", &day);
  tree.Branch("hour", &hour);
  tree.Branch("minute", &minute);
  tree.Branch("second", &second);
  tree.Branch("quality", &quality);

  // auto& is used to make a reference whos data type is determined by the
  // compiler
  for (const auto& measurement : measurements) {
    year = measurement.get_year();
    month = measurement.get_month();
    day = measurement.get_day();
    hour = measurement.get_hour();
    minute = measurement.get_minute();
    second = measurement.get_second();
    temp = measurement.get_temp();
    quality = measurement.get_quality();

    tree.Fill();
  }
  tree.Write();
}