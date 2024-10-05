#include <stdio.h>
#include <stdlib.h>

struct node
{
    int data;
    struct node *next;
};

struct node *head = NULL;
int main()
{
   
    struct node *current = NULL;
    struct node *temp = NULL;
    // Loop over input
    int input;
    printf("Enter any set of numbers (do CTRL+D to stop):\n");
    while (scanf("%d", &input) != EOF)
    {
        // Create new node
        current = (struct node *)malloc(sizeof(struct node));
        if (current == NULL) {
            printf("Memory Allocation Failure!\n");
            return -1;
        }   
        current->data = input;
        current->next = NULL;
        // Add to List
        if (head == NULL)
        {
            head = current;
        }
        else
        {
            temp = head;
            while (temp->next != NULL)
            {
                temp = temp->next;
            }
            temp->next = current;
        }
    }
    
    if (head == NULL) {
        printf("Nothing in list!\n");
    }
    
    
    else 
    {
        temp = head;
        printf("\n\nData entered in the list are:\n");
        // Iterates through linked list
        while (temp != NULL)
        {
            // Outputs data and then goes to next node
            printf("Data = %d\n", temp->data);
            temp = temp->next;
        }
        
        // bubble sorts
        int swap = 1;
        while (swap)
        {
            swap = 0;
            current = head;
            struct node *prev = NULL;
            struct node *next = current->next;
            while (next != NULL)
            {
                if (current->data > next->data)
                {
                    swap = 1;
                    if (prev != NULL)
                    {
                        prev->next = next;
                    }
                    else
                    {
                        head = next;
                    }
                    current->next = next->next;
                    next->next = current;
                    prev = next;
                    next = current->next;
                }
                else
                {
                    prev = current;
                    current = next;
                    next = next->next;
                }
            }
        }
        // Print out Sorted List
        printf("Sorted Linked List:\n");
        temp = head;
        while (temp != NULL)
        {
          printf("Data = %d\n", temp->data);
          temp = temp->next;
        }
    }

    return 0;    
}
