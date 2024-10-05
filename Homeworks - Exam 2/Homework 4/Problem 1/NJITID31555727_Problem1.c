#include <stdio.h>

#define MAXSIZE 100

int main(void)
{
    unsigned int TOTAL_BITS = 32;
    int i;
    int numElements = 0;
    int j;
    printf("How many numbers do you want to sort? "); 

    scanf("%d", &numElements);

    while (numElements < 1 || numElements > MAXSIZE)
    {
        printf("Re-enter number of elements: ");
        scanf("%d", &numElements);
    }

    // Unsigned Integer Array of inputted size
    unsigned int values[numElements];
    int currElement = 1;
    for (i = 0; i < numElements; i++)
    {
        printf("Enter element #%d: ", currElement++);
        scanf("%u", &values[i]);
    }

    int counterBucketOne = 0;
    int counterBucketTwo = 0;
    int temp;
    int totallyCoolCounter = 0;
    
    int d;
    
    unsigned int bucketOne[numElements];
    unsigned int bucketTwo[numElements];

    for (d = 0; d < TOTAL_BITS; d++)
    {
        // Sort A using d-th bit as key

        for (i = 0; i < numElements; i++)
        {
            temp = 1 & (values[i] >> d);
            if (temp == 1) 
            {

                bucketTwo[counterBucketTwo] = values[i];
                
                counterBucketTwo += 1;
            }
            else 
            {
                bucketOne[counterBucketOne] = values[i];
                
                counterBucketOne += 1;
            }

        }

        for (i = 0; i < counterBucketOne; i++) {
            values[i] = bucketOne[i];

        }
        for (j = counterBucketOne; j < counterBucketOne + counterBucketTwo; j++)
        {

            values[j] = bucketTwo[totallyCoolCounter];
            totallyCoolCounter += 1;
        }

        counterBucketOne = 0;
        counterBucketTwo = 0;
        totallyCoolCounter = 0;
    }

    printf("\nRadix Sort for Unsigned Integers\n");
    for (i = 0; i < numElements; i++)
    {
        
        printf("%u ", values[i]);
       
    }
    printf("\n");

    return 0;
}
