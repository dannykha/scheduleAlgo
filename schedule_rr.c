/*
* file: schedule_rr.c
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

const int timeQ = 10;

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

// function to pick next task in terms of round robin
// uses a variable minRuns to track how many runs each task has gotten so far
Task *pickNextTask() {
  // if list is empty nothing to do
  if (!g_head) {
    return NULL;
  }

  struct node *temp;
  temp = g_head;
  Task *best_sofar = temp->task;
  int minRuns = temp->task->timesRun;

  while (temp != NULL) {
    if (temp->task->timesRun <= minRuns) {
      minRuns = temp->task->timesRun;
      best_sofar = temp->task;
    }
    temp = temp->next;
  }
  delete (&g_head, best_sofar);
  return best_sofar;
}

// print the cpu utilization
void printCPUutil(int totalTime, int dispatcherTime) {
  printf("CPU Utilization: %.2f%%\n", (float)totalTime / dispatcherTime * 100);
}

// scheduler and ensuring each task only gets the time quantum of 10
// or how much time to run they have left
void schedule() {
  int currTime = 0;
  int switchTime = 0;
  
  while (g_head != NULL) {
    Task *task = pickNextTask();
    int timeToRun;
    if (task->burst <= timeQ) {
      timeToRun = task->burst;
    } 
    else {
      timeToRun = timeQ;
    }
    run(task, timeToRun);
    task->timesRun++;
    currTime += timeToRun;
    switchTime++;
    task->burst -= timeToRun;
    if (task->burst > 0) {
      insert(&g_head, task);
    }
    printf("%22s%d\n", "Time is now : ", currTime);
  }
  int dispatcherTime = currTime + switchTime - 1;
  printCPUutil(currTime, dispatcherTime);
}
