#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "schedulers.h"
#include "task.h"
#include "list.h"
#include "cpu.h"


static struct node *list = NULL;
static struct task *shortest_task;

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

void find_SJ(struct node *tmp)
{
    if (shortest_task == NULL)
        shortest_task = tmp -> task;
    else
    {
        if (shortest_task->burst > tmp->task->burst )
            shortest_task = tmp -> task;
    }
}


// invoke the scheduler
void schedule() {
    struct node *first_node  = list->next;
    list->next = NULL;
    traverse(first_node, reverse);
    while( list != NULL)
    {
    shortest_task = NULL;
    traverse(list, find_SJ);
    run (shortest_task, shortest_task -> burst);
    delete (&list, shortest_task);
    free (shortest_task);
    }
}


