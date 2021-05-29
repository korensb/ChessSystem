#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct node_t {
    int x;
    struct node_t *next;
} *Node;

typedef enum {
    SUCCESS=0,
    MEMORY_ERROR,
    UNSORTED_LIST,
    NULL_ARGUMENT,
} ErrorCode;

int getListLength(Node list);
bool isListSorted(Node list);
Node mergeSortedLists(Node list1, Node list2, ErrorCode* error_code);
// add necessary declarations here

int getListLength(Node list) {
    int len=0;
    Node i=list;
    while(i){
        len++;
        i=i->next;
    }
    return len;
}

bool isListSorted(Node list) {
    Node i=list;
    while(i && i->next){
        if ((i->x)>(i->next->x))
        {
            return false;
        }
        i=i->next;
    }
    return true;
}

// Start of my code
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
    Node node = (Node)malloc(sizeof(Node));
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

    // end of my code

}

int main(){
    ErrorCode* error_code = malloc(sizeof(*error_code));
    *error_code = SUCCESS;
    Node list1 = createNode(2);
    list1->next = createNode(4);
    list1->next->next=createNode(9);
    Node list2 = createNode(2);
    list2->next = createNode(4);
    list2->next->next=createNode(8);
    Node merged_list = mergeSortedLists(list1,list2,error_code);
    printf("\n%d\n\n", *error_code);
    while(merged_list){
        printf("%d\n",merged_list->x);
        merged_list=merged_list->next;
    }
    destroyList(list1);
    destroyList(list2);
    destroyList(merged_list);
    list1 = createNode(1);
    list1->next = createNode(4);
    list1->next->next=createNode(9);
    list2 = createNode(2);
    list2->next = createNode(5);
    list2->next->next=createNode(8);
    merged_list = mergeSortedLists(list1,list2,error_code);
    printf("\n%d\n\n", *error_code);
    while(merged_list){
        printf("%d\n",merged_list->x);
        merged_list=merged_list->next;
    }
    destroyList(list1);
    destroyList(list2);
    destroyList(merged_list);
    list1 = createNode(5);
    list1->next = createNode(4);
    list1->next->next=createNode(9);
    list2 = createNode(2);
    list2->next = createNode(5);
    list2->next->next=createNode(8);
    merged_list = mergeSortedLists(list1,list2,error_code);
    printf("\n%d\n", *error_code);
    while(merged_list){
        printf("%d\n",merged_list->x);
        merged_list=merged_list->next;
    }
    destroyList(list1);
    destroyList(list2);
    destroyList(merged_list);
    list1 = NULL;
    list2 = createNode(2);
    list2->next = createNode(5);
    list2->next->next=createNode(8);
    merged_list = mergeSortedLists(list1,list2,error_code);
    printf("\n%d\n", *error_code);
    while(merged_list){
        printf("%d\n",merged_list->x);
        merged_list=merged_list->next;
    }
    return 0;
}