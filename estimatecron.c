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


 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

#define LINESIZE 100
#define FUNCSIZE 40
#define MAXFUNCS 20

FILE *file_opener(char filename[]) {
    FILE *name = fopen(filename, "r");

    if(name == NULL) {
        fprintf(stderr, "cannot open file '%s'\n", filename);
        exit(EXIT_FAILURE);
    }

    return name;
}

int month_num(char *month){
    int month_num;
    if(isnumber(atoi(month))){
        return atoi(month);
    }
    const char months[12][9] = {"january", "february", "march",
                              "april", "may", "june",
                              "july", "august", "september",
                              "october", "november", "december"};
    const char short_months[12][3] = {"jan", "feb", "mar",
                                      "apr", "may", "jun",
                                      "jul", "aug", "sep",
                                      "oct", "nov", "dec"};
    for(int i = 0; month[i]; i++){
        month[i] = tolower(month[i]);
    }
    for(int i = 0; i < 12; i++){
        if(! strcmp(month, months[i])){
            return i + 1;
        }
        else if(! strcmp(month, short_months[i])) {
            return i + 1;
        }
        else{
            fprintf(stderr, "%s is an invalid month", month);
            exit(EXIT_FAILURE);
        }
    }
}

int days_in_month(int month){
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
            fprintf(stderr, "%i is an invalid month", month);
            exit(EXIT_FAILURE);
    }
    return daysinmonth;
}

void estimatecron(int month, FILE *crontab_file, FILE *estimates_file){
    int i = 0;
    char line[LINESIZE];

    struct{
        char command[FUNCSIZE + 1];
        int minutes;
    }estimates[MAXFUNCS];

    while(fgets(line, sizeof line, estimates_file) != NULL){
        char estimate_name[FUNCSIZE + 1];
        int estimate_minutes = 0;
        if(line[0] != '#') {
            sscanf(line, "%s %i", estimate_name, &estimate_minutes);
            strcpy(estimates[i].command, estimate_name);
            estimates[i].minutes = estimate_minutes;
            i++;
        }
    }

    struct{
        char minute[2];
        char hour[2];
        char date[2];
        char month[2];
        char day[2];
        char command[FUNCSIZE + 1];
    }crontabs[MAXFUNCS];

    i = 0;
    while(fgets(line, sizeof line, crontab_file) != NULL){
        char crontab_minute[2];
        char crontab_hour[2];
        char crontab_date[2];
        char crontab_month[2];
        char crontab_day[2];
        char crontab_command[FUNCSIZE + 1];

        if(line[0] != '#') {
            sscanf(line, "%s %s %s %s %s %s", crontab_minute, crontab_hour,
                   crontab_date, crontab_month, crontab_day, crontab_command);
            strcpy(crontabs[i].minute, crontab_minute);
            strcpy(crontabs[i].hour, crontab_hour);
            strcpy(crontabs[i].date, crontab_date);
            strcpy(crontabs[i].month, crontab_month);
            strcpy(crontabs[i].day, crontab_day);
            strcpy(crontabs[i].command, crontab_command);
            i++;
        }
    }

    for(int j = 0; j < 5; j++){
        printf("Estimates function %i is %s and will run for %i minutes\n", j, estimates[j].command, estimates[j].minutes);
    }
}

int main(int argc, char *argv[]){
    if(argc != 4){
        fprintf(stderr, "%s expected 3 arguments, instead got %i", argv[0], argc - 1);
    }
    char *month = argv[1];
    FILE *crontab_file = file_opener(argv[2]);
    FILE *estimates_file = file_opener(argv[3]);

    estimatecron(month, crontab_file, estimates_file);

    return 0;
}
