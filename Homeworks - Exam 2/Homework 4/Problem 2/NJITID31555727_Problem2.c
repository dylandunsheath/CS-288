#include <stdio.h>

#define MAXSIZE 100

int main()
{
    int i, j;
    // Get count of numbers to be sorted
    int numElements;
    // Comment out to test with bash script
    printf("How many numbers do you want to sort? "); 
    scanf("%d", &numElements);
    
    while (numElements < 1 || numElements > MAXSIZE)
    {
        printf("Re-enter number of elements: ");
        scanf("%d", &numElements);
    }
    
    
    float f[numElements];
    int currentNum = 1;
    // Typecast 
    unsigned int *FtoInt = (unsigned int *)&f;
    // Get Float numbers
    for (i = 0; i < numElements; i++)
    {
        // Comment out to test with bash script
        printf("Enter float number #%d: ", currentNum); 
        scanf("%f", &f[i]);
        currentNum += 1;
    }

    // Radix sort unsigned integers
    for (i = 0; i < 32; i++)
    {
        // Count number of 0s and 1s
        int mask = 1 << i;
        int zeroCount = 0;
        // Count number of zeroes to determine where to place 0s
        // and 1s in the sorted array
        for (j = 0; j < numElements; j++)
        {
            if ((FtoInt[j] & mask) == 0)
            {
                zeroCount++;
            }
        }
        // Set temp array to hold sorted numbers
        int temp[numElements];
        int zeroIndex = 0;
        int oneIndex = zeroCount;
        for (j = 0; j < numElements; j++)
        {
            // If bit is 0, place in tmp array at zeroIndex
            if ((FtoInt[j] & mask) == 0)
            {
                temp[zeroIndex] = FtoInt[j];
                zeroIndex++;
            }
            // Otherwise, place in tmp array after zeroIndex
            else
            {
                temp[oneIndex] = FtoInt[j];
                oneIndex++;
            }
        }
       
        // Copy temp array to numbers array
        for (j = 0; j < numElements; j++)
        {
            FtoInt[j] = temp[j];
        }
    }
    // Re-sort the numbers to move the negative numbers to the front in descending order
    int tmp[numElements];
    int tmpIndex = 0;
    for (i = numElements - 1; i >= 0; i--)
    {
        // If number is negative, place in tmp array
        if (f[i] < 0)
        {
            tmp[tmpIndex] = FtoInt[i];
            tmpIndex++;
        }
    }
    for (i = 0; i < numElements; i++)
    {
        // If number is positive, place in tmp array
        if (f[i] >= 0)
        {
            tmp[tmpIndex] = FtoInt[i];
            tmpIndex++;
        }
    }
    // Copy tmp array to numbers array
    for (i = 0; i < numElements; i++)
    {
        FtoInt[i] = tmp[i];
    }

    // Print sorted Float numbers
    printf("Sorted Array of Floats:\n"); // Comment out entier line for bash diff testing
    for (i = 0; i < numElements; i++)
    {
        // Set precision to 2 when printing
        printf("%.2f ", f[i]);
    }
    printf("\n");
}

