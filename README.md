# MNXB11-project


# SMHI cleaning 

To get rid of any text and ; in the SMHI data files, run the smhicleaner.sh script.
This will create a data file for each part of the script. The final version of the cleaned data will be named baredata_smhi-opendata... with the name of the city in the name. 

How to run: 
./smhicleaner.sh path/to/data

# Data extraction 

The scripts used for data extraction is Measurement.cxx and DataExtraction.cxx with their respective header files.
To run each of these they need to be first compiled with extract_main.cxx. Make sure to check that in extract_main.cxx has the correct path to your data and to change the name of the city to the respective cleaned data file. 
The ROOT file already produced is of weather data in and have branches for year, month, day, hour, minute, second, temp, and quality. 

# Code Running
Step 1: Please clone the repository on your local machine:
* Open terminal;
* Head into your specified `.git` directory;
* Navigate into said directory: `cd ~/PATH/TO/git`;
* Run `git clone git@github.com:BookiestOfWyrms/MNXB11-project.git/`.


Step 2: Make sure that you are on the main branch:
* In the same terminal window, in the project directory (accessible by `cd MNXB11-project`, assuming you followed the previous steps accordingly);
* Run `git status`. If the command output contains `On branch main`, skip to the next step. Otherwise, continue with the list of instructions;
* Run `git checkout main`.


Step 3: Enter the course container and run `make`:
* In the same terminal as above, run `startmnxb11container.sh bash`;
* If successful, run `g++ extract_main.cxx src/Measurement.cxx src/DataExtraction.cxx -o extract_main -Iinclude -Wall -Wextra -Werror $(root-config --cflags --glibs)`;
* Now, run `make` to finalise compiling the remainder of the project.
* Then run `./extract_main`.

Step 4: Run `./temperature_analysis measurements_lund.root` in the same terminal.


Step 5: After reviewing the plotting and overall console output, press `CTRL+C` to kill the ROOT instance still hanging.
