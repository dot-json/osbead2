#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include <math.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_EMPLOYEES 50
#define FILENAME "employees"

struct Employee {
    char name[128];
    int workdays[5];
};

typedef struct msg_buffer {
    long msg_type;
    char msg_text[128];
} msg_buffer;

int count_lines(const char *filename);
void read_file(char *filename, int size, struct Employee *employees);
void write_file(char *filename, struct Employee *employees);
int **initSchedule(int size);
void initEmployees(struct Employee *employees);
int getEmployeeSize(struct Employee *employees);
void newEmployee(struct Employee *employees, int size, int capacity[]);
void removeEmployee(struct Employee *employees, int size, int index);
void modifyEmployee(struct Employee *employees, int capacity[], int size, int index);
void scheduleEmlpoyees(int **schedule, int size, int capacity[], struct Employee *employees);
void descheduleEmployees(int **schedule, int size, int capacity[], struct Employee *employees);
void printSchedule(int **schedule, int size, struct Employee *employees);
void printEmployees(struct Employee *employees, int size);
void runMenu(int **schedule, struct Employee *employees, int capacity[]);
void checkDayAndStartBusses(int **schedule, struct Employee *employees, int day);

#endif