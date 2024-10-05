#include <stdio.h> 
#include <stdlib.h> 
struct node {    
    int data;     
    // Points to next node
    struct node *next; 
};
// Empty Head Node 
struct node *head = NULL;
void insertNewValue(int);
void display();
void displayRev(struct node *);

int main(void) 
{     
    // Holds inputted value
    int n;     
    int repeat = 1;
    printf("Enter any set of numbers (do CTRL+D to stop):\n");
    while (repeat)     
    {     
        if (scanf("%d", &n) == EOF)             
            break;         
        else             
            insertNewValue(n); 
        
    }     
    display();      
    if (head != NULL) { 
    	printf("\nThe list in reverse is:\n");
    	displayRev(head);  
    }
    return 0;
}

void insertNewValue(int item)
{    
    struct node *newNode;
    struct node *findLast;      
    // Create a link
    newNode = (struct node *) malloc( sizeof( struct node));        
    if (newNode == NULL) {
        printf("Memory Allocation Failure!\n");
        return;
    }
    // Does newNode have data?
    // Verifies each node has an integer (can't enter NULL integers)
    if (newNode != NULL)     
    { 
        // Gets data for each node
        newNode->data = item;     
        // Empty Linked List    
        if (head == NULL)         
        { 
            // Next node after first is empty
            newNode->next = NULL;            
            // Makes newNode the head node
            // and content within
            head = newNode;         
        }        
        
        // Otherwise, find last node and add it
        else         
        {             
            findLast = head;             
            // Iterate through list until we find that
            // NULL is after the last node
            while (findLast->next != NULL)             
            {                 
                findLast = findLast->next;             
            }                
            // Adds new node to end of list
            findLast->next = newNode;     
            // Next pointer points to NULL (nothing)
            newNode->next = NULL;         
        }     
    } 
}

void display() {     
    struct node *iterator;     
    iterator = head;           
    if (iterator == NULL)
    {
        printf("Nothing in list!\n");
    }   
    else {
        printf("\n\nData entered in the list are:\n");  
        // Iterates through linked list       
        while (iterator != NULL)         
        {    
            // Outputs data and then goes to next node
            printf("Data = %d\n", iterator->data);             
            iterator = iterator->next;         
        }  
    }   
} 
 

void displayRev(struct node *head) 
{      
    if (head == NULL)         
        return;     
    displayRev(head->next);     
    
    printf("Data = %d\n", head->data); 
}  

