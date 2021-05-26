#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct node_t {
	int x;
	struct node_t* next;
} *Node;

typedef enum {
	SUCCESS = 0,
	MEMORY_ERROR,
	UNSORTED_LIST,
	NULL_ARGUMENT,
} ErrorCode;

int getListLength(Node list);
bool isListSorted(Node list);
Node mergeSortedLists(Node list1, Node list2, ErrorCode* error_code);
void destroyList(Node ptr);


//Implemention:
void destroyList(Node ptr) 
{
    while (ptr) {
        Node toDelete = ptr;
        ptr = ptr->next;
        free(toDelete);
    }
}


    Node mergeSortedLists(Node list1, Node list2, ErrorCode * error_code)
    {

        if (list1 == NULL || list2 == NULL)
        {
            *error_code = NULL_ARGUMENT;
            return NULL;
        }

        if (isListSorted(list1) != true && isListSorted(list2) != true)
        {
            *error_code = UNSORTED_LIST;
            return NULL;
        }

        Node head = NULL;
        head = (Node*)malloc(sizeof(Node));
        if (head == NULL) {
            *error_code = MEMORY_ERROR;
            return NULL;
        }
        if (list1->x > list2->x)
        {
            head->x = list1->x;
            list1->next = list1->next->next;
        }
        else
        {
            head->x = list2->x;
            list2->next = list2->next->next;
        }

        head->next = NULL;

        Node current = head;

        while (list1->next != NULL && list2->next != NULL)
        {
            current->next = (Node*)malloc(sizeof(Node));
            if (current->next == NULL) {
                *error_code = MEMORY_ERROR;
                destroyList(head);
                return NULL;
            }
            current = current->next;
            current->next = NULL;

            if (list1->x > list2->x)
            {
                current->x = list1->x;
                list1->next = list1->next->next;
            }
            else
            {
                current->x = list2->x;
                list2->next = list2->next->next;
            }
        }

        while (list1->next != NULL)
        {
            current->next = (Node*)malloc(sizeof(Node));
            if (current->next == NULL) {
                *error_code = MEMORY_ERROR;
                destroyList(head);
                return NULL;
            }
            current = current->next;
            current->next = NULL;

            current->x = list1->x;
            list1->next = list1->next->next;
        }

        while (list2->next != NULL)
        {
            current->next = (Node*)malloc(sizeof(Node));
            if (current->next == NULL) {
                *error_code = MEMORY_ERROR;
                destroyList(head);
                return NULL;
            }
            current = current->next;
            current->next = NULL;

            current->x = list2->x;
            list2->next = list2->next->next;
        }

        *error_code = SUCCESS;
        return head;
    }