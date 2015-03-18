#ifndef __LINKED_LIST__
#define __LINKED_LIST__

#include <stdlib.h>

struct LinkedListNode
{
    LinkedListNode* next;
    size_t data;
};

class LinkedList
{
public:
    LinkedList(): head(NULL) { };
    ~LinkedList() {
        clear();
    }

    void add(size_t val) {
        LinkedListNode* next = head;
        head = new LinkedListNode();
        head->data = val;
        head->next = next;
    }

    size_t size() {
        size_t res = 0;
        LinkedListNode* it = head;
        while (it != NULL) {
            it = it->next;
            res++;
        }
        return res;
    }

    LinkedListNode* getHead() {
        return head;
    }

    void clear() {
        while (head != NULL) {
            LinkedListNode* temp = head;
            head = temp->next;
            delete temp;
        }
    }

private:
    LinkedListNode* head;
};

#endif /* __LINKED_LIST__ */