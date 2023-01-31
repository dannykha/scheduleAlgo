

#include "list.h"
#include "schedulers.h"
#include "task.h"

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"

struct node *g_head = NULL;
int taskCnt = 0;

void add(char *name, int priority, int burst) {
    Task* tempTask = (Task*) malloc(sizeof(Task));
    tempTask->name = name;
    tempTask->priority = priority;
    tempTask->burst = burst;
    insert(&g_head, tempTask);
    taskCnt++;
}

bool comesBefore(char *a, char *b) { return strcmp(a, b) < 0; }

// based on traverse from list.c
// finds the task whose name comes first in dictionary
Task *pickNextTask() {
  // if list is empty, nothing to do
  if (!g_head)
    return NULL;

  struct node *temp;
  temp = g_head;
  Task *best_sofar = temp->task;

  while (temp != NULL) {
    if (comesBefore(temp->task->name, best_sofar->name))
      best_sofar = temp->task;
    temp = temp->next;
  }
  // delete the node from list, Task will get deleted later
  delete (&g_head, best_sofar);
  return best_sofar;
}

void schedule() {
    int currTime = 0;
    int switchCnt = 0;

    while(g_head) {
        Task *task = pickNextTask();
        run(task, task->burst);
        switchCnt++;
        currTime += task->burst;
        printf("Time is now : %d\n", currTime);
        delete(&g_head, task);
    }
}