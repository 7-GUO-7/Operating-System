#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "schedulers.h"
#include "task.h"
#include "list.h"
#include "cpu.h"

static struct node *list = NULL;
static struct node *queue = NULL;

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

void execute(struct node *tmp)
{
    if (tmp->task->burst > QUANTUM)
    {
        run (tmp->task, QUANTUM);
        tmp->task->burst -= QUANTUM;
        insert(&queue ,tmp->task);
    }
    else
    {
        run(tmp ->task, tmp ->task->burst);
        free(tmp->task);
    }
    free(tmp);
}


// invoke the scheduler
void schedule() {
    struct node *head;
    struct node *first_node  = list->next;
    list->next = NULL;
    traverse(first_node, reverse);
    while( list != NULL)
    {
        head = list; // make head move forward
        list = list ->next;
        execute(head);
        if ( list == NULL) //the original list has traverse to end
        {
            list = NULL;
            traverse(queue, reverse); //make the queue reverse and traverse function add the reverse queue to original pointer list
            queue = NULL;
        }

    }
}

