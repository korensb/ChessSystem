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
    while (ptr != NULL)
    {
        Node toDelete = ptr;
        ptr = ptr->next;
        free(toDelete);
    }
}

Node createNode(int value)
{
    Node node = malloc(sizeof(*node));
    if (node == NULL)
    {
        return NULL;
    }
    node->x = value;
    node->next = NULL;
    return node;
}

Node mergeSortedLists(Node list1, Node list2, ErrorCode* error_code){

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

        Node head;
        if (list1->x < list2->x)
        {
            head = createNode(list1->x);
            list1 = list1->next;
        }
        else
        {
            head = createNode(list2->x);
            list2 = list2->next;
        }

        Node current = head;
        while (list1 != NULL && list2 != NULL)
        {
            if (list1->x < list2->x)
            {
                current->next = createNode(list1->x);
                if (current->next == NULL)
                    {
                        *error_code = MEMORY_ERROR;
                        destroyList(head);
                        return NULL;
                    }
                current = current->next;
                list1 = list1->next;
            }
            else
            {
                current->next = createNode(list2->x);
                if (current->next == NULL)
                    {
                        *error_code = MEMORY_ERROR;
                        destroyList(head);
                        return NULL;
                    }
                current = current->next;

                list2 = list2->next;
            }
        }

        while (list1 != NULL)
        {
             current->next = createNode(list1->x);
                if (current->next == NULL)
                    {
                        *error_code = MEMORY_ERROR;
                        destroyList(head);
                        return NULL;
                    }
                current = current->next;

                list1 = list1->next;
        }

        while (list2 != NULL)
        {
             current->next = createNode(list2->x);
                if (current->next == NULL)
                    {
                        *error_code = MEMORY_ERROR;
                        destroyList(head);
                        return NULL;
                    }
                current = current->next;

                list2 = list2->next;
        }

        *error_code = SUCCESS;
        return head;
    }