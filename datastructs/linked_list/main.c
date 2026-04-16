/**
 * Ernest Stover
 * Implementing Linked List in C
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/// CONSTANTS
#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_RESET "\x1b[0m"

/// STRUCTS
// Node struct for a singly linked list
struct Node
{
    int data;
    struct Node *next;
};

struct Node *createNode(int data)
{
    // allocate memory for a new Node
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));

    // check if memory allocation was successful
    if (newNode == NULL)
    {
        printf("Memory allocation failed !\n");
        return NULL; // Return NULL to indicate an error
    }

    // Initialize the new node's data and next pointer
    newNode->data = data;
    newNode->next = NULL; // New node is always the last, so it's next is null
    return newNode;
}

/// FUNCTIONS
/**
 * @brief adds a value to the end of list
 * @param headRef pointer to head of list
 * @param data number to be added
 */
void append(struct Node **headRef, int data)
{
    // create a new node with data
    struct Node *newNode = createNode(data);
    if (newNode == NULL)
    {
        return; // Exit if node creation failed
    }

    // If the list is empty, the new node becomes head
    if (*headRef == NULL)
    {
        *headRef = newNode;
        return; // New node is now the head, nothing else to do
    }

    // Traverse the list to find the last node
    struct Node *current = *headRef;
    while (current->next != NULL)
    {
        current = current->next;
    }

    // Link the last node to the new node
    current->next = newNode;
}

/**
 * @brief adds a value to the front of list
 * @param headRef pointer to head of list
 * @param data number to be added to list
 */
void prepend(struct Node **headRef, int data)
{
    // create a new node with data
    struct Node *newNode = createNode(data);
    if (newNode == NULL)
    {
        return; // Exit if node creation failed
    }

    // if the list is empty, the new node becomes head
    if (*headRef == NULL)
    {
        *headRef = newNode;
        printf(COLOR_GREEN "NUM PREPENDED" COLOR_RESET "\n");
        return; // New node is now the head, nothing else to do
    }

    // set new node pointer to head
    newNode->next = *headRef;
    *headRef = newNode;
    printf(COLOR_GREEN "NUM PREPENDED" COLOR_RESET "\n");
}

/**
 * @brief removes the last node of the list
 */
void removeLastNode(struct Node **headRef)
{
    if (*headRef == NULL)
    {
        printf(COLOR_RED "LIST IS EMPTY\n" COLOR_RESET);
        return;
    }

    // traverse the list to find the last node
    struct Node *current = *headRef;
    struct Node *prev;
    if (current->next == NULL)
    {
        *headRef = NULL;
    }
    while (current->next != NULL)
    {
        prev = current;
        current = current->next;
    }
    prev->next = NULL;
    free(current);
    printf(COLOR_GREEN "NUM REMOVED" COLOR_RESET "\n");
}

/**
 * @brief Prints all elements on the list
 * @param head A pointer to the head of the list
 */
void printList(struct Node *head)
{
    struct Node *current = head;
    printf("List: ");
    while (current != NULL)
    {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n"); // Indicate the end of the list
}

/**
 * @brief Frees all memory allocated for the linked list to prevent memory leaks
 * @param head A pointer to the head of the list
 */
void freeList(struct Node *head)
{
    struct Node *current = head;
    struct Node *nextNode;
    while (current != NULL)
    {
        nextNode = current->next; // store the next node before freeing current
        free(current);            // free the current node
        current = nextNode;       // Move to the next node
    }
    printf("\nLIST MEMORY FREED\n");
}

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

int main()
{
    // Initialize an empty linked list
    struct Node *head = NULL;

    int option; // for input
    int num;

    /// VISUAL LOOP
    while (true)
    {
        printf("----------------------------------------------\n"); // print intro
        printf("| Welcome to my LinkedList visualization in C |\n");
        printf("----------------------------------------------\n");

        printf("\n");
        printList(head); // printing list

        printf("\n| 1. append | 2. prepend | 3. remove | 9. quit |\n"); // show options
        printf("\n");
        printf("pick a number from the options above: ");
        scanf("%d", &option);

        if (option == 9) // input check
        {                // quit visual
            freeList(head);
            head = NULL;
            break;
        }
        else if (option == 1 || option == 2 || option == 3)
        { // execute option
            switch (option)
            {
            case 1:
                printf("Enter num to append: ");
                scanf("%d", &num);
                append(&head, num);
                printf(COLOR_GREEN "NUM APPENDED" COLOR_RESET "\n");
                break;
            case 2:
                printf("Enter num to prepend: ");
                scanf("%d", &num);
                prepend(&head, num);
                break;
            case 3:
                removeLastNode(&head);
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

    return 0; // end program
}