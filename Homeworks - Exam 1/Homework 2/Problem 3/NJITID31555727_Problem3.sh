
#!/bin/bash

if [ $# -eq 0 ]; then
    echo "ERROR: No Argument Passed"
    echo "Format: $0 <fileName.txt>"
    exit 1
fi


# Define the input file
input_file="$1"

if [ -d "$input_file" ]; then
	echo "ERROR: Input is a directory, not a file"
	exit 1


elif [ ! -f "$input_file" ]; then
	echo "The file '$input_file' doesn't exist."
	exit 1
fi


# Source: https://stackoverflow.com/questions/965053/extract-filename-and-extension-in-bash
extension="${input_file##*.}"

if [ "$extension" != "txt" ]; then
	echo "ERROR: 'txt' extension required"
	exit 1
fi

# Checks if file is empty
if [ ! -s "$input_file" ]; then
	echo "ERROR: '$input_file' is empty"
	exit 1
fi

# Define the current year
current_year="2024"

# Regular expression to match lines with cities containing two or more words (also ensures all criteria is met)

city_pattern="^[A-Z][a-z]*\ [A-Z][a-z]*,\ (19[0-9][0-9]|20[0-1][0-9]|202[0-4])-(0[1-9]|1[0-2])-(0[1-9]|[1-2][0-9]|3[0-1]),\ [A-Z][a-zA-Z ]* [A-Z][a-zA-Z]*,\ [A-Z][a-zA-Z]*$"


# Function to calculate age based on date of birth
function calculate_age() {
    local dob="$1"
    local year=$(date -d "$dob" +%Y)
    echo $((current_year - year))
}

# Create a temporary file to store the sorted data
temp_file=$(mktemp)

# Read the input file line by line and process it
while read line; do
    # Check if the line matches the city pattern
    if [[ $line =~ $city_pattern ]]; then
        # Extract name, date of birth
        # Source: https://phoenixnap.com/kb/awk-command-in-linux
        name=$(echo "$line" | awk -F ', ' '{print $1}')
        dob=$(echo "$line" | awk -F ', ' '{print $2}')
        
        # Calculate age based on date of birth
        age=$(calculate_age "$dob")
        
        # Print name and age to the temporary file
        echo "$name,$age" >> "$temp_file"
    fi
done < "$input_file"


# Sort the temporary file based on age (the second field) in descending order
sort -t',' -k2,2nr "$temp_file" | awk -F ',' '{print $1 " is " $2}'

# Remove the temporary file
rm "$temp_file"



