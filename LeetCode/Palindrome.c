#include <stdio.h>
#include <stdbool.h>

bool isPalindrome(int x);

int main() {
    isPalindrome(121);
    isPalindrome(-121);
    isPalindrome(10);
}

bool isPalindrome(int x) {
    if (x < 0) {return false;} // neg case
    long reverse = 0, remainder = 0, dummy = 0;
    dummy = x;
    while (dummy != 0) {
        remainder = dummy % 10;
        reverse = reverse * 10 + remainder;
        dummy /= 10;
    }
    printf("%d ? %d\n",reverse, x);
    if (reverse == x) {return true;}
    return false;
}