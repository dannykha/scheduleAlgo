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

// function to print TAT, WT, and RT
void print_stats(char* taskList[MAX_TASKS], int TAT[MAX_TASKS], int WT[MAX_TASKS], int RT[MAX_TASKS], int numTasks) {
  printf("---|");
  for (int i = 0; i < numTasks; i++) {
    printf(" %s |", taskList[i]);
  }
  printf("\nTAT|");
  for (int i = 0; i < numTasks; i++) {
    printf(" %3d|", TAT[i]);
  }
  printf("\n WT|");
  for (int i = 0; i < numTasks; i++) {
    printf(" %3d|", WT[i]);
  }
  printf("\n RT|");
  for (int i = 0; i < numTasks; i++) {
    printf(" %3d|", RT[i]);
  }
  printf("\n");
}

// function to schedule with shortest job first
void schedule() {
  int currTime = 0;
  int switchTime = 0;
  int taskIndex = 0;

  // arrays to store task list, TAT, WT, and RT
  char* taskList[MAX_TASKS];
  int TAT[MAX_TASKS];
  int WT[MAX_TASKS];
  int RT[MAX_TASKS];
  
  while(g_head) {
    Task *task = pickNextTask();
    run(task, task->burst);
    currTime += task->burst;
    switchTime++;
    taskList[taskIndex] = task->name;
    TAT[taskIndex] = currTime;
    WT[taskIndex] = TAT[taskIndex] - task->burst;
    RT[taskIndex] = TAT[taskIndex] - task->burst;
    taskIndex++;
    printf("%22s%d\n", "Time is now : ", currTime);
  }
  // print CPU utilization
  int dispatcherTime = currTime + switchTime - 1;
  printCPUutil(currTime, dispatcherTime);

  // print TAT, RT, and WT
  print_stats(taskList, TAT, WT, RT, taskIndex);
}


