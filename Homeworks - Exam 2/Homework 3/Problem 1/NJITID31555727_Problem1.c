#include <stdio.h>
#include <unistd.h>

void print_matrix(int n, long matrix[n][n][n]) {
    int i, j, k;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            for (k = 0; k < n; k++) {
                printf("%ld ", matrix[i][j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Format: %s <filename #1> <filename #2>\n", argv[0]);
        return 1;
    }
     // Check if files exist
    if (access(argv[1], F_OK) == -1 || access(argv[2], F_OK) == -1) {
        printf("Error: One or both input files do not exist\n");
        return 1;
    }
    // Open matrix1.bin and matrix2.bin for reading
    FILE *matrix1_file = fopen(argv[1], "rb");
    FILE *matrix2_file = fopen(argv[2], "rb");

    if (matrix1_file == NULL || matrix2_file == NULL) {
        perror("Error opening input files\n");
        return 1;
    }

    int n1, n2, n;
    // Read the dimension of the matrices
    fread(&n1, sizeof(int), 1, matrix1_file);
    fread(&n2, sizeof(int), 1, matrix2_file);

    // Check if dimensions are compatible
    if (n1 != n2) {
        printf("Error: Matrices are not compatible for multiplication\n");
        fclose(matrix1_file);
        fclose(matrix2_file);
        return 1;
    }

    if (n1 > 100 || n2 > 100) {
        printf("Error: Dimensions exceeds 100!\n");
        fclose(matrix1_file);
        fclose(matrix2_file);
        return 1;
    }
    n = n1;

    long matrix_1[n1][n1][n1], matrix_2[n1][n1][n1], result_matrix[n1][n1][n1];

    // Read the elements of the matrices
    fread(matrix_1, sizeof(long), n * n * n, matrix1_file);
    fread(matrix_2, sizeof(long), n * n * n, matrix2_file);

    // Close the input files
    fclose(matrix1_file);
    fclose(matrix2_file);
    
    int i, j, k;
    // Perform element-wise multiplication
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            for (k = 0; k < n; k++) {
                result_matrix[i][j][k] = matrix_1[i][j][k] * matrix_2[i][j][k];
            }
        }
    }

    // Open result.bin for writing
    FILE *result_file = fopen("result.bin", "wb");

    if (result_file == NULL) {
        perror("Error opening output file");
        return 1;
    }

    // Write the dimension of the result_matrix
    fwrite(&n, sizeof(int), 1, result_file);

    // Write the elements of the result_matrix
    fwrite(result_matrix, sizeof(long), n * n * n, result_file);

    // Close the output file
    fclose(result_file);
    printf("Matrix 1:\n");  
    print_matrix(n, matrix_1);

    printf("Matrix 2:\n");    
    print_matrix(n, matrix_2);
    
    printf("Result Matrix:\n");
    print_matrix(n, result_matrix);
    return 0;
}




