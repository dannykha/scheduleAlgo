/**
 * Representation of a task in the system.
 */

#ifndef TASK_H
#define TASK_H

// length of a task
#define MAX_TASKS 128

// representation of a task
typedef struct task {
    char *name;
    int tid;
    int priority;
    int burst;
    int runsNum;
    int timesRun;
} Task;

#endif
