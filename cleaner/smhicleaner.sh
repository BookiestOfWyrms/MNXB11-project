#!/bin/bash

########################################################################
# 
# shmicleaner.sh - MNXB11-2023 Tutorial
#
# Author: Florido Paganelli florido.paganelli@hep.lu.se
#
# Description: this script manuipulates specific SMHI dataset and 
#              performs some cleaning actions on it, namely:
#              - Takes an SMHI datafile <filename> from a specified path in the filesystem
#              - Cleanses up unwanted information in the data file and
#                 extracts just the bare temperature data in
#                 - baredata_<filename>
#              - prepares the file to be read by standard C++ 
#                CSV libraries
#
# Examples:
#        ./smhicleaner.sh ../data/smhi-opendata_1_52240_20200905_163726.csv
#
# NOTE: the paths above are examples.
# So you should NOT assume the file is exactly in any of the above paths.
# The code must be able to process any possible file path.
#
########################################################################


###### Functions #######################################################

## usage
# this function takes no parameters and prints an error with the 
# information on how to run this script.
usage(){
	echo "----"
	echo -e "  To call this script please use"
	echo -e "   $0 '<path-to-datafile>'"
	echo -e "  Example:"
    echo -e "   $0 '/home/macsch/MNXB11/git/MNXB11-project/datasets/smhi-opendata_1_53430_20231007_155558_Lund.csv'"
	echo "----"
}

###### Functions END =##################################################


# T1 Get the first parameter from the command line:
# (see Tutorial 4 slides 43,44)
# and put it in the variable SMHIINPUT
SMHIINPUT=$1

# T2 Input parameter validation:
# Check that the variable SMHIINPUT is defined, if not, 
# inform the user, show the script usage by calling the usage() 
# function in the library above and exit with error
# See Tutorial 4 Slide 45-47 and exercises 4.14, 4.15
if [[ "x$SMHIINPUT" == 'x' ]]; then
   echo "Missing input file parameter, exiting"
   usage
   exit 1
fi

# T3 Extract filename:
# Extract the name of the file using the "basename" command 
# basename examples: https://www.geeksforgeeks.org/basename-command-in-linux-with-examples/
# then store it in a variable DATAFILE
DATAFILE=$(basename $SMHIINPUT)

# T4 Analyze the input parameter and copy:

# T4.1 If $SMHIINPUT not empty
if [[ "x$SMHIINPUT" != "x" ]]; then
   # T4.2 check if the file is a directory, it should not be!
   # exit with error in case.
   if [[ -d $SMHIINPUT ]]; then
      echo -e "This script requires a data file and not a directory, exiting..."     
      exit 1
   fi
   # T4.3 Copy the file in the current directory as
   #    original_$DATAFILE
   # use the -a option to preserve filesystem information (permissions etc)
   echo "Copying input file $SMHIINPUT to original_$DATAFILE"
   cp -a $SMHIINPUT ./original_$DATAFILE
fi 

# T5 Check that the input file has been copied with no errors:
# Write an IF statement that does the following:
# If any of the previous commands failed, exit with error 
# and tell the user what happened.
# remind the user of the syntax by calling the usage() function defined
# in the libraries section above.
# test the $? variable, Tutorial 4 slides 45-47 
# and previous years exercises (Tutorial 4 slide 17)
if [[ $? != 0 ]]; then
   echo "Error downloading or copying file, maybe wrong command syntax? exiting...."
   usage
   exit 1
fi

# If we got here without errors, we can start cleaning up!

# T6 Identify the data starting line:
# Looking at the SHMI data, it seems that the line that contains the
# string "Datum" is the beginning of data.
# Find what line contains the string "Datum" using grep
# put the value in a variable called STARTLINE
# - use the grep option -n  and cut to take just the number.
# - use the cut command to select field 1 using ':' as separator
# - Use the pipe | to pass the output of grep to cut
# Examples about the above can be found in Tutorial 4 slide 13, 57, 58 and 
# some in Tutorial 2
# read also:
# https://stackoverflow.com/questions/6958841/use-grep-to-report-back-only-line-numbers
echo "Finding the first line containing 'Datum'..."
STARTLINE=$(grep -n 'Datum' original_${DATAFILE} | cut -d':' -f 1)

# T7 skip one more header line:
# Use arithmetic expansion to add a line, since the actual 
# data starts at the STARTLINE + 1 line, so to remove the header
# where Datum;... is contained
# See Tutorial 4 Slide 96
STARTLINE=$(( $STARTLINE + 1 ))

# T8 Remove unnecessary lines at the top of the datafile:
# Strip away the top STARTLINE lines (the value of $STARTLINE) using 
# the tail command and write the result 
# to as file called clean1_$DATAFILE using the > operator to 
# redirect the standard output.
# how to exclude n lines with tail : http://heyrod.com/snippets/skip-n-lines.html
# For the use of the > operator, see Tutorial 2
# Redirecting output: https://thoughtbot.com/blog/input-output-redirection-in-the-shell#redirecting-output
# You can verify the generated file against the same file in casestudy/result
echo "Removing the first $STARTLINE lines, result in clean1_${DATAFILE}"
tail -n +$STARTLINE original_${DATAFILE} > clean1_${DATAFILE}

# T9 Fix format for the strange lines with comments:
# consider only the relevant columns (1,2,3,4,5) using cut
# this will clean up the lines at the beginning of the data 
# that are not consistent with the format, so that we can use the data 
# these lines contain without discarding them.
# Write the result to a file called clean2_${DATAFILE} using the > operator
# see cut examples at the link in Tutorial 4 slide 13, in particular
#       "5. Select Multiple Fields from a File"
# You can verify the generated file against the same file in casestudy/result
echo "Selecting only relevant columns, result in clean2_${DATAFILE}"
cut -d';' -f 1,2,3,4,5 clean1_${DATAFILE} > clean2_${DATAFILE}

# T10 Convert format to C++ readable with spaces instead of commas:
# Change semicolons to singe spaces using sed and 
# write out the result to a file called baredata_${DATAFILE}
# Read about sed at the links in Tutorial 4 slide 13,
# and here: https://linuxize.com/post/how-to-use-sed-to-find-and-replace-string-in-files/
# You can verify the generated file against the same file in casestudy/result
echo "Substituting the ; with spaces, result in baredata_${DATAFILE}"
sed 's/;/ /g' clean2_${DATAFILE} > baredata_${DATAFILE}

# initialize empty report file
# here I use the special device /dev/null so that if there is already
# an existing file it will be cleaned
cat /dev/null > report_${DATAFILE}

# T11 Print sizes of generated .csv files in a report file:
# Using a for, scan all the .csv files in the folder and write out a
# report of their size in report_<filename>
# cycle the files with a variable csvfile
# Save the size of each file in variable SIZE using the "stat" command
# see: https://www.howtogeek.com/451022/how-to-use-the-stat-command-on-linux/
# to verify the results: compare your output with the script_output file
# in the tutorial3/casestudy/result folder
echo "Writing filesizes summary to report_${DATAFILE}"
for csvfile in *.csv; do
    SIZE=`stat -c %s $csvfile`
    echo "   file $csvfile has size: $SIZE" >> report_${DATAFILE}
done