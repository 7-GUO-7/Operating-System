#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "schedulers.h"
#include "task.h"
#include "list.h"
#include "cpu.h"


static struct node *list = NULL;
static struct task *VIP_task;

// add a task to the list 
void add(char *name, int priority, int burst, int tid){
    struct task *t = malloc(sizeof(struct task));
    t->name = name;
    t->priority = priority;
    t->burst = burst;
    t->tid = tid;
    insert(&list, t);
}


void reverse(struct node *temp){
    insert(&list, temp->task);
    free(temp);
}

void find_VIP(struct node *tmp)
{
    if (VIP_task == NULL)
        VIP_task = tmp -> task;
    else
    {
        if (VIP_task->priority < tmp->task->priority )
            VIP_task = tmp -> task;
    }
}


// invoke the scheduler
void schedule() {
    struct node *first_node  = list->next;
    list->next = NULL;
    traverse(first_node, reverse);
    while( list != NULL)
    {
    VIP_task = NULL;
    traverse(list, find_VIP);
    run (VIP_task, VIP_task -> burst);
    delete (&list, VIP_task);
    free (VIP_task);
    }
}

