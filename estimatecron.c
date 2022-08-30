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

#define LINE_SIZE 100
#define COMMAND_SIZE 40
#define MAX_COMMANDS 20
#define MINUTES_IN_DAY 1440

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
    const char months[12][9] = {"january", "february", "march", // all months as full name
                              "april", "may", "june",
                              "july", "august", "september",
                              "october", "november", "december"};
    const char short_months[12][3] = {"jan", "feb", "mar", // all months as 3 letter name
                                      "apr", "may", "jun",
                                      "jul", "aug", "sep",
                                      "oct", "nov", "dec"};

    for(int i = 0; month[i]; i++){
        month[i] = tolower(month[i]); // Converts each letter of given string to lowercase
    }
    for(int i = 0; i < 12; i++){
        if(! strcmp(month, months[i])){ // Compares whether given month is equal to its long month
            return i + 1;
        }
        else if(! strcmp(month, short_months[i])) { // Compares whether given month is equal to its short month
            return i + 1;
        }
    }
    fprintf(stderr, "%s is an invalid month\n", month);
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

int day_num(char *day){
    /* Returns the integer of a day given as a string */

    int day_num;
    if(! isnumber(atoi(day))){ // Return the day integer if it is already one
        return atoi(day);
    }
    const char days[7][9] = {"monday", "tuesday", "wednesday", // all days as full name
                                "thursday", "friday", "saturday","sunday"};
    const char short_days[7][3] = {"mon", "tue", "wed", // all days as 3 letter name
                                      "thu", "fri", "sat","sun"};

    for(int i = 0; day[i]; i++){
        day[i] = tolower(day[i]); // Converts each letter of given string to lowercase
    }
    for(int i = 0; i < 7; i++){
        if(! strcmp(day, days[i])){ // Compares whether given day is equal to its long day
            return i + 1;
        }
        else if(! strcmp(day, short_days[i])) { // Compares whether given day is equal to its short day
            return i + 1;
        }
    }
    fprintf(stderr, "%s is an invalid day\n", day);
    exit(EXIT_FAILURE);
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

    struct{
        char command[COMMAND_SIZE + 1];
        int minutes;
    }estimates[MAX_COMMANDS];

    struct{
        char minute[1];
        char hour[1];
        char date[1];
        char month[1];
        char day[9];
        char command[COMMAND_SIZE + 1];
    }crontabs[MAX_COMMANDS];

    // Reading estimates file line by line and putting contents into array of structures
    while(fgets(line, sizeof line, estimates_file) != NULL){
        char estimate_name[COMMAND_SIZE + 1];
        int estimate_minutes = 0;
        if(line[0] != '#') {
            sscanf(line, "%s %i", estimate_name, &estimate_minutes);
            strcpy(estimates[i].command, estimate_name);
            estimates[i].minutes = estimate_minutes;
            i++;
        }
    }

    // Reading crontab file line by line and putting contents into array of structures
    i = 0;
    while(fgets(line, sizeof line, crontab_file) != NULL){
        char crontab_minute[2];
        char crontab_hour[2];
        char crontab_date[2];
        char crontab_month[2];
        char crontab_day[2];
        char crontab_command[COMMAND_SIZE + 1];

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

    for(int k = 0; k < (days_in_month(month_int) * MINUTES_IN_DAY) + 1; k++){




    }

    for(int j = 0; j < 5; j++){
        printf("Estimates function %i is %s and will run for %i minutes\n", j, estimates[j].command, estimates[j].minutes);
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

    printf("%i", month_num("Jan"));

    // Running program
    // estimatecron(month, crontab_file, estimates_file);

    return 0;
}
