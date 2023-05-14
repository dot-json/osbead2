#include "framework.h"

int main() {
    struct Employee employees[MAX_EMPLOYEES];
    initEmployees(employees);

    int file_size = count_lines(FILENAME);

    read_file(FILENAME, file_size, employees);
    write_file(FILENAME, employees);
    int capacity[] = {10, 9, 7, 8, 10};
    int **schedule = initSchedule(MAX_EMPLOYEES);

    runMenu(schedule, employees, capacity);
    return 0;
}

int count_lines(const char *filename) {
    FILE *fp = fopen(filename, "r");
    int count = 0;
    char c;

    if (fp == NULL) {
        printf("Error opening file %s", filename);
        return -1;
    }

    while ((c = fgetc(fp)) != EOF) {
        if (c == '\n') {
            count++;
        }
    }

    fclose(fp);
    return count;
}

void read_file(char *filename, int size, struct Employee *employees) {
    FILE *fp;
    char c;
    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file");
        exit(1);
    }
    char line[256];
    int i = 0;
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = '\0';
        char *divider = strchr(line, ';');
        *divider = '\0';
        char *name = line;
        char *days = divider + 1;
        int workdays[5] = {0, 0, 0, 0, 0};
        char *day;
        for (int i = 0; i < 5; i++) {
            switch (i) {
                case 0:
                    day = "hetfo";
                    break;
                case 1:
                    day = "kedd";
                    break;
                case 2:
                    day = "szerda";
                    break;
                case 3:
                    day = "csutortok";
                    break;
                case 4:
                    day = "pentek";
                    break;
            }
            if (strstr(days, day) != NULL) {
                workdays[i] = 1;
            }
        }

        strcpy(employees[i].name, name);
        memcpy(employees[i].workdays, workdays, sizeof(workdays));
        i++;
    }
    fclose(fp);
}

void write_file(char *filename, struct Employee *employees) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Error opening file");
        exit(1);
    }
    for (int i = 0; i < getEmployeeSize(employees); i++) {
        char line[256];
        strcpy(line, employees[i].name);
        strcat(line, ";");
        for (int j = 0; j < 5; j++) {
            if (employees[i].workdays[j] == 1) {
                switch (j) {
                    case 0:
                        strcat(line, "hetfo ");
                        break;
                    case 1:
                        strcat(line, "kedd ");
                        break;
                    case 2:
                        strcat(line, "szerda ");
                        break;
                    case 3:
                        strcat(line, "csutortok ");
                        break;
                    case 4:
                        strcat(line, "pentek ");
                        break;
                }
            }
        }
        if (i != getEmployeeSize(employees) - 1) {
            line[strlen(line) - 1] = '\n';
        } else {
            line[strlen(line) - 1] = '\0';
        }
        fputs(line, fp);
    }
    fclose(fp);
}

int **initSchedule(int size) {
    int **schedule = (int **)malloc(5 * sizeof(int *));
    for (int i = 0; i < 5; i++) {
        schedule[i] = (int *)malloc(size * sizeof(int));
    }
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < size; j++) {
            schedule[i][j] = -1;
        }
    }
    return schedule;
}

void initEmployees(struct Employee *employees) {
    for (int i = 0; i < MAX_EMPLOYEES; i++) {
        strcpy(employees[i].name, "");
        memset(employees[i].workdays, 0, sizeof(employees[i].workdays));
    }
}

int getEmployeeSize(struct Employee *employees) {
    int size = 0;
    for (int i = 0; i < MAX_EMPLOYEES; i++) {
        if (employees[i].name[0] != '\0') {
            size++;
        }
    }
    return size;
}

int getEmployeeIndex(struct Employee *employees, char *name) {
    for (int i = 0; i < MAX_EMPLOYEES; i++) {
        if (strcmp(employees[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

int getEmployeesPerDay(int **schedule, int day) {
    int count = 0;
    for (int i = 0; i < MAX_EMPLOYEES; i++) {
        if (schedule[day][i] != -1) {
            count++;
        }
    }
    return count;
}

void newEmployee(struct Employee *employees, int size, int capacity[]) {
    char name[128];
    int workdays[5] = {0, 0, 0, 0, 0};
    char days[128];
    printf("Enter name: ");
    scanf(" %[^\n]s", name);
    printf("Enter workdays: ");
    scanf(" %[^\n]s", days);
    char *day;
    for (int i = 0; i < 5; i++) {
        switch (i) {
            case 0:
                day = "hetfo";
                break;
            case 1:
                day = "kedd";
                break;
            case 2:
                day = "szerda";
                break;
            case 3:
                day = "csutortok";
                break;
            case 4:
                day = "pentek";
                break;
        }
        if (strstr(days, day) != NULL) {
            workdays[i] = 1;
        }
    }
    bool empty = true;
    for (int i = 0; i < 5; i++) {
        if (capacity[i] == 0 && workdays[i] == 1) {
            workdays[i] = 0;
        }
        if (workdays[i] == 1) {
            empty = false;
        }
    }
    if (empty) {
        printf("The selected workdays are full!\n");
        return;
    }
    strcpy(employees[size].name, name);
    memcpy(employees[size].workdays, workdays, sizeof(workdays));
}

void removeEmployee(struct Employee *employees, int size, int index) {
    for (int i = index; i < size; i++) {
        strcpy(employees[i].name, employees[i + 1].name);
        memcpy(employees[i].workdays, employees[i + 1].workdays,
               sizeof(employees[i + 1].workdays));
    }
    strcpy(employees[size - 1].name, "");
    memset(employees[size - 1].workdays, 0,
           sizeof(employees[size - 1].workdays));
}

void modifyEmployee(struct Employee *employees, int capacity[], int size, int index) {
    char name[128];
    int workdays[5] = {0, 0, 0, 0, 0};
    char days[128];
    printf("* Enter 'same' to keep the same data *\n");
    printf("Enter name: ");
    scanf(" %[^\n]s", name);
    printf("Enter workdays: ");
    scanf(" %[^\n]s", days);
    char *day;
    if (strcmp(name, "same") != 0) {
        for (int i = 0; i < strlen(name); i++) {
            if (name[i] == '_') {
                name[i] = ' ';
            }
        }
        strcpy(employees[index].name, name);
    }
    if (strcmp(days, "same") != 0) {
        bool empty = true;
        for (int i = 0; i < 5; i++) {
            switch (i) {
                case 0:
                    day = "hetfo";
                    break;
                case 1:
                    day = "kedd";
                    break;
                case 2:
                    day = "szerda";
                    break;
                case 3:
                    day = "csutortok";
                    break;
                case 4:
                    day = "pentek";
                    break;
            }
            if (strstr(days, day) != NULL) {
                workdays[i] = 1;
            }
        }
        for (int i = 0; i < 5; i++) {
            if (capacity[i] == 0 && workdays[i] == 1) {
                workdays[i] = 0;
            }
            if (workdays[i] == 1) {
                empty = false;
            }
        }
        if (empty) {
            printf("The selected workdays are full!\n");
            return;
        }
        memcpy(employees[index].workdays, workdays, sizeof(workdays));
    }
}

void scheduleEmlpoyees(int **schedule, int size, int capacity[], struct Employee *employees) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < 5; j++) {
            if (employees[i].workdays[j] == 1) {
                for (int k = 0; k < size; k++) {
                    if (schedule[j][k] == i || capacity[j] == 0) {
                        break;
                    }
                    if (schedule[j][k] == -1) {
                        schedule[j][k] = i;
                        capacity[j]--;
                        break;
                    }
                }
            }
        }
    }
}

void descheduleEmployees(int **schedule, int size, int capacity[], struct Employee *employees) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < 5; j++) {
            if (employees[i].workdays[j] == 1) {
                for (int k = 0; k < size; k++) {
                    if (schedule[j][k] == i) {
                        schedule[j][k] = -1;
                        capacity[j]++;
                        break;
                    }
                }
            }
        }
    }
}

void printSchedule(int **schedule, int size, struct Employee *employees) {
    char *days[] = {"HETFO", "KEDD", "SZERDA", "CSUTORTOK", "PENTEK"};
    for (int i = 0; i < 5; i++) {
        printf("------------\n%s \n", days[i]);
        for (int j = 0; j < size; j++) {
            if (schedule[i][j] != -1) {
                printf(" - %s \n", employees[schedule[i][j]].name);
            }
        }
    }
}

void printEmployees(struct Employee *employees, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d: %s - ", i, employees[i].name);
        for (int j = 0; j < 5; j++) {
            if (employees[i].workdays[j] == 1) {
                switch (j) {
                    case 0:
                        printf("hetfo ");
                        break;
                    case 1:
                        printf("kedd ");
                        break;
                    case 2:
                        printf("szerda ");
                        break;
                    case 3:
                        printf("csutortok ");
                        break;
                    case 4:
                        printf("pentek");
                        break;
                }
            }
        }
        printf("\n");
    }
}

void printCapacity(int capacity[]) {
    char *days[] = {"HETFO", "KEDD", "SZERDA", "CSUTORTOK", "PENTEK"};
    for (int i = 0; i < 5; i++) {
        printf("%s: %d \n", days[i], capacity[i]);
    }
}

// write a function that counts the employees in a string and the employees are separated by newlines
int countEmployeesOnBus(char *bus) {
    int count = 0;
    for (int i = 0; i < strlen(bus); i++) {
        if (bus[i] == '\n') {
            count++;
        }
    }
    return count;
}

void sigusr1_handler(int sig) {
    if (sig == SIGUSR1) {
        printf("------------\nBus 1 is ready!\n");
    }
}

void sigusr2_handler(int sig) {
    if (sig == SIGUSR2) {
        printf("------------\nBus 2 is ready!\n");
    }
}

void checkDayAndStartBusses(int **schedule, struct Employee *employees, int day) {
    int size = ceil(getEmployeesPerDay(schedule, day) / 5.0);
    if (size == 0) {
        printf("No employees on this day!\n");
        return;
    } else {
        printf("Starting %d bus(es)!\n", size);
    }

    int fd_0[2];
    int fd_1[2];
    pid_t pid[2];
    pid_t wpid;
    int status;
    int msgid[2];
    msg_buffer msgbuf[2];

    if (pipe(fd_0) < 0 || pipe(fd_1) < 0) {
        perror("Error creating pipes");
        return;
    }

    for (int i = 0; i < size; i++) {
        msgid[i] = msgget(IPC_PRIVATE, 0666 | IPC_CREAT);
        if (msgid[i] == -1) {
            perror("Error creating message queue");
            return;
        }
    }

    signal(SIGUSR1, sigusr1_handler);
    signal(SIGUSR2, sigusr2_handler);

    for (int i = 0; i < size; i++) {
        pid[i] = fork();
        if (pid[i] == 0) {
            if (i == 0) {
                close(fd_0[1]);
                kill(getppid(), SIGUSR1);
                char buffer[1024];
                read(fd_0[0], buffer, sizeof(buffer));
                printf("Employees:\n%s", buffer);
                msgbuf[0].msg_type = 1;
                int employeeCount = countEmployeesOnBus(buffer);
                char employeeCountStr[10];
                memset(employeeCountStr, 0, sizeof(employeeCountStr));
                sprintf(employeeCountStr, "%d", employeeCount);
                snprintf(msgbuf[0].msg_text, 128, "%s", employeeCountStr);
                if (msgsnd(msgid[0], &msgbuf[0], 128, 0) == -1) {
                    perror("Error sending message");
                    return;
                }
                close(msgid[0]);
                exit(0);
            } else {
                wpid = waitpid(pid[0], &status, 0);
                close(fd_1[1]);
                kill(getppid(), SIGUSR2);
                char buffer[1024];
                read(fd_1[0], buffer, sizeof(buffer));
                printf("Employees:\n%s", buffer);
                msgbuf[1].msg_type = 1;
                int employeeCount = countEmployeesOnBus(buffer);
                char employeeCountStr[10];
                memset(employeeCountStr, 0, sizeof(employeeCountStr));
                sprintf(employeeCountStr, "%d", employeeCount);
                snprintf(msgbuf[1].msg_text, 128, "%s", employeeCountStr);
                if (msgsnd(msgid[1], &msgbuf[1], 128, 0) == -1) {
                    perror("Error sending message");
                    return;
                }
                close(msgid[1]);
                exit(0);
            }
        } else {
            sleep(1);
            int n = 0;
            char employee[64];
            if (i == 0) {
                close(fd_0[0]);
                char buffer[1024];
                memset(buffer, 0, sizeof(buffer));
                for (int i = 0; i < getEmployeeSize(employees); i++) {
                    if (schedule[day][i] != -1) {
                        strcat(buffer, employees[schedule[day][i]].name);
                        strcat(buffer, "\n");
                        n++;
                    }
                    if (n == 5) {
                        strcat(employee, employees[schedule[day][i]].name);
                        break;
                    }
                }
                write(fd_0[1], buffer, sizeof(buffer));
                close(fd_0[1]);
            } else if (i == 1) {
                close(fd_1[0]);
                char buffer[1024];
                memset(buffer, 0, sizeof(buffer));
                for (int i = getEmployeeIndex(employees, employee); i < 10; i++) {
                    if (schedule[day][i] != -1) {
                        strcat(buffer, employees[schedule[day][i]].name);
                        strcat(buffer, "\n");
                    }
                }
                write(fd_1[1], buffer, sizeof(buffer));
                close(fd_1[1]);
            }
        }
    }
    if (size == 1) {
        msgrcv(msgid[0], &msgbuf[0], 128, 1, 0);
        printf("------------\nBus 1 arrived with %s employees!\n", msgbuf[0].msg_text);
    } else {
        msgrcv(msgid[0], &msgbuf[0], 128, 1, 0);
        msgrcv(msgid[1], &msgbuf[1], 128, 1, 0);
        printf("------------\nBus 1 arrived with %s employees!\n", msgbuf[0].msg_text);
        printf("------------\nBus 2 arrived with %s employees!\n", msgbuf[1].msg_text);
    }
    for (int i = 0; i < 2; i++) {
        msgctl(msgid[i], IPC_RMID, NULL);
    }
}

void runMenu(int **schedule, struct Employee *employees, int capacity[]) {
    int choice;
    int selectedDay;
    int id;
    scheduleEmlpoyees(schedule, getEmployeeSize(employees), capacity, employees);
    do {
        system("clear");
        printf("1. Register new employee\n");
        printf("2. Remove employee\n");
        printf("3. Modify employee\n");
        printf("4. Schedule all registered employees\n");
        printf("5. Deschedule all registered employees\n");
        printf("6. List employees\n");
        printf("7. List schedule\n");
        printf("8. Check capacity\n");
        printf("9. Check day\n");
        printf("10. Exit\n");
        printf("Enter operation number: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                system("clear");
                newEmployee(employees, getEmployeeSize(employees), capacity);
                scheduleEmlpoyees(schedule, getEmployeeSize(employees), capacity, employees);
                write_file(FILENAME, employees);
                break;
            case 2:
                system("clear");
                printEmployees(employees, getEmployeeSize(employees));
                descheduleEmployees(schedule, getEmployeeSize(employees), capacity, employees);
                printf("Index of employee to remove: ");
                scanf("%d", &id);
                removeEmployee(employees, getEmployeeSize(employees), id);
                write_file(FILENAME, employees);
                scheduleEmlpoyees(schedule, getEmployeeSize(employees), capacity, employees);
                break;
            case 3:
                system("clear");
                printEmployees(employees, getEmployeeSize(employees));
                descheduleEmployees(schedule, getEmployeeSize(employees), capacity, employees);
                printf("Index of employee to modify: ");
                scanf("%d", &id);
                modifyEmployee(employees, capacity, getEmployeeSize(employees), id);
                write_file(FILENAME, employees);
                scheduleEmlpoyees(schedule, getEmployeeSize(employees), capacity, employees);
                break;
            case 4:
                system("clear");
                printf("Scheduling all registered employees...\n");
                scheduleEmlpoyees(schedule, getEmployeeSize(employees), capacity, employees);
                break;
            case 5:
                system("clear");
                printf("Descheduling all registered employees...\n");
                descheduleEmployees(schedule, getEmployeeSize(employees), capacity, employees);
                break;
            case 6:
                system("clear");
                printf("Employees:\n");
                printEmployees(employees, getEmployeeSize(employees));
                break;
            case 7:
                system("clear");
                printf("Schedule:\n");
                printSchedule(schedule, getEmployeeSize(employees), employees);
                break;
            case 8:
                system("clear");
                printf("Capacity:\n");
                printCapacity(capacity);
                break;
            case 9:
                system("clear");
                printf("Select day:\n");
                printf("0. HETFO\n");
                printf("1. KEDD\n");
                printf("2. SZERDA\n");
                printf("3. CSUTORTOK\n");
                printf("4. PENTEK\n");
                printf("Enter day number: ");
                scanf("%d", &selectedDay);
                if (selectedDay < 0 || selectedDay > 4) {
                    printf("Invalid day!\n");
                    break;
                }
                checkDayAndStartBusses(schedule, employees, selectedDay);
                break;
            case 10:
                printf("Exiting...\n");
                return;
                break;
            default:
                system("clear");
                printf("Invalid choice!\n");
                break;
        }

        printf("\nPress enter to continue...");
        getchar();
        getchar();
    } while (choice != 10);
}