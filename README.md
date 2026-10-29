# MNXB11-project


# SMHI cleaning 

To get rid of any text and ; in the SMHI data files, run the smhicleaner.sh script. 
This will create a data file for each part of the script. The final version of the cleaned data will be named baredata_smhi-opendata... with the name of the city in the name. 

# Data extraction 

The scripts used for data extraction is Measurement.cxx and DataExtraction.cxx with their respective header files.
To run each of these they need to be first compiled and then main.cxx needs to be compiled and ran. 
The ROOT file already produced is of weather data in and have branches for year, month, day, hour, minute, second, temp, and quality. 

# Code Running
Step 1: Please clone the repository on your local machine.
Step 2: Make sure that you are on the main branch.
Step 3: Enter the course container and run `make`.
Step 4: Run `./temperature_analysis measurements_lund.root`.
Step 5: After reviewing the plotting and overall console output, press `CTRL+C` to kill the ROOT instance still hanging.
