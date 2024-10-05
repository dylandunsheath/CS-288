#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 4
#define NxN (N*N)
#define TRUE 1
#define FALSE 0

struct node
{
    int tiles[N][N];
    int f, g, h;
    short zero_row, zero_column;    /* location (row and colum) of blank tile 0 */
    struct node *next;
    struct node *parent;            /* used to trace back the solution */
};

int goal_rows[NxN];
int goal_columns[NxN];
struct node *start, *goal;
struct node *open = NULL, *closed = NULL;
struct node *succ_nodes[4];

void print_a_node(struct node *pnode)
{
    int i, j;
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
            
            printf("%2d ", pnode->tiles[i][j]);
        
        printf("\n");
        
    }
    printf("\n");
}

struct node *initialize(char **argv)
{
    int i, j, k, index, tile;
    struct node *pnode;
    pnode = (struct node *)malloc(sizeof(struct node));
    index = 1;
    for (j = 0; j < N; j++)
        for (k = 0; k < N; k++)
        {
            tile = atoi(argv[index++]);
            pnode->tiles[j][k] = tile;
            if (tile == 0)
            {
                pnode->zero_row = j;
                pnode->zero_column = k;
            }
        }
    pnode->f = 0;
    pnode->g = 0;
    pnode->h = 0;
    pnode->next = NULL;
    pnode->parent = NULL;
    
    start = pnode;
    printf("initial state\n");
    print_a_node(start);
    
    pnode = (struct node *)malloc(sizeof(struct node));
    goal_rows[0] = 3;
    goal_columns[0] = 3;
    
    for (index = 1; index < NxN; index++)
    {
        j = (index - 1) / N;
        k = (index - 1) % N;
        goal_rows[index] = j;
        goal_columns[index] = k;
        pnode->tiles[j][k] = index;
        
    }
    
    pnode->tiles[N-1][N-1]=0;       /* empty tile=0 */
    pnode->f = 0;
    pnode->g = 0;
    pnode->h = 0;
    pnode->next = NULL;
    goal = pnode;
    printf("goal state\n");
    print_a_node(goal);
    
    return start;
}

void free_memory() {
    // Free memory for start and goal nodes
    free(start);
    free(goal);

    // Free memory for nodes in open list
    struct node *temp = open;
    while (temp != NULL) {
        struct node *next = temp->next;
        free(temp);
        temp = next;
    }

    // Free memory for nodes in closed list
    temp = closed;
    while (temp != NULL) {
        struct node *next = temp->next;
        free(temp);
        temp = next;
    }
    int i;
    // Free memory for successor nodes in expand function
    for (i = 0; i < N; i++) {
        free(succ_nodes[i]);
    }
}

void merge_to_open()
{
    int i;
    for (i = 0; i < N; i++)
    {
        if (succ_nodes[i] == NULL)
        {
            continue; 
            
        }
        struct node *toInsert = (struct node *)malloc(sizeof(struct node));
        memcpy(toInsert->tiles, succ_nodes[i]->tiles, NxN * sizeof(int));

        toInsert->f = succ_nodes[i]->f;
        toInsert->g = succ_nodes[i]->g;
        toInsert->h = succ_nodes[i]->h;
        toInsert->zero_row = succ_nodes[i]->zero_row;
        toInsert->zero_column = succ_nodes[i]->zero_column;
        toInsert->parent = succ_nodes[i]->parent;
        
        if (open == NULL)
        {
            open = toInsert;
            continue; 
            
        } 
        struct node *temp = open;
        int hasInserted = FALSE;
                
        while (temp != NULL && temp->next != NULL)
        {
            if (toInsert->f < temp->next->f)
            {
                toInsert->next = temp->next;
                temp->next = toInsert;
                hasInserted = TRUE;
                break;
                
            }
            temp = temp -> next;
        
        }
        if (hasInserted == FALSE)
        {
            temp->next = toInsert;
            
        }
    }
}

void swap(int row1, int column1, int row2, int column2, struct node *pnode)
{
      int temp;
      temp = pnode -> tiles[row1][column1];
      pnode -> tiles[row1][column1] = pnode -> tiles[row2][column2]; 
      pnode -> tiles[row2][column2] = temp;
}

// Implemented due to its convenience over Misplaced Tiles (and effeciency)
int manhattanDist(int entry, int row, int col)
{
    int i, j;
      if (entry == 0)
      {
          return 0;
      }
      
      for (i = 0; i < NxN; i++)
      {
            for (j = 0; j < NxN; j++)
            {
                // Loop until current tile (goal) is entry
                if (goal->tiles[i][j] == entry)
                {
                
                    // Calculate Manhattan Distance
                    // Formula: |X1 – X2| + |Y1 – Y2|
                    return abs(row - i) + abs(col - j);
                }
            }
      }
      return 0;
}

// Min value of two different unique values
int min(int a, int b)
{
    return(a > b) ? b : a;
}

void update_fgh(struct node *pnode)
{
    /* **** UPDATES g **** */
    if (pnode->parent != NULL)
    {
        
        pnode->g = pnode->parent->g + 1;
    }
    else {
            pnode->g = 1;
        
    }
    int h1 = 0, h2 = 0;
    int i, j;
    int correct = 0;
    
    for (i = 0; i < NxN; i++)
    {
        for (j = 0; i < NxN; i++)
        {
            correct++;
            if (pnode->tiles[j][i] != correct)
            {
                h1++;
                
            }
            h2 += manhattanDist(pnode->tiles[j][i], j, i);
            
        }
        
    }
    /* **** UPDATES f and h **** */
    pnode->h = min(h1, h2);
    pnode->f = pnode->g + pnode->h;
    
}

void move_down(struct node *pnode)
{
    if (pnode->zero_row + 1 < N)
    {
        swap(pnode->zero_row, pnode->zero_column, pnode->zero_row + 1, pnode->zero_column, pnode);
        pnode->zero_row++;
        
    } 
    pnode = NULL;
}

void move_right(struct node *pnode)
{
    if (pnode->zero_column + 1 < N)
    {
        swap(pnode->zero_row, pnode->zero_column, pnode->zero_row, pnode->zero_column + 1, pnode);
        pnode->zero_column++;
    }
    pnode = NULL;
              
}

void move_up(struct node *pnode)
{
    if (pnode->zero_row - 1 > -1)
    {
        swap(pnode->zero_row, pnode->zero_column, pnode->zero_row - 1, pnode->zero_column, pnode);
        pnode->zero_row--;
    }
    pnode = NULL;
}

void move_left(struct node *pnode)
{
    if (pnode->zero_column - 1 > -1)
    {
        swap(pnode->zero_row, pnode->zero_column, pnode->zero_row, pnode->zero_column - 1, pnode);
        pnode->zero_column--;
    } 
    pnode = NULL;
}
void expand(struct node *selected)
{
    int i;
    for (i = 0; i < N; i++)
    {
        succ_nodes[i] = (struct node *)malloc(sizeof(struct node));
        memcpy(succ_nodes[i]->tiles, selected->tiles, NxN * sizeof(int));
        succ_nodes[i]->zero_row = selected->zero_row;
        succ_nodes[i]->zero_column = selected->zero_column;
        succ_nodes[i]->parent = selected;
    }
    move_down(succ_nodes[0]);
    move_right(succ_nodes[1]);
    move_up(succ_nodes[2]);
    move_left(succ_nodes[3]);
    for (i = 0; i < N; i++)
    {
        update_fgh(succ_nodes[i]);
    }
}

int nodes_same(struct node *a, struct node *b)
{
    int flg = FALSE;
    if (memcmp(a->tiles, b->tiles, sizeof(int) * NxN) == 0)
        flg = TRUE;
    
    return flg; 
}

void filter(int i, struct node *pnode_list)
{
    struct node *pnode = succ_nodes[i];
    if (pnode_list == NULL || pnode == NULL)
    {
        return; 
    }
    struct node *temp = pnode_list;
    while (temp != NULL)
    {
        if (nodes_same(succ_nodes[i], temp))
        {
            succ_nodes[i] = NULL;
            return; 
        }
        temp = temp->next;
          }
}

int countInversions(struct node *pnode) {
    int inversions = 0;
    int puzzle[NxN];
    int k = 0;
    int i, j;
    // Flatten the puzzle to a 1D array for easier counting
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            puzzle[k++] = pnode->tiles[i][j];
        }
    }

    // Count inversions
    for (i = 0; i < NxN - 1; i++) {
        for (j = i + 1; j < NxN; j++) {
            if (puzzle[i] && puzzle[j] && puzzle[i] > puzzle[j]) {
                inversions++;
            }
        }
    }

    return inversions;
}

// Check if the puzzle is solvable
int isSolvable(struct node *pnode) {
    int inversions = countInversions(pnode);
    int blankRow = pnode->zero_row;
    
    if (N % 2 == 0) { // Even grid width
        return (inversions + blankRow) % 2 == 1;
    }
}

// Ensures all values are unique:
int hasDupes(char *args[], int argc) {
   int i, j;
   // Starts at 1 because argv[0] is the executable
   for (i = 1; i < argc; i++) 
   {
      for (j = i + 1; j < argc; j++)
      {
         if (strcmp(args[i], args[j]) == 0) {
            return 1; // Duplicate Found
         }
      }
   }
   return 0; // No duplicates
}


int main(int argc, char **argv)
{
    
    int j;
    // Checks Number of arguments is EXACTLY 17
    if (argc != 17) {
       printf("ERROR: Program requires EXACTLY 17 arguments: %s <Numbers from 0-15>\n", argv[0]);
       return 1;
    }
    
    // Checks if numbers are 0 - 15:
    
    for (j = 1; j < argc; j++) {
       int num = atoi(argv[j]);
       
       if (num < 0 || num > 15) {
          printf("Arguments must be values in the range 0 - 15 inclusively!\n");
          return 1;
       } 
    }
    
    if (hasDupes(argv, argc)) {
       printf("ERROR: values must be unique!\n");
       return 1;
    }
    int iter, cnt;
    struct node *copen, *cp, *solution_path;
    int ret, i, pathlen = 0, index[N - 1];
    solution_path = NULL;
    start = initialize(argv);
    if (!isSolvable(start)) {
       printf("No Solution Found.\n");
       return 1;
    }
    open = start;
    iter = 0;
    while (open != NULL)
    {
        copen = open;
        open = open->next;
        if (nodes_same(copen, goal))
        {
            do {
                copen->next = solution_path;
                solution_path = copen;
                copen = copen->parent;
                pathlen++;
            } 
            while (copen != NULL);
            printf("Path (length=%d):\n", pathlen);
            copen = solution_path;
            do {
                print_a_node(solution_path);
                solution_path = solution_path->next;
            } while (solution_path != NULL);
            break;
        }
        expand(copen);
        for (i = 0; i < 4; i++)
        {
            filter(i, open);
            filter(i, closed);
            update_fgh(copen);
        }
        merge_to_open();
        copen->next = closed;
        closed = copen;
        iter++;
        if (iter % 1000 == 0)
            printf("iter %d\n", iter);
    }

    // Free all dynamically allocated memory
    free_memory();

    return 0; 
}

