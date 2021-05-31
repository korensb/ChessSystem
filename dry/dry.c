#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* reverseEvenString (char* str, int* x)  //error 1 - we need to give the function a meaningfull name
    {  //error 2 - we need to give the { of a function its own line
    if (str == NULL){  //error 3 - we need to check if str is not NULL
        return NULL;
    }
    if (x == NULL){  //error 4 - we need to check check if x is not NULL
        return NULL;
    }
    char* str2;
    int i;
    *x = strlen(str); // error 5 - we need to dereference the pointer to put the value in it
    str2 = malloc(sizeof(char)*((*x)+1)); //errors 6, 7 - we need to multiply the memory needed with sizeof char (conventions error), needs to consider the null space.
    if (str2 == NULL){
        return NULL; //error 7 - we need to make sure malloc succeded
    }
    for (i = 0; i < *x; i++) //error 8 - we need to make an indent line and use {}
    {
        str2[i] = str[*x -i-1]; //error 9 - we need to add '-1' to the i index to not slip out of array
    }
    if (*x % 2 == 0) {
        printf("%s", str);
    }
    if (*x % 2 != 0)
    {
        printf("%s", str2);
    }
    return str2;
}