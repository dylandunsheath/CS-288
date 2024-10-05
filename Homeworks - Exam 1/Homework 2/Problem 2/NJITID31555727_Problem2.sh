#!/bin/bash

# Function to extract and append unique email addresses from a file
extract_emails() {
    local file="$1"
    # Only matches with extended regex 
    # the matching emails to this regex
    # Sorts and removes duplicates
    grep -oE "\b[A-Za-z0-9._-]+@[A-Za-z]+\.[A-Za-z]{2,}\b" "$file" | sort -u
}

# Function to recursively traverse directories and process text files
traverse_directory() {
    local directory="$1"
    for entry in "$directory"/*; do
        if [ -d "$entry" ]; then
            traverse_directory "$entry"
        elif [ -f "$entry" ]; then
            if [[ "$entry" == *.txt ]]; then
                extract_emails "$entry"
                
            fi
        fi
    done
}

if [ $# -eq 0 ]; then
    echo "ERROR: No Argument Passed"
    echo "Format: $0 <directory>"
    exit 1
fi
    
directory="$1"

# Check if the directory exists
if [ ! -d "$directory" ]; then
    echo "Error: Directory '$directory' not found."
    exit 1
fi

#


# Traverse the directory and extract email addresses
all_emails=$(traverse_directory "$directory")

if [ -z "$all_emails" ]; then
    echo "ERROR: No emails found."
    exit 1
fi 

# Sort and remove duplicates
sorted_unique_emails=$(echo "$all_emails" | sort -u)



# Write the unique email addresses to a file
echo "$sorted_unique_emails" > unique_emails.txt

echo "Unique email addresses extracted and saved to unique_emails.txt"




