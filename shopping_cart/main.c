#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*
Shopping Cart program 
f(x) = O(x)
*/
#define SIZE 21
#define FMT "%20s"

// function stubs 
void greetCustomer();
void showMerch();
void returnTotal();

int main() {
    greetCustomer();
    showMerch();
    returnTotal();
    return 0;
}

/**
 * @brief asks the user for their name then greets them
 */
void greetCustomer() {
    char *name = (char *)malloc(sizeof(char)*SIZE);
    printf("Enter your name: ");
    scanf(FMT, name);
    printf("\nHey %s! Welcome to my store.", name);
    free(name);
    printf("\n\n");

    return; 
}

/**
 * @brief shows the available merchandise and their associated price 
 */
void showMerch() {
    // declaring variables
    char store[6][10] = {"juice", "milk", "eggs", "chicken", "cereal", "poptarts"};
    float prices[6] = {3.45, 2.99, 2.25, 7.89, 5.36, 4.12};

    printf("Here is the list of merchandise in my store: \n");
    int length = sizeof(store) / sizeof(store[0]); 
    for (int x=0; x<length; x++) {
        printf("%s : $%.2f\n",store[x], prices[x]);
    }
    return; 
}

/**
 * @brief parse input from the user and return their total from the items they picked
 * @note input must be in the form (item, item, item, ...)
 */
void returnTotal() {
    // declaring variables 
    int letter;
    char user_items[50]; // size of 50 allocated for user items 
    char *input_list = (char *)malloc(sizeof(char)*30);
    printf("\n\nPlease make your selection from the list of items above: ");
    printf("\nFormat for input -> item, item, ... : ");
    while ((letter = getchar()) != "\n") {
        printf("%c",letter);
    }

    free(input_list);
    return;
}

