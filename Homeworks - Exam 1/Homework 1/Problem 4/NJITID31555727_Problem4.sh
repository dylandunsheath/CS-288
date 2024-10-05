#!/bin/bash
# Display file 10 lines at a time
function display_file() {
    local file="$1"
    local totalLines=$(wc -l < "$file")
    local currentLine=0
    local linesShow=10
    
    # If empty file
    if [ "$totalLines" -eq 0 ]; then
        baseDisplay=$(basename "$file")
        echo "$baseDisplay is empty"
        cd "$originalDir"
        select_choice
    fi 
    # Display first ten lines
    head "$file"
    # Adds 10 lines 
    currentLine=`expr $currentLine + 10`
    while true; do
        # When finished reading file
        if [ "$currentLine" -ge "$totalLines" ]; then
            echo "All lines were displayed." 
            cd "$originalDir"
            select_choice
        fi
        read -p "Do you want to display the next ten lines in $file (y/n)? " response

        case $response in
        "y")
            tail -n +$((currentLine+1)) "$file" | head -n "$linesShow"
            currentLine=$((currentLine+10))
            continue
             
            ;;
        # They don't
        "n")
            cd "$originalDir"
            select_choice
            ;;
         # Invalid
         *) 
            echo "$response is invalid. Please re-enter: " 
            ;;
        esac
    done   
}

# Recursive DFS
function search_modded_files() {
    # Passed argument
    local directory="$1"
    local current_time=$(date +%s)
    local twenty_four_hours_seconds=86400
    
    for entryTwo in "$directory"/*; do
        if [ -d "$entryTwo" ]; then
            search_modded_files "$entryTwo"
        elif [ -f "$entryTwo" ]; then
            local modified_time=$(stat -c %Y "$entryTwo")
            local time_difference=$((current_time - modified_time))

            # file IS last modified in the last 24 hours 
            if [ $time_difference -le $twenty_four_hours_seconds ]; then
                # Adds path to file
                modded_files+=("$entryTwo")
            fi
        fi
    done

}
# User selection
function select_choice() {
    # Continously Loops
    while true; do
        echo "Current Directory: $(pwd)"
        choices=()
        for entry in *; do
            # Gets content
            if [ -f "$entry" ] || [ -d "$entry" ]; then
                choices+=("$entry")
            fi      
        done 
        choices+=("Quit")
       
        PS3="Select a File or Subdirectory: "
        select entry in "${choices[@]}"; do
            # Checks if user entered a valid number

            # Source: https://berkeley-scf.github.io/tutorial-using-bash/regex.html
            if [[ "$REPLY" =~ ^[0-9]+$ ]]; then
                # Checks if valid input
                if ((REPLY >= 1 && REPLY <= ${#choices[@]})); then

                    if [ "$entry" = "Quit" ]; then 
                        echo "Exiting..."
                        exit 0

                    elif [ -d "$entry" ]; then
                        echo "You selected subdirectory: $entry"

                        cd "$entry"

                        currentDirectory="$(pwd)"
                        modded_files=()
                        # Calls function
                        search_modded_files "$currentDirectory"

                        if [ ${#modded_files[@]} -eq 0 ]; then 
                            echo "No files modified in last 24 hours"
                            cd "$originalDir"
                            select_choice
    
                        else     
                            echo "Files Modified in Last 24 Hours:"
                            select file in "${modded_files[@]}"; do 
                                if [ -n "$file" ]; then 
                                    echo "You selected file: $file"
                                    
                                    display_file "$file"
                                      
                                else
                                    echo "Choice is not valid. Going back to original directory..."
                                    cd "$originalDir"
                                    select_choice
                                fi
                            done 
                    
                        fi  
                    else
                        echo "You selected file: $entry"
                        display_file "$entry"
                    fi
                else
                    echo "Choice is not valid. Please re-enter."
                fi
            else 
                echo "Choice is not valid. Please re-enter."
            fi
            break
        done
    done   
}

# Gets choice of directory
function get_directory_choice() {
    # Gets user input
    directories=()
    for entry in *; do
        # Gets content
        if [ -d "$entry" ]; then
            directories+=("$entry")
        fi      
    done
    echo "Directories List:"
    for directory in "${directories[@]}"; do 
        echo "$directory"
    done
    read -p "Enter a directory name: " dirName
    if [ -z "$dirName" ]; then
            echo "Directory Name can't be empty. Going back..."
            get_directory_choice
        fi
    # Directory doesn't exist
    if [ ! -d "$dirName" ]; then
        # Asks if they wanna create it
        read -p "Directory entered does not exist. Would you like to create it? (y/n): " choice
        
        while true; do 
            # they do
            case $choice in
            "y")

                echo "You said: $choice"
                mkdir "$dirName"
                echo "The directory '$dirName' was created"
                get_directory_choice
                ;;
            # They don't
            "n")
                echo "You said: $choice"
                get_directory_choice
                ;;
            # Invalid
            *) 
                read -p "'$choice' is invalid. Please re-enter (y/n): " choice
                ;;
            esac
        done   

    else   
        cd "$dirName"

        # No subdirectories or files to use
        if [ ! -n "$(find . -mindepth 1 -type d)" ] && [ ! -n "$(find . -mindepth 1 -type f)" ]; then

            echo "Empty Directory"
            cd .. 
            get_directory_choice
        fi 
       

        originalDir="$(pwd)"
    
    fi

    select_choice
}

# Calls initial function
get_directory_choice






