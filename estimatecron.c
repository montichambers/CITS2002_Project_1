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
#define MINUTES_IN_HOUR 60
#define HOURS_IN_DAY 24

FILE *file_opener(char filename[]) {
    /* Returns the opened file pointer */

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

    const char *months[12] = {"january", "february", "march",
                              "april", "may", "june",
                              "july", "august", "september",
                              "october", "november", "december"};

    for(int i = 0; i < 12; i++){
        if(strstr(months[i], month) != NULL){ // Determines whether a given month is a valid month (or substring)
            return i;
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
    const char *days[7] = {"sunday", "monday", "tuesday", "wednesday",
                             "thursday", "friday", "saturday"};

    for(int i = 0; i < 7; i++){
        if(strstr(days[i], day) != NULL){ // Compares whether given day is a valid day (or substring)
            return i;
        }
    }
    fprintf(stderr, "%s is an invalid day\n", day);
    exit(EXIT_FAILURE);
}

int days_in_month(int month){
    /* Returns the amount of days in the month given as an integer [1-12] */

    int daysinmonth;
    switch (month) {
        case 0:
            daysinmonth = 31;
            break;
        case 1:
            daysinmonth = 28;
            break;
        case 2:
            daysinmonth = 31;
            break;
        case 3:
            daysinmonth = 30;
            break;
        case 4:
            daysinmonth = 31;
            break;
        case 5:
            daysinmonth = 30;
            break;
        case 6:
            daysinmonth = 31;
            break;
        case 7:
            daysinmonth = 31;
            break;
        case 8:
            daysinmonth = 30;
            break;
        case 9:
            daysinmonth = 31;
            break;
        case 10:
            daysinmonth = 30;
            break;
        case 11:
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
        case 0:
            first_day = 6;
            break;
        case 1:
            first_day = 2;
            break;
        case 2:
            first_day = 2;
            break;
        case 3:
            first_day = 5;
            break;
        case 4:
            first_day = 0;
            break;
        case 5:
            first_day = 3;
            break;
        case 6:
            first_day = 5;
            break;
        case 7:
            first_day = 1;
            break;
        case 8:
            first_day = 4;
            break;
        case 9:
            first_day = 6;
            break;
        case 10:
            first_day = 2;
            break;
        case 11:
            first_day = 4;
            break;
        default:
            fprintf(stderr, "%i is an invalid month", month);
            exit(EXIT_FAILURE);
    }
    return first_day;
}

struct Estimates{
    char command[COMMAND_SIZE + 1];
    int minutes;
};

struct Crontabs{
    char minute[3];
    char hour[3];
    char date[3];
    char month[3];
    char day[10];
    char command[COMMAND_SIZE + 1];
};

int read_crontabs(struct Crontabs crontabs[MAX_COMMANDS],  FILE *crontab_file){
    int i = 0;
    int j;
    int crontabs_size = 0;
    char line[LINE_SIZE];
    // Reading crontab file line by line and putting contents into array of structures
    while(fgets(line, sizeof line, crontab_file) != NULL){
        char crontab_minute[3];
        char crontab_hour[3];
        char crontab_date[3];
        char crontab_month[3];
        char crontab_day[10];
        char crontab_command[COMMAND_SIZE + 1];

        j = 0;
        while(isspace(line[j])) {  // Test for whitespace at start of sentence
            j++;
        }

        if(line[j] != '#') {
            sscanf(line, "%s %s %s %s %s %s", crontab_minute, crontab_hour,
                   crontab_date,crontab_month, crontab_day, crontab_command);
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
    return crontabs_size;
}

int read_estimates(struct Estimates estimates[MAX_COMMANDS], FILE *estimates_file){
    int i = 0;
    int j;
    int estimates_size = 0;
    char line[LINE_SIZE];
    // Reading estimates file line by line and putting contents into array of structures
    while(fgets(line, sizeof line, estimates_file) != NULL){
        char estimate_name[COMMAND_SIZE + 1];
        int estimate_minutes = 0;

        j = 0;
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
    return estimates_size;
}

bool is_current_time(struct Crontabs crontabs[MAX_COMMANDS], int minute, int current_month, int i){

    char *always = "*";
    int current_day = (minute / MINUTES_IN_DAY + first_day(current_month)) % 7;
    int current_date = (minute / MINUTES_IN_DAY) + 1;
    int current_hour = ((minute / MINUTES_IN_HOUR) % HOURS_IN_DAY);
    int current_minute = ((minute % MINUTES_IN_DAY) % MINUTES_IN_HOUR);
    bool monthflag = false;
    bool dayflag = false;
    bool dateflag = false;
    bool hourflag = false;
    bool minuteflag = false;

    if(strcmp(crontabs[i].month, always) == 0 || month_num(crontabs[i].month) == current_month) { // Test if the month is correct
        monthflag = true;
    }
    if(strcmp(crontabs[i].day, always) == 0 || day_num(crontabs[i].day) == current_day) { // Test if the day is correct
        dayflag = true;
    }
    if(strcmp(crontabs[i].date, always) == 0 || atoi(crontabs[i].date) == current_date) { // Test if the date is correct
        dateflag = true;
    }
    if(strcmp(crontabs[i].hour, always) == 0 || atoi(crontabs[i].hour) == current_hour) { // Test if the hour is correct
        hourflag = true;
    }
    if(strcmp(crontabs[i].minute, always) == 0 || atoi(crontabs[i].minute) == current_minute) { // Test if the minute is correct
        minuteflag = true;
    }
    if(monthflag && dayflag && dateflag && hourflag && minuteflag){
        return true;
    }else{
        return false;
    }
}

void error_checker(struct Estimates estimates[MAX_COMMANDS], struct Crontabs crontabs[MAX_COMMANDS],
        int crontabs_size, int estimates_size){

    int i;
    char *always = "*";
    //Error Checker for each line in crontabs file
    for(i = 0; i < crontabs_size; ++i) {
        //Check for correct minute domain
        if(strcmp(crontabs[i].minute, always) != 0) {
            if(atoi(crontabs[i].minute) < 0 || atoi(crontabs[i].minute) > 59) {
                printf("Min: %s, Hour: %s, Date: %s, Month: %s, Day: %s, Command: %s\n", crontabs[i].minute,
                       crontabs[i].hour, crontabs[i].date, crontabs[i].month, crontabs[i].day, crontabs[i].command);
                fprintf(stderr, "Minutes value %s is not in the required domain!!!\n", crontabs[i].minute);
                exit(EXIT_FAILURE);
            }
        }

        //Check for correct hour domain
        if(strcmp(crontabs[i].hour, always) != 0) {
            if(atoi(crontabs[i].hour) < 0 || atoi(crontabs[i].hour) > 23) {
                printf("Min: %s, Hour: %s, Date: %s, Month: %s, Day: %s, Command: %s\n", crontabs[i].minute,
                       crontabs[i].hour, crontabs[i].date, crontabs[i].month, crontabs[i].day, crontabs[i].command);
                fprintf(stderr, "Hour value %s is not in the required domain!!!\n", crontabs[i].hour);
                exit(EXIT_FAILURE);
            }
        }

        //Check for correct date domain
        if(strcmp(crontabs[i].date, always) != 0) {
            if(atoi(crontabs[i].date) < 1 || atoi(crontabs[i].date) > 31) {
                printf("Min: %s, Hour: %s, Date: %s, Month: %s, Day: %s, Command: %s\n", crontabs[i].minute,
                       crontabs[i].hour, crontabs[i].date, crontabs[i].month, crontabs[i].day, crontabs[i].command);
                fprintf(stderr, "Date value %s is not in the required domain!!!\n", crontabs[i].date);
                exit(EXIT_FAILURE);
            }
        }

        //Check for correct month domain
        if (strcmp(crontabs[i].month, always) != 0) {
            if (atoi(crontabs[i].month) < 0 || atoi(crontabs[i].month) > 11) {
                printf("Min: %s, Hour: %s, Date: %s, Month: %s, Day: %s, Command: %s\n", crontabs[i].minute,
                       crontabs[i].hour, crontabs[i].date, crontabs[i].month, crontabs[i].day, crontabs[i].command);
                fprintf(stderr, "Month value %s is not in the required domain!!!\n", crontabs[i].month);
                exit(EXIT_FAILURE);
            }
        }

        //Check for correct day domain
        if (strcmp(crontabs[i].day, always) != 0) {
            if (atoi(crontabs[i].day) < 0 || atoi(crontabs[i].day) > 59) {
                printf("Min: %s, Hour: %s, Date: %s, Month: %s, Day: %s, Command: %s\n", crontabs[i].minute,
                       crontabs[i].hour, crontabs[i].date, crontabs[i].month, crontabs[i].day, crontabs[i].command);
                fprintf(stderr, "Day value %s is not in the required domain!!!\n", crontabs[i].day);
                exit(EXIT_FAILURE);
            }
        }
    }

    //Error Checker for each line in estimates file
    //for (int k = 0; k < estimates_size; ++k) {
    //Check for correct minute input
    // printf("FINAL: %d\n", estimates[1].minutes);
    // }
}

void estimatecron(char *month, FILE *crontab_file, FILE *estimates_file){
    /* Calculates and prints:
     * 1. The name of the most frequently executed command
     * 2. The total number of commands invoked
     * 3. The maximum number of commands running at any time
     */

    // Defining variables
    int i;
    int j;
    int month_int = month_num(month);
    int pid = 0; // Total commands run in the month
    int nrunning = 0; // Number of current commands running at a particular time
    int max_nrunning = 0; // Highest value nrunning reaches
    int crontabs_size; // Size of crontabs array
    int estimates_size; // Size of estimates array
    int counter_size = 0; // Size of counter array
    int timer_size = 0; // Size of timer array

    struct{
        char command[COMMAND_SIZE + 1];
        int counter;
    }counter[MAX_COMMANDS];

    struct{
        char command[COMMAND_SIZE + 1];
        int timer;
    }timer[MAX_COMMANDS];

    struct Crontabs crontabs[MAX_COMMANDS];
    crontabs_size = read_crontabs(crontabs, crontab_file);

    struct Estimates estimates[MAX_COMMANDS];
    estimates_size = read_estimates(estimates, estimates_file);

    error_checker(estimates, crontabs, crontabs_size, estimates_size);

    for(i = 0; i < MAX_COMMANDS; i++){
        timer[i].timer = -1; // Initialise all timers to -1
        counter[i].counter = 0; // Initialise all counters to 0
    }
    for(int minute = 0; minute < (days_in_month(month_int) * MINUTES_IN_DAY) + 1; minute++){
        for(i = 0; i < timer_size + 1; i++){
            if(timer[i].timer >= 0){
                --timer[i].timer; // Decrement each timer
            }
            if(timer[i].timer == 0){
                --nrunning; // Decrement number of command running if its timer finishes
                printf("%s terminated at minute %i, pid = %i, nrunning = %i\n", crontabs[i].command, minute, pid, nrunning);
            }
        }
        timer_size = 0;
        for(i = 0; i < crontabs_size; i++){
            if(is_current_time(crontabs, minute, month_int, i)){
                ++pid; // Increment total commands run
                ++nrunning; // Increment current running commands
                printf("%s invoked at minute %i, pid = %i, nrunning = %i\n", crontabs[i].command, minute, pid, nrunning);
                if(nrunning > max_nrunning){
                    max_nrunning = nrunning;
                }
                bool duplicate = false; // Duplicate flag
                for (j = 0; j < counter_size + 1; j++) {
                    if (strcmp(counter[j].command, crontabs[i].command) == 0) { // Test if command is already in counter array
                        duplicate = true;
                    }
                }
                if (!duplicate) {
                    strcpy(counter[counter_size].command, crontabs[i].command); // Add command to counter if it's not a duplicate
                    ++counter[counter_size].counter;
                    ++counter_size;
                } else {
                    for (j = 0; j < counter_size + 1; j++) {
                        if (strcmp(counter[j].command, crontabs[i].command) == 0) {
                            ++counter[j].counter; // Increment the command counter if it is a duplicate
                        }
                    }
                }
                j = 0;
                while(timer[j].timer != -1){ // Find first free timer array
                    ++j;
                    ++timer_size;
                }
                strcpy(timer[j].command, crontabs[i].command); // If it is then store the command in array position
                for (int m = 0; m < estimates_size; m++) {
                    if(strcmp(estimates[m].command, timer[j].command) == 0) {
                        timer[j].timer = estimates[m].minutes; // Store commands estimated minute in timer
                    }
                }
            }
        }
    }
    int max_counter = 0;
    char max_counter_name[COMMAND_SIZE];
    for(i = 0; i < counter_size; i++){
        if(counter[i].counter > max_counter){
            max_counter = counter[i].counter;
        }
    }
    for(i = 0; i < counter_size; i++){
        if(counter[i].counter == max_counter){
            strcpy(max_counter_name, counter[i].command);
        }
    }
    printf("The command which ran the most times was %s\n", max_counter_name);
    printf("The total amount of commands run was %i\n", pid);
    printf("The total number of commands running at a single time was %i\n", max_nrunning);
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
