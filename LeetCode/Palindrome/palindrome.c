/**
 * Given an integer x, return true if x is a palindrome, and false otherwise
 * Palindrome: a int that reads the same backwards and forwards 
 */
#include <stdio.h>
#include <stdbool.h>

bool isPalindrome(int x);

int main() {
    printf("%d\n",isPalindrome(121));
    printf("%d\n",isPalindrome(-121));
    printf("%d\n",isPalindrome(10));
}

bool isPalindrome(int x) {
    if (x < 0) {return false;} // neg case
    int reverse = 0, remainder = 0, dummy = 0;
    dummy = x;
    while (dummy != 0) {
        remainder = dummy % 10;
        reverse = reverse * 10 + remainder;
        dummy /= 10;
    }
    if (reverse == x) {return true;}
    return false;
}