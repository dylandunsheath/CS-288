#!/bin/bash

# Verifies that an argument was actually passed
if [ $# -eq 0 ]; then
	echo "ERROR: No Arguments Passed"
	echo "Format: $0 <file>"
	exit 1
fi

# Passes / initializes argument to a variable 
fileName="$1"

if [ -d "$fileName" ]; then
	echo "ERROR: Input is a directory, not a file"
	exit 1


elif [ ! -f "$fileName" ]; then
	echo "The file '$fileName' doesn't exist."
	exit 1
fi

# Source: https://www.cyberciti.biz/faq/extract-bash-get-filename-extension/
# Gets extension (verificatino it's a c file)
extension="${fileName##*.}"
#echo "$extension"

if [ "$extension" = "c" ]; then
	# Declare and initialize counters for scanf and printf
	scanfCount=0
	printfCount=0
	totalLines=0
	
	# Gets total lines for scanf,printf and total file lines
	scanfCount=$(grep -c "scanf" "$fileName")
	printfCount=$(grep -c "printf" "$fileName")
	totalLines=$(wc -l < "$fileName")
	
	# Calculate percentage
	# Source for scale: https://unix.stackexchange.com/questions/599485/set-scale-for-bc-inside-a-variable
	scanfPer=$(echo "scale=2; 100 * $scanfCount / $totalLines" | bc)
	printfPer=$(echo "scale=2; 100 * $printfCount / $totalLines" | bc)
	
	# Extracts 'scanf' and 'printf' and APPENDS to end of file
	grep "scanf" "$fileName" >> scanf_log.txt
	grep "printf" "$fileName" >> printf_log.txt
	
	echo 
	echo "The total lines in $fileName: $totalLines"
	echo 
	echo "The lines that contain 'scanf': $scanfCount ($scanfPer%)"
	echo
	echo "The lines that contain 'printf': $printfCount ($printfPer%)"



	
else
	echo "ERROR: extension '$extension' is not supported"
	exit 1
fi

