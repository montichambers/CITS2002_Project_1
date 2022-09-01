//
// CITS2002 Project 1 2022
// Student1: 23057892 Chambers Monti
// Student2: 23058262 Nguyen Nathan

/*
 crontab-file:
 minute (int 0-59)
 hour (int 0-24)
 date (int 0-31 depending)
 month (int 0-12)
 day (string mon or monday works)
 command (string)

 estimates-file:
 command (string)
 minutes (int)

 outputs:
 The name of the most frequently executed command (string)
 The total number of commands invoked (unsigned int)
 The maximum number of commands running at any time (unsigned int)

 Open and read files crontab-file and estimates file

 if line begins with #, skip

 given the month, determine:
 which commands run in that month
 count of how many times each command runs
 count of the total number of commands run

 From estimates file list how long each command runs for

 list the amount of commands running at any time

 potential errors:
 file doesn't exist
 too many arguments
 minute, hour, date, month, day out of range
 no name
 ints instead of strings vice versa


 Plan of attack:
    Turn files into array of structures
    Iterate through every minute of given month:
        record whether a program terminates in that minute
        if a program starts in that minute, record it
        use to count how many programs run and are running at 1 time





 PLAN:
    Iterate through each minute
    Iterate through each crontab array
    Determine if a command executes or terminates in that minute (terminate first)
    If a command executes:
        +1 total programs run, +1 current programs running, +1 specific command run, start (timer?) based on minutes from estimates
    If a command terminates:
        -1 current programs running

    Find potential errors

    Test each minute and see how many programs are running
    Use a counter to count  frequency of each program
    Find the name of the most frequently executed command (a single word),
    The total number of commands invoked (a non-negative integer),
    The maximum number of commands running at any time (a non-negative integer).

 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

#define LINE_SIZE 100
#define COMMAND_SIZE 40
#define MAX_COMMANDS 20
#define MINUTES_IN_DAY 1440
#define HOURS_IN_DAY 24

FILE *file_opener(char filename[]) {
    /* Returns  */

    FILE *name = fopen(filename, "r");

    if(name == NULL) {
        fprintf(stderr, "cannot open file '%s'\n", filename);
        exit(EXIT_FAILURE);
    }

    return name;
}

int month_num(char *month){
    /* Returns the integer of a month given as a string */

    if(isnumber(*month)){ // Return the month integer if it is already one
        return atoi(month);
    }
    const char *months[12] = {"january", "february", "march", // all months as full name
                              "april", "may", "june",
                              "july", "august", "september",
                              "october", "november", "december"};
    for(int i = 0; i < 12; i++){
        if(strstr(months[i], month) != NULL){ // Compares whether given month is equal to its long month
            return i + 1;
        }
    }
    fprintf(stderr, "%s is an invalid month\n", month);
    exit(EXIT_FAILURE);
}

int day_num(char *day){
    /* Returns the integer of a day given as a string */

    int day_num;
    if(isnumber(*day)){ // Return the day integer if it is already one
        return atoi(day);
    }
    const char *days[7] = {"monday", "tuesday", "wednesday", // all days as full name
                             "thursday", "friday", "saturday","sunday"};

    for(int i = 0; i < 7; i++){
        if(strstr(days[i], day) != NULL){ // Compares whether given day is a valid day (or substring)
            return i + 1;
        }
    }
    fprintf(stderr, "%s is an invalid day\n", day);
    exit(EXIT_FAILURE);
}

int days_in_month(int month){
    /* Returns the amount of days in the month given as an integer [1-12] */

    int daysinmonth;
    switch (month) {
        case 1:
            daysinmonth = 31;
            break;
        case 2:
            daysinmonth = 28;
            break;
        case 3:
            daysinmonth = 31;
            break;
        case 4:
            daysinmonth = 30;
            break;
        case 5:
            daysinmonth = 31;
            break;
        case 6:
            daysinmonth = 30;
            break;
        case 7:
            daysinmonth = 31;
            break;
        case 8:
            daysinmonth = 31;
            break;
        case 9:
            daysinmonth = 30;
            break;
        case 10:
            daysinmonth = 31;
            break;
        case 11:
            daysinmonth = 30;
            break;
        case 12:
            daysinmonth = 31;
            break;
        default:
            fprintf(stderr, "%i is an invalid month\n", month);
            exit(EXIT_FAILURE);
    }
    return daysinmonth;
}

int first_day(int month){
    /* Returns the first day of the month as an int [0-6] */

    int first_day;
    switch (month) {
        case 1:
            first_day = 6;
            break;
        case 2:
            first_day = 2;
            break;
        case 3:
            first_day = 2;
            break;
        case 4:
            first_day = 5;
            break;
        case 5:
            first_day = 0;
            break;
        case 6:
            first_day = 3;
            break;
        case 7:
            first_day = 5;
            break;
        case 8:
            first_day = 1;
            break;
        case 9:
            first_day = 4;
            break;
        case 10:
            first_day = 6;
            break;
        case 11:
            first_day = 2;
            break;
        case 12:
            first_day = 4;
            break;
        default:
            fprintf(stderr, "%i is an invalid month", month);
            exit(EXIT_FAILURE);
    }
    return first_day;
}

void estimatecron(char *month, FILE *crontab_file, FILE *estimates_file){
    /* Calculates and prints:
     * 1. The name of the most frequently executed command
     * 2. The total number of commands invoked
     * 3. The maximum number of commands running at any time
     */

    // Defining variables
    int i = 0;
    char line[LINE_SIZE];
    int month_int = month_num(month);
    char *always = "*";
    int pid = 0;
    int nrunning = 0;
    int counter_size = 0;
    int crontabs_size = 0;
    int estimates_size = 0;
    bool duplicate = false;

    struct{
        char command[COMMAND_SIZE + 1];
        int counter;
        int timer;
    }counter[MAX_COMMANDS];

    struct{
        char command[COMMAND_SIZE + 1];
        int minutes;
    }estimates[MAX_COMMANDS];

    struct{
        char minute[3];
        char hour[3];
        char date[3];
        char month[3];
        char day[10];
        char command[COMMAND_SIZE + 1];
    }crontabs[MAX_COMMANDS];

    // Reading estimates file line by line and putting contents into array of structures
    while(fgets(line, sizeof line, estimates_file) != NULL){
        char estimate_name[COMMAND_SIZE + 1];
        int estimate_minutes = 0;

        int j = 0;
        while(isspace(line[j])) {  // Test for whitespace at start of sentence
            j++;
        }

        if(line[j] != '#') {
            sscanf(line, "%s %i", estimate_name, &estimate_minutes);
            strcpy(estimates[i].command, estimate_name);
            estimates[i].minutes = estimate_minutes;
            estimates_size++;
            i++;
        }
    }

    // Reading crontab file line by line and putting contents into array of structures
    i = 0;
    while(fgets(line, sizeof line, crontab_file) != NULL){
        char crontab_minute[3];
        char crontab_hour[3];
        char crontab_date[3];
        char crontab_month[3];
        char crontab_day[10];
        char crontab_command[COMMAND_SIZE + 1];

        int j = 0;
        while(isspace(line[j])) {  // Test for whitespace at start of sentence
                j++;
            }

        if(line[j] != '#') {
            sscanf(line, "%s %s %s %s %s %s", crontab_minute, crontab_hour,
                   crontab_date, crontab_month, crontab_day, crontab_command);
            strcpy(crontabs[i].minute, crontab_minute);
            strcpy(crontabs[i].hour, crontab_hour);
            strcpy(crontabs[i].date, crontab_date);
            strcpy(crontabs[i].month, crontab_month);
            strcpy(crontabs[i].day, crontab_day);
            strcpy(crontabs[i].command, crontab_command);
            crontabs_size++;
            i++;
            }
        }
    for(int z = 0; z < 5; z++) {
        printf("%s %s %s %s %s %s\n", crontabs[z].minute, crontabs[z].hour, crontabs[z].date, crontabs[z].month,
               crontabs[z].day, crontabs[z].command);
    }

    for(int j = 0; j < (days_in_month(month_int) * MINUTES_IN_DAY) + 1; j++){
        int current_day = (j / MINUTES_IN_DAY + first_day(month_int)) % 7;
        int current_hour = (j % MINUTES_IN_DAY) * HOURS_IN_DAY;
        for(int k = 0; k < sizeof counter / sizeof *counter; k++){
            if(counter[k].timer >= 0){
                --counter[k].timer;
            }
            if(counter[k].timer == 0){
                --nrunning;
            }
        }
        for(int k = 0; k < crontabs_size; k++){
            if(strcmp(crontabs[k].month, always) == 0 || month_num(crontabs[k].month) == month_int){
                if(strcmp(crontabs[k].day, always) == 0 || day_num(crontabs[k].day) == current_day){
                    if(strcmp(crontabs[k].hour, always) == 0 || atoi(crontabs[k].hour) == current_hour){
                        if(strcmp(crontabs[k].minute, always) == 0 || atoi(crontabs[k].minute) == j){
                            ++pid;
                            ++nrunning;
                            for(int l = 0; l < counter_size + 1; l++){
                                if(strcmp(counter[l].command, crontabs[k].command) == 0) {
                                    duplicate = true;
                                }
                            }
                            if(! duplicate){
                                strcpy(counter[counter_size].command, crontabs[k].command);
                                ++counter[counter_size].counter;
                                ++counter_size;
                            }
                            else{
                                ++counter[counter_size].counter;
                            }
                            for(int m = 0; m < estimates_size; m++){
                                if(strcmp(estimates[m].command, counter[counter_size].command) == 0) {
                                        counter[counter_size].timer = estimates[m].minutes;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

int main(int argc, char *argv[]){

    if(argc != 4){
        fprintf(stderr, "%s expected 3 arguments, instead got %i\n", argv[0], argc - 1);
    }
    // Defining variables
    char *month = argv[1];
    FILE *crontab_file = file_opener(argv[2]);
    FILE *estimates_file = file_opener(argv[3]);

    // Running program
    estimatecron(month, crontab_file, estimates_file);


    return 0;
}
