#include <stdio.h>

int main(int argc, char **argv) {
    FILE *file;
    //char filename[] = "file4values"; // Replace with the actual file name
    if (argc == 1) {
        printf("No arguments passed!\n");
        return 1;
    }
    // Open the binary file in binary read mode
    file = fopen(argv[1], "rb");


    // Check if the file was opened successfully
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", argv[1]);
        return 1; // Return an error code
    }

    // Read numbers from the file and print them
    int number;
    while (fread(&number, sizeof(int), 1, file) == 1) {
        printf("%d\n", number);
    }

    // Close the file
    fclose(file);

    return 0; // Return success
}
