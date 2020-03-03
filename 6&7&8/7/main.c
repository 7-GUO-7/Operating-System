#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LENGTH 32
//#define SPACE(node) ((node)->hi - (node)->lo + 1)




typedef struct BLOCK{
    int low;
    int high;
    char name[NAME_LENGTH];
    struct BLOCK *next;
}block;


int size(block *node)
{
    return node->high - node->low + 1;
}


block *head;

void init(int size){
    head = (block*)malloc(sizeof(block));
    block *node = (block*)malloc(sizeof(block));
    node->low = 0;
    node->high = size-1;
    strcpy(node->name, "Unused");
    node->next = NULL;
    head->next = node;
}

//delete a node
void delete_node(block **node){
    block *tmp;
    tmp = (*node)->next;
    (*node)->next = tmp->next;
    free(tmp);
}

void free_list(){
    while (head->next != NULL)
    {
        delete_node(&head);
    }
    free(head);
}

void status(){
    block *p = head->next;
    while (p != NULL)
    {
        if(p->next == NULL && p->name[0] == 'U'){
            printf("Addresses [%d] . . .\n", p->low);
            break;
        }
        else{
            printf("Addresses [%d:%d] %s\n", p->low, p->high, p->name);
            p = p->next;
        }
    }
}

// request memory
// return 0 if succeed, otherwise return -1
int request(char name[], int request_size, char choice) {
    block *p;
    // target->next is the space to insert
    block *target;
    switch (choice) {
        // worst fit
        case 'W':
            target = NULL;
            p = head;
            while (p->next != NULL) {
                if (p->next->name[0] == 'U'
                    && size(p->next) >= request_size &&
                    (target == NULL || size(p->next) > size(target->next))) {
                    target = p;
                }
                p = p->next;
            }
            break;
            // best fit
        case 'B':
            target = NULL;
            p = head;
            while (p->next != NULL) {
                if (p->next->name[0] == 'U' &&
                    size(p->next) >= request_size &&
                    (target == NULL || size(p->next) < size(target->next))) {
                    target = p;
                }
                p = p->next;
            }
            break;
            // first fit
        case 'F':
            target = head;
            while (target->next != NULL &&
                   (target->next->name[0] != 'U' || size(target->next) < request_size))
                target = target->next;
            break;
        default:
            printf("Error! No such flag, please enter W or B or F!\n");
            return -1;
    }
    // failed
    if(target == NULL || target->next == NULL)
//    if (target == NULL)
    {
        printf("Memory overflow\n");

        return -1;
    }
    else{
        // succeed and just fit
        if (size(target->next) == request_size) {
            strcpy(target->next->name, "Process ");
            strcat(target->next->name, name);
        }
            // succeed but need to split
        else {
            p = (block *) malloc(sizeof(block));
            p->low = target->next->low;
            p->high = p->low + request_size - 1;
            target->next->low += request_size;
            strcpy(p->name, "Process ");
            strcat(p->name, name);
            p->next = target->next;
            target->next = p;
        }
        return 0;
    }
}


// release memory
// return 0 if succeed, otherwise return -1
int release(char process_name[]){
    char name[64] = "Process ";
    block *p = head;
    strcat(name, process_name);
    while(p->next != NULL){
        if(!strcmp(p->next->name, name)){
            int flag = 1;
            // merge backward
            if(p->next->next != NULL && p->next->next->name[0] == 'U')
            {
                p->next->next->low = p->next->low;
                delete_node(&p);
                flag = 0;
            }
            // merge forward
            if(p != head && p->name[0] == 'U'){
                p->high = p->next->high;
                delete_node(&p);
                flag = 0;
            }
            // rename
            if(flag){
                strcpy(p->next->name, "Unused");
            }
            return 0;
        }
        p = p->next;
    }
    printf("Process %s is not found\n", process_name);
}

// compact
void compact(){
    int offset = 0;
    block *p = head;
    while (p->next != NULL)
    {
        if(p->next->name[0] == 'U')
        {
            // update last hole
            if(p->next->next == NULL){
                p->next->low -= offset;
                break;
            }
            // delete hole
            else{
                offset += size(p->next);
                delete_node(&p);
            }
        }
        // update address
        else
        {
            p = p->next;
            p->low -= offset;
            p->high -= offset;
        }
    }
}

int main(int argc, char **args){
    int size = 0;
    char command[64];
    char name[NAME_LENGTH];
    char choice[3];
    if(argc > 1){
        size = atoi(args[1]); //change string to int
    }
    init(size);
    while (1)
    {
        printf("allocator > ");
        scanf("%s", command);
        if(command[0] == 'X') break;
        if (command[0] == 'C')
        {
            // compact
            compact();

        }
        switch (command[1])
        {
        case 'L':
            scanf("%s", command);
            release(command);
            break;
        // request
        case 'Q':
            scanf("%s%d%s", name, &size, choice);
            request(name, size, choice[0]); //if entering more, we take the first
            break;
        // print out
        default:
            status();
            break;
        }
    }
    return 0;
}

