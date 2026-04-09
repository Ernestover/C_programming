/**
* Calculator program 
* Info: calculator program using bit shifting 
* f(x) = O(x)
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>


/*------- function stubs -------*/
void printIntro();
char showOperators();
void doMath(char op);


int main() {
    printIntro();
    return 0;
}

/**
 * @brief prints the intro statement of the program 
 */
void printIntro() {
    printf("Welcome to my Calculator program!!!\n");
    for(char i=0; i<40;i++) printf("-");
    printf("\nAllowed operands: +,-,*,/ \n");
    
}


/**
 * @brief does the math operation based on the operator 
 * @note returns the result to the terminal 
 */
void doMath(char op) {

}
