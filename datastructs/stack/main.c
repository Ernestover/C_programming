/**
 * Ernest Stover
 * Implementing Stack in C 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/// CONSTANTS
#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_RESET "\x1b[0m"

/// STRUCTS
// Node struct for stack
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
 * @brief push items to the top of the stack
 * @param data info to be pushed to stack
 * @param topRef entry point to the stack
 */
void push(struct Node **topRef, int data)
{
    // create the newNode
    struct Node *newNode = createNode(data);
    if (newNode == NULL)
    {
        return; // failed to create
    }

    // set new Node as top
    newNode->next = *topRef;
    *topRef = newNode;
    printf("%d added...\n", data);
}

/**
 * @brief pop items from the top of the stack
 */
void pop(struct Node **topRef)
{
    // is stack empty
    if (*topRef == NULL)
    {
        printf(COLOR_RED "STACK IS EMPTY\n" COLOR_RESET);
        return;
    }

    // Store the current top node in a temporary pointer
    struct Node *temp = *topRef;

    // Update topRef to point to the next node (the new top)
    *topRef = (*topRef)->next;

    // Free the memory of the old top node
    free(temp);
    printf(COLOR_GREEN "POPPED\n" COLOR_RESET);
}

/**
 * @brief print all the contents on the stack
 * @param top entry point to the stack
 */
void printAll(struct Node *top)
{
    struct Node *current = top;
    printf("\n--- Stack Contents (Top to Bottom) ---\n");

    // Check if the stack is empty
    if (current == NULL)
    {
        printf("|   (Empty)  |\n");
    }

    // Traverse the stack from top to bottom
    // the loop continues as long as 'current is not NULL
    while (current != NULL)
    {
        printf("|     %d     |\n", current->data);
        current = current->next; // move to the enxt node
    }

    // base of the stack
    printf("|    NULL    |\n");
    printf("--------------------------------------\n");
}

/**
 *@brief frees all memory allocated to the stack
 @param top entry point to the stack
 */
void freeStack(struct Node *top)
{
    struct Node *current = top;
    struct Node *nextNode;
    while (current != NULL)
    {
        nextNode = current->next; // store the next Node
        free(current);            // free the current node
        current = nextNode;       // move to the next node
    }
    printf("\nSTACK MEMORY FREED\n");
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
    // intialize a empty stack
    struct Node *top = NULL;

    int option; // for input
    int num;

    /// VISUAL LOOP
    while (true)
    {
        printf("----------------------------------------------\n"); // print intro
        printf("|    Welcome to my Stack Visualization in C  |\n");
        printf("----------------------------------------------\n");

        printf("\n");
        printAll(top); // printing stack

        printf("\n | 1. push | 2. pop | 9. quit |"); // show options
        printf("\n");
        printf("pick a number from the options above: ");
        scanf("%d", &option);

        if (option == 9)
        { // input check
            freeStack(top);
            top = NULL;
            break;
        }
        else if (option == 1 || option == 2 || option == 3)
        { // execute option
            switch (option)
            {
            case 1:
                printf("Enter num to push: ");
                scanf("%d", &num);
                push(&top, num);
                printf(COLOR_GREEN "NUM PUSHED" COLOR_RESET "\n");
                break;

            case 2:
                pop(&top);
                break;
            }
        }
        else
        { // input validation
            printf("\n");
            printf(COLOR_RED "invalid option, pick from the options above\n" COLOR_RESET);
        }

        printf("----------------------------------------------\n");
        printf("\n");
        pressEnterToContinue();
        printf("\n");
    }

    return 0; // END
}