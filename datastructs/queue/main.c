/**
 * Ernest Stover
 * Implementing Queue in C 
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/// CONSTANTS
#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_RESET "\x1b[0m"

/// STRUCTS
// Node struct for queue
struct Node
{
    int data;
    struct Node *next;
};

// struct for newNode
struct Node *createNode(int data)
{
    // allocate memory for a new Node
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));

    if (newNode == NULL)
    {
        printf("Memory allocation failed !\n");
        return NULL; // return NULL to indicate failure
    }

    // intialize the new Node's data and next pointer
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

/// FUNCTIONS
/**
 * @brief push items to the front of the queue
 * @param data info to be enqueued to the queue
 * @param front entry point to the queue
 */
void enqueue(struct Node **frontRef, int data)
{
    // create the newNode
    struct Node *newNode = createNode(data);
    if (newNode == NULL)
    {
        return; // failed to create
    }

    // If the list is empty, the new node becomes head
    if (*frontRef == NULL)
    {
        *frontRef = newNode;
        return; // New node is now the head, nothing else to do
    }

    // Traverse the list to find the last node
    struct Node *current = *frontRef;
    while (current->next != NULL)
    {
        current = current->next;
    }

    // Link the last node to the new node
    current->next = newNode;
    printf("%d Adding...\n", data);
}

void dequeue(struct Node **frontRef)
{
    // is stack empty
    if (*frontRef == NULL)
    {
        printf(COLOR_RED "QUEUE IS EMPTY\n" COLOR_RESET);
        return;
    }

    // store current front node in temp pointer
    struct Node *temp = *frontRef;
    // update frontRef to point to the next node
    *frontRef = (*frontRef)->next;

    // free the memory of the old front node
    free(temp);
    printf(COLOR_GREEN "DEQUEUED\n" COLOR_RESET);
}

/**
 * @brief Prints all elements on the list
 * @param front a pointer to the fron of the queue
 */
void printQueue(struct Node *front)
{
    struct Node *current = front;
    printf("\nQueue: ");
    while (current != NULL)
    {
        printf("|  %d  ", current->data);
        current = current->next;
    }
    printf("|  NULL \n"); // Indicate the end of the queue
}

/**
 * @brief frees all memory allocated for the Queue to prevent memory leaks
 * @param front a pointer to the head of the list
 */
void freeQueue(struct Node *front)
{
    struct Node *current = front;
    struct Node *nextNode;
    while (current != NULL)
    {
        nextNode = current->next; // store the next node before freeing current
        free(current);            // free the current node
        current = nextNode;       // Move to the next node
    }
    printf("\nQUEUE MEMORY FREED\n");
}

/// INPUT STUFF
void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void pressEnterToContinue()
{
    printf("Press Enter to Continue: ");
    clearInputBuffer();
    while (getchar() != '\n')
        ;
}

/// MAIN
int main()
{
    // Initialize an empty queue
    struct Node *front = NULL;

    int option; // for input
    int num;

    /// VISUAL LOOP
    while (true)
    {
        printf("----------------------------------------------\n"); // print intro
        printf("|   Welcome to my Queue Vizualization in C    |\n");
        printf("----------------------------------------------\n");

        printf("\n");
        printQueue(front); // printing queue

        printf("\n | 1. enqueue | 2. dequeue | 9. quit |"); // show options
        printf("\n");
        printf("pick a number from the options above: ");
        scanf("%d", &option);

        if (option == 9)
        {
            freeQueue(front);
            front = NULL;
            break;
        }
        else if (option == 1 || option == 2 || option == 3)
        {
            switch (option)
            {
            case 1:
                printf("Enter num to enqueue: ");
                scanf("%d", &num);
                enqueue(&front, num);
                printf(COLOR_GREEN "NUM ENQUEUED" COLOR_RESET "\n");
                break;

            case 2:
                dequeue(&front);
                break;
            }
        }
        else
        {
            // input validation
            printf("\n");
            printf(COLOR_RED "invalid option, pick from the options above\n" COLOR_RESET);
        }

        printf("----------------------------------------------\n");
        printf("\n");
        pressEnterToContinue();
        printf("\n");
    }

    return 0; // end program
}
