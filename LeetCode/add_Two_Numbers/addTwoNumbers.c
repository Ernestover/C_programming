/**
 * You are given two non-empty linked lists representing two non-negative integers. 
 * The digits are stored in reverse order, and each of their nodes contains a single digit. 
 * Add the two numbers and return the sum as a linked list.
 */

 #include <stdio.h>

 struct ListNode {
    int val;
    struct ListNOde *next;
 };

 struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2);

 int main() {

 }

struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2) {
    struct ListNode dummy = {0,NULL};
    struct ListNode* cur = &dummy;
    int carry = 0;
    while(l1 || l2 || carry) {
        int result = carry;
        if (l1) {result+=l1->val; l1=l1->next;}
        if (l2) {result+=l2->val; l2=l2->next;}
        carry = result / 10;
        struct ListNode* node = malloc(sizeof(struct ListNode));
        node->val = result%10;
        node->next = NULL;
        cur->next = node;
        cur = cur->next;
    }
    return dummy.next;
}