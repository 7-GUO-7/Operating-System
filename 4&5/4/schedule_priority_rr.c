#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "schedulers.h"
#include "task.h"
#include "list.h"
#include "cpu.h"

static struct node *list = NULL;
static struct node *queue = NULL;
static struct task *last = NULL;

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

void find_lowest_priority(struct node *tmp)
{
    if (last == NULL)
        last = tmp -> task;
    else
    {
        if (last->priority > tmp->task->priority )
            last = tmp -> task;
    }
}

void execute(struct node *tmp) //execute current process and add the remaining process to queue
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
    int current_priority = 0; //the current priority
    queue = list; //now queue is the list
    list = NULL;
    while( queue != NULL)
    {
        last = NULL;
        traverse(queue, find_lowest_priority);
        delete(&queue, last);
        insert(&list, last);
        // add the element in queue back to list, and with reverse arrangement.
    }
    while(list!=NULL) //same priority are in queue, lower are in list
    {
        head = list;
        if (queue != NULL && head->task->priority < current_priority)
            //vips are still doing!
        {
            traverse(queue, reverse);
            queue = NULL;
        }
        else{
            list = list->next;
            current_priority = head->task->priority;
            execute(head);
        }
        if(list == NULL) //the last priority are moved in the stack
        {
            traverse(queue, reverse);
            queue = NULL;
        }
    }

}

