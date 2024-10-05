#include <stdio.h>
#include <string.h>
#include <dirent.h>

#define MAX_PATH_LENGTH 1001

int count_lines_in_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        return 1;
    }

    int count = 0;
    char ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            count++;
        }
    }

    fclose(file);
    return count;
}

int traverse_directory(const char *path) {
    DIR *dir;
    struct dirent *entry;
    dir = opendir(path);

    if (dir == NULL) {
        printf("Error opening directory: %s\n", path);
        return 1;
    }

    int total_lines = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                char sub_path[MAX_PATH_LENGTH];
                snprintf(sub_path, sizeof(sub_path), "%s/%s", path, entry->d_name);
                total_lines += traverse_directory(sub_path);
            }
        } else {
            if (strstr(entry->d_name, ".txt") != NULL) {
                char file_path[MAX_PATH_LENGTH];
                snprintf(file_path, sizeof(file_path), "%s/%s", path, entry->d_name);
                total_lines += count_lines_in_file(file_path);
            }
        }
    }

    closedir(dir);
    return total_lines;
}

int main() {
    
    char directory[MAX_PATH_LENGTH];
    printf("Enter the directory path: ");
  
    scanf("%[^\n]", directory);
    if (strlen(directory) == 0) {
        printf("ERROR: Directory name can't be empty!\n");
        return 1;
    }
    if (strlen(directory) > 1000) {
        printf("String length can't exceed 1000!\n");
	return 1;
    }
    DIR *dir = opendir(directory);

    if (dir == NULL) {
        printf("ERROR: Directory can't be opened or doesn't exist!\n");
        return 1;
    }
    closedir(dir);
    int total_lines = traverse_directory(directory);
    printf("Total lines across all text files: %d\n", total_lines);

    return 0;
}


