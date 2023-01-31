

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

Task *pickNextTask() {
  // if list is empty, nothing to do
  if (!g_head) {
    return NULL;
  }
  
  struct node *temp;
  temp = g_head;
  Task *best_sofar = temp->task;

  while (temp != NULL) {
    // if the priority of the best so far is the smallest then keep picking that one
    if (temp->task->priority < best_sofar->priority) {
        best_sofar = temp->task;
    }
    else if (temp->task->priority == best_sofar->priority) {
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

void cpuUtilitization(int totalTime, int dispatcherTime) {
  float result;
  result = (totalTime / (float)dispatcherTime) * 100;
  printf("CPU Utilization: %.2f%%\n", result);
}

void schedule() {
    int currTime = 0;
    int switchTime = 0;
    
    while(g_head) {
        Task *task = pickNextTask();
        run(task, task->burst);
        currTime += task->burst;
        switchTime++;
        printf("Time is now : %d\n", currTime);
    }
    int dispatcherTime = currTime + switchTime - 1;
    cpuUtilitization(currTime, dispatcherTime);
}
