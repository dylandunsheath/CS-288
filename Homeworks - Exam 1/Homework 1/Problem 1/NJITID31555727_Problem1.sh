#!/bin/bash

read -p "Enter a filename: " fileName 

# Checks if fileName has a length of zero
# Also accounts for spaces for filename
if [ -z "$fileName" ]; then
    echo "ERROR: No filename was provided..."
    exit 1
# Checks if directory
elif [ -d "$fileName" ]; then
    echo "ERROR: $fileName is a directory, not a file"
    exit 1
# Checks if the file does NOT exist in current directory
elif [ ! -f "$fileName" ]; then
    echo "ERROR: $fileName not found in the current directory"
    exit 1
# File exists
else
    lineCount=$(wc -l < "$fileName")

    if [ "$lineCount" -eq 0 ]; then
        echo "The file '$fileName' is empty."
        exit 1
    else
        # Displays the last 10 lines
        tail -n 10 "$fileName"
    fi
fi


