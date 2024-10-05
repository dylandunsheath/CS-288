#!/bin/bash


# If the length of the argument is 0, prompt for input
if [ -z "$1" ]; then
    read -p "Enter a file or directory: " userInput

    while [ -z "$userInput" ]; do 
        read -p "ERROR: Invalid Input. Please re-enter file or directory: " userInput
    done
# Otherwise, use the argument
else
    userInput="$1"
fi

# Is it a directory?
if [ -d "$userInput" ]; then 
    echo "You gave directory $userInput"
    file_count=0
    subdir_count=0

    
    if [ -r "$userInput" ] && [ -x "$userInput" ]; then
        # Gets files and subdirectories 
        file_count=$(find "$userInput" -type f | wc -l)
        subdir_count=$(find "$userInput" -type d | wc -l)
        # Excludes the CURRENT directory we are in
        # as it would have subdirectories + 1
        subdir_count=`expr $subdir_count - 1`
        # Display the number of files and subdirectories

        echo "Number of files in directory: $file_count"
        echo "Number of Subdirectories: $subdir_count"
      
        if [ "$file_count" -gt 0 ]; then
            echo "List of files with size:"
            find "$userInput" -type f -exec du -h {} + 2>/dev/null
        else
            echo "There are no files in the specified directory"
        fi
        if [ "$subdir_count" -gt 0 ]; then
            echo "List of subdirectories with size"

            find "$userInput" -type d -exec du -h --exclude="$userInput" {} + 2>/dev/null
        else
            echo "There are no subdirectories in the specified directory"
        fi
    else
        echo "You don't have the necessary read and execute permissions."
        exit 1
    fi
#Is file?
elif [ -f "$userInput" ]; then 
    echo "You gave a file"
    
    if [ -r "$userInput" ]; then
        wordCount=$(wc -w < "$userInput")
        lineCount=$(wc -l < "$userInput")
        charCount=$(wc -c < "$userInput")
            
        if [ "$charCount" -gt 10000 ]; then
            categoryType="Text Heavy"
        elif [ "$charCount" -ge 1000 ] && [ "$charCount" -le 10000 ]; then
            categoryType="Moderately Sized"
        else
            categoryType="Light File"
        fi
        echo "Word Count: $wordCount"
        echo "Line Count: $lineCount"
        echo "Character Count: $charCount"
        echo "Category Type: $categoryType"
    else
        echo "No read permissions" 
    fi

else 
    echo "$userInput doesn't exist"
    exit 1
fi
