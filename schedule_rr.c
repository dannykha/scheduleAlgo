

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

Task *pickNextTask() {
  // if list is empty, nothing to do
  if (!g_head) {
    return NULL;
  }

  struct node *temp;
  temp = g_head;
  Task *best_sofar = temp->task; 
  int minRuns = temp->task->runsNum;

  while (temp) {
    if (temp->task->runsNum <= minRuns) {
      minRuns = temp->task->runsNum;
      best_sofar = temp->task;
    }
    temp = temp->next;
  }

  temp = g_head;

  while (temp) {
    if (temp->task->runsNum == minRuns) {
      best_sofar = temp->task;
    }
    temp = temp->next;
  }

  delete (&g_head, best_sofar);
  return best_sofar;
}

void cpuUtilitization(int totalTime, int dispatcherTime) {
  float result;
  result = (totalTime / (float)dispatcherTime) * 100;
  printf("CPU Utilization: %.2f%%\n", result);
}

void schedule() {
    int currTime = 0;
    int switchTime = 0;

    struct node *temp;
    temp = g_head;

    while (temp) {
      Task* task = pickNextTask();
      if (task->burst >= timeQ) {
        run(task, timeQ);
        task->runsNum++;
        currTime += timeQ;
        switchTime++;
        task->burst -= timeQ;

        if (task->burst == 0) {
          printf("%22s%d\n", "Time is now : ", currTime);
          continue;
        }
        insert(&g_head, task);

      }
      else {
        if (task->burst == 0) {
          continue;
        }
        run(task, task->burst);
        task->runsNum++;
        currTime += task->burst;
        switchTime++;
        task->burst -= task->burst;
      }
      printf("%22s%d\n", "Time is now : ", currTime);
      temp = temp->next;

      if (temp == NULL) {
        if (g_head != NULL) {
          temp = g_head;
        }
      }
  }
  int dispatcherTime = currTime + switchTime - 1;
  cpuUtilitization(currTime, dispatcherTime);
}