#include <iostream>
using namespace std;

struct ListNode {
	int val;
	ListNode* next;
	ListNode(int x) : val(x), next(NULL) {}
};

void printList(ListNode* head) {
    ListNode* tmp = head;
    while (tmp != NULL) {
        cout << tmp->val << ' ';
        tmp = tmp->next;
    }
    cout << endl;
}

ListNode* insertionSortList(ListNode* head) {
    ListNode* current = head->next;
    ListNode* newHead = new ListNode(head->val);

    while(current != NULL){
        ListNode* newPointer = newHead;
        ListNode* next = current->next;

        if(current->val <= newHead->val){
            ListNode* oldHead = newHead;
            newHead = current;
            newHead->next = oldHead;
        }
        else{
            while(newPointer->next != NULL){
                if(current->val > newPointer->val && current->val <= newPointer->next->val){
                    ListNode* oldNext = newPointer->next;
                    newPointer->next = current;
                    current->next = oldNext;                
                }

                newPointer = newPointer->next;
            }

            if(newPointer->next == NULL && current->val > newPointer->val){
                newPointer->next = current;
                current->next = NULL;
            }
        }

        current = next;

    }
    return newHead; // modify the return value appropriately in your implementation
}

bool detectCycle(ListNode* head) {
    ListNode* slow = head;
    ListNode* fast = head;
    while(fast->next->next != NULL && slow->next != NULL){
        slow = slow->next;
        fast = fast->next->next;
        if(slow == fast){
            return true;
        }
    } 
    return false; // modify the return value appropriately in your implementation
}

ListNode* findCycleStart(ListNode* head) {
    ListNode* slow = head;
    ListNode* fast = head;
    int slowCount = 0, fastCount = 0, d = 0;
    while(fast->next->next != NULL && slow->next != NULL){
        slow = slow->next;
        fast = fast->next->next;
        if(slow == fast){
            slow = head;
            while(slow != fast){
                slow = slow->next;
                fast = fast->next;
            }
            return slow;
        }
    }
    
    return NULL; // modify the return value appropriately in your implementation
}
