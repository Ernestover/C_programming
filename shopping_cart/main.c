/*
Shopping Cart program 
f(x) = O(x)
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIZE 21
#define FMT "%20s"

// global variables
char g_store[6][15] = {"juice", "milk", "eggs", "chicken", "cereal", "poptarts"};
float g_prices[6] = {3.45, 2.99, 2.25, 7.89, 5.36, 4.12};

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
    printf("Here is the list of merchandise in my store: \n");
    int length = sizeof(g_store) / sizeof(g_store[0]); 
    for (int x=0; x<length; x++) {
        printf("%s : $%.2f\n",g_store[x], g_prices[x]);
    }
    return; 
}

/**
 * @brief parse input from the user and return their total from the items they picked
 * @note input must be in the form (item, item, item, ...)
 */
void returnTotal() {
    // local variables 
    float total = 0.0;
    int letter;
    char user_items[50]; // size of 50 allocated for user items 
    int i = 0;
    int stock_check = 0;

    printf("\n\nPlease make your selection from the list of items above: ");
    printf("\nFormat for input -> item, item, ... : ");

    while ((letter = getchar()) != '\n' && letter != EOF);

    while ((letter = getchar()) != '\n' && i<49) {
        user_items[i++] = (char)letter;
    }
    user_items[i] = '\0'; // mark end of string
    
    char *token = strtok(user_items, ", ");
    while (token != NULL) {
       for (int i=0; i<6; i++) {
            if (strcmp(token, g_store[i]) == 0) {
                total += g_prices[i];
                printf("%s added: $%.2f\n", g_store[i], g_prices[i]);
            }
            else {stock_check++;}
       }
        token = strtok(NULL, ", ");
    }

    printf("\nYour total is: $%.2f\n", total);
    if (stock_check != 0) {printf("\nOne of the items you selected is not in stock at the moment.");}
        
    return;
}


