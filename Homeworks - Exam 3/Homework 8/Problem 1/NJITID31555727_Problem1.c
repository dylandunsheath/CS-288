#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MAX 1000

int *arr;

// MPI Count sort function
void *countElems(int *arr, int size, int rank, int world_size, char filename[])
{
    MPI_Request request;
    MPI_Status status;

    // Get the number of elements to be sorted by each process
    int numElems = size / world_size;
    // Get the starting index of the array for each process
    int start = rank * numElems;
    // Get the ending index of the array for each process
    int end = start + numElems;
    // Create a count array to store the count of each unique element
    int *count = (int *)malloc(sizeof(int) * MAX);

    // Initialize the count array to 0
    for (int i = 0; i < MAX; i++)
    {
        count[i] = 0;
    }

    // Sort the elements in the array
    // Uncomment the line below if you want to to ensure the parallel process works
    //printf("Process: %d, Start: %d, End: %d\n", rank, start, end);
    for (int i = start; i < end; i++)
    {
        int val = arr[i];
        count[val]++;
        // Uncomment the line below if you want to check the count sort (w/ count of values) and 
        // the values are evenly split into N - 1 processes
        //printf("\tRank: %d, Index: %d, Count: %d\n", rank, val, count[val]);
    }

    if (rank == 0)
    {
        // Send the count array to 1
        MPI_Isend(count, MAX, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
        // Wait for the count from final process
        MPI_Wait(&request, &status);
        // Receive the count array from final process
        int *finalCount = (int *)malloc(sizeof(int) * MAX);
        MPI_Recv(finalCount, MAX, MPI_INT, world_size - 1, 0, MPI_COMM_WORLD, &status);
        // Sort the array using the count array
        int index = 0;
        for (int i = 0; i < MAX; i++)
        {
            if (finalCount[i] > 0)
            {
                for (int j = 0; j < finalCount[i]; j++)
                {
                    arr[index] = i;
                    index++;
                }
            }
        }

        // Print the sorted array
        //int total = 0;
        //for (int i = 0; i < size; i++)
        //{
            // printf("%d ", arr[i]);
            //total += arr[i];
        //}
        //printf("Total: %d", total);
        //printf("\n");

        // Save the sorted array to the file
        FILE *file = fopen(filename, "wb");
        fwrite(arr, sizeof(int), size, file);
        fclose(file);
    }
    else
    {
        // Receive the count array from previous process
        int *prevCount = (int *)malloc(sizeof(int) * MAX);
        MPI_Recv(prevCount, MAX, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);
        // Add the count array to the previous count array
        for (int i = 0; i < MAX; i++)
        {
            count[i] += prevCount[i];
        }
        // Send the count array to next process
        if (rank != world_size - 1)
        {
            MPI_Isend(count, MAX, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, &request);
        }
        else
        {
            // Send the count array to 0
            MPI_Isend(count, MAX, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
        }
    }
    return NULL;
}

int main(int argc, char **argv)
{
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    //printf("# of Processes: %d\n", world_size);
    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    if (world_size < 2) {
        printf("Program requires at least two processes to run!\n");
        return 1;
    }

    char fileName[256];
    int size = 0;
    if (world_rank == 0)
    {
        // Ask User for the file name
        printf("Enter the file name: ");
        scanf("%255[^\n]", fileName);

        // Open the binary file
        FILE *file = fopen(fileName, "rb");
        while (file == NULL)
        {
            printf("File not found. Enter the file name again: ");
            scanf("%255[^\n]", fileName);
            file = fopen(fileName, "rb");
        }
        // Save number of ints in the file and get size
        int num;
        while (fread(&num, sizeof(int), 1, file))
        {
            // printf("%d ", num);
            // Dynamically allocate memory for the array
            arr = (int *)realloc(arr, sizeof(int) * (size + 1));
            arr[size] = num;
            size++;
        }
        // Close the file
        fclose(file);
        int remainder = size % world_size;
        if (remainder != 0) {
            printf("\n%s with %d values in the file cannot be evenly distributed among %d processes.\n\n", fileName, size, world_size);
            return 1;
        }
        // Send the size to other processes
        MPI_Send(&size, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        // Send the array to process 1
        MPI_Send(arr, size, MPI_INT, 1, 0, MPI_COMM_WORLD);
        // Call the MPI Count sort function
        countElems(arr, size, world_rank, world_size, fileName);
    }
    else
    {
        // Receive the size from previous process
        MPI_Recv(&size, 1, MPI_INT, world_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // Allocate memory for the array
        arr = (int *)malloc(sizeof(int) * size);
        // Receive the array from previous process
        MPI_Recv(arr, size, MPI_INT, world_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // Send the array to next process
        if (world_rank != world_size - 1)
        {
            MPI_Send(&size, 1, MPI_INT, world_rank + 1, 0, MPI_COMM_WORLD);
            MPI_Send(arr, size, MPI_INT, world_rank + 1, 0, MPI_COMM_WORLD);
        }
        // Call the MPI Count sort function
        countElems(arr, size, world_rank, world_size, fileName);
    }
    // Finalize the MPI environment.
    MPI_Finalize();
    if (world_rank == 0) {
        printf("Exiting Program...\n");
    }
    return 0;
}


