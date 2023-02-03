/*
* file: schedule_priority_rr.c
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

const int timeQ = 10;
int completionTime = 0;

void add(char *name, int priority, int burst) {
    Task* tempTask = (Task*) malloc(sizeof(Task));
    tempTask->name = name;
    tempTask->priority = priority;
    tempTask->burst = burst;
    insert(&g_head, tempTask);
    completionTime += burst;
}

bool comesBefore(char *a, char *b) { 
  return strcmp(a, b) < 0; 
}

// helper function to find the highest priority currently
int findHighestPriority() {
  struct node *temp; 
  temp = g_head;
  int maxTemp = temp->task->priority; 

  while(temp != NULL) {
    if(temp->task->priority > maxTemp) {
      maxTemp = temp->task->priority; 
    }
    temp = temp->next; 
  }
  return maxTemp; 
}

// function to pick next task in terms of round robin
// uses a variable minRuns to track how many runs each task has gotten so far
// also uses a helper function called findHighestPriority to help find the highest priority
Task *pickNextTask() {
  if (!g_head) {
    return NULL;
  }
  struct node *temp;
  temp = g_head;
  Task *best_sofar = temp->task; 
  int minRuns = temp->task->timesRun; 

  while (temp != NULL) {
    if (temp->task->timesRun <= minRuns) {
      if (temp->task->priority == findHighestPriority()) {
        minRuns = temp->task->timesRun;
        best_sofar = temp->task;
      }
    }
    temp = temp->next;
  }
  delete (&g_head, best_sofar);
  return best_sofar;
}

void printCPUutil(int totalTime, int dispatcherTime) {
  printf("CPU Utilization: %.2f%%\n", (float)totalTime / dispatcherTime * 100);
}

// function to schedule
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
    if (currTime >= completionTime) {
      break;
    }
  }
  int dispatcherTime = currTime + switchTime - 1;
  printCPUutil(currTime, dispatcherTime);
}

