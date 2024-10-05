#!/bin/bash

# Check if filenames are provided

if [ $# -lt 3 ]; then
    
    echo "ERROR: At least three filenames are required..."
    echo "Format: $0 <file 1> <file 2> <file 3> ..."
    exit 1
fi

# Excludes filename of script when processing

for file in "$@"; do
    if [ -d "$file" ]; then 
        echo "Directories are prohibited..."
        exit 1
    fi
    # File does not actually exist
    if [ ! -f "$file" ]; then
        echo "The file '$file' doesn't exist. Skipping..."
    fi
done

# Creates dir for backups
backupDir="backup_$(date +%Y%m%d)"

# Verifies if backup dir exists or not before attempting to create
if [ -d "$backupDir" ]; then
    echo "Directory $backupDir already exists"
else 
    mkdir "$backupDir"
fi

# Log file for backups
logFile="$backupDir/backup_log.txt"
# Starting line in file
echo "Original File | Backup File" > "$logFile"

# Loops through all files passed
for file in "$@"; do
    # If it exists
    if [ -f "$file" ]; then
        # Copy into backup Dir
        cp "$file" "$backupDir"
        timestamp=$(date +%Y%m%d)
        newName="${file}_${timestamp}"
        mv "$backupDir/$file" "$backupDir/$newName"
        echo "$file | $newName" >> "$logFile"
    fi        
done

echo "Backup Completed. Backups can be found in $PWD/$backupDir"

