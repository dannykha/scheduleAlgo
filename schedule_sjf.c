/*
* file: schedule_sjf.c
* author: Danny Kha
* CSS 430
* P3: Scheduling Algorithms
* Due: 2/12/23
*
*/

#include "list.h"
#include "schedulers.h"
#include "task.h"
#include "cpu.h"

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"

struct node *g_head = NULL;

void add(char *name, int priority, int burst) {
    Task* tempTask = (Task*) malloc(sizeof(Task));
    tempTask->name = name;
    tempTask->priority = priority;
    tempTask->burst = burst;
    insert(&g_head, tempTask);
}

bool comesBefore(char *a, char *b) { 
  return strcmp(a, b) < 0; 
}

// function to pick next task by shortest job first
Task *pickNextTask() {
  // if list is empty nothing to do
  if (!g_head) {
    return NULL;
  }

  struct node *temp;
  temp = g_head;
  Task *best_sofar = temp->task;

  while (temp != NULL) {
    if (temp->task->burst < best_sofar->burst) {
      best_sofar = temp->task;
    }
    // burst is same then choose first
    else if (temp->task->burst == best_sofar->burst) {
      if (comesBefore(temp->task->name, best_sofar->name)) {
        best_sofar = temp->task;
      }
    }
    temp = temp->next;
  }
  // delete the node from list, Task will get deleted later
  delete (&g_head, best_sofar);
  return best_sofar;
}

// function to print cpu utilization
void printCPUutil(int totalTime, int dispatcherTime) {
  printf("CPU Utilization: %.2f%%\n", (float)totalTime / dispatcherTime * 100);
}

// function to schedule with shortest job first
void schedule() {
  int currTime = 0;
  int switchTime = 0;
  
  while(g_head) {
    Task *task = pickNextTask();
    run(task, task->burst);
    currTime += task->burst;
    switchTime++;
    printf("%22s%d\n", "Time is now : ", currTime);
  }
  int dispatcherTime = currTime + switchTime - 1;
  printCPUutil(currTime, dispatcherTime);
}


