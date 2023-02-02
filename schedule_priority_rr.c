#include "list.h"
#include "schedulers.h"
#include "task.h"
#include "cpu.h"

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"

const int timeQ = 10;

bool rrBool = 0;

struct node *g_head = NULL;
struct node *rr_head = NULL;

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

  while(temp != NULL) {
    if(temp->task->priority > best_sofar->priority){
      if (comesBefore(temp->task->name, best_sofar->name)){
          best_sofar = temp->task;
      }
    }
    temp = temp->next;
  }
  delete (&g_head, best_sofar);
  temp = g_head; 
  
  
  while(temp != NULL){
    if(temp->task->priority == best_sofar->priority){ 
      rrBool = 1;
      insert(&rr_head, temp->task);
      delete (&g_head, temp->task);
    }
    temp = temp->next;
  }

  return best_sofar;
}

Task *pickNextTaskRR() {
  if (!rr_head) {
    return NULL;
  }

  struct node *temp;
  temp = rr_head;
  Task *best_sofar = temp->task;
  
  delete (&rr_head, best_sofar);
  return best_sofar;
}

void cpuUtilitization(int totalTime, int dispatcherTime) {
  float result;
  result = (totalTime / (float)dispatcherTime) * 100;
  printf("CPU Utilization: %.2f%%\n", result);
}

void schedule()
{
  int currTime = 0;
  int switchTime = 0;
  Task *task = pickNextTask();
  
  while(task){
    if(rrBool) { 
        while(task){
        if(task->burst > timeQ){
          run(task, timeQ);
          task->burst -= timeQ;
          currTime += timeQ;
          switchTime++;
          insert(&rr_head, task);
        }
        else {
          run(task, task->burst);
          currTime += task->burst;
          switchTime++;
          free(task->name); 
        }
        printf("%22s%d\n", "Time is now : ", currTime);
        task = pickNextTaskRR();
       }
       rrBool = 0;
    }
    else {
        run(task, task->burst);
        currTime += task->burst;
        switchTime++;
        printf("%22s%d\n", "Time is now : ", currTime);
        free(task->name); 
    }
    task = pickNextTask();
  }
  int dispatcherTime = currTime + switchTime - 1;
  cpuUtilitization(currTime, dispatcherTime);
}

