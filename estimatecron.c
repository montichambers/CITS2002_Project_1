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
    Turn files into structures

 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

#define CRONTAB_FILE
#define ESTIMATES_FILE
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

void crontab_process(char filename[]){
    file_opener(filename);
}

void estimates_process(char filename[]){
    FILE *estimates_file = file_opener(filename); //Open file
    char line[LINESIZE];

    while(fgets(line, sizeof line, estimates_file) != NULL){
        char command[FUNCSIZE + 1];
        int minutes;
        if(line[0] != '#'){
            sscanf(line, "%s %i", command, &minutes);
            printf("Line is %s %i\n", command, minutes);
        }
    }
    fclose(estimates_file);
}

void calculations(){


}

int main(int argc, char *argv[]){
    if(argc != 4){
        fprintf(stderr, "%s expected 3 arguments, instead got %i", argv[0], argc - 1);
    }
    char *month = argv[1];
    char *crontab_file = argv[2];
    char *estimates_file = argv[3];

    estimates_process(estimates_file);

    crontab_process(crontab_file);

    return 0;
}


//int count = 0;
//char *token = strtok(line, " ");
/*
while(token != NULL){
    if(count == 0){
        strcpy(*estimates[i].command, token);
    }
    if(count == 1){
        estimates[i].minutes = atoi(token);
    }
    */
//count += 1;
//token = strtok(NULL, " ");