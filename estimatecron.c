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
    FILE *estimates = file_opener(filename);
    char line[LINESIZE];

    struct{
        char command[FUNCSIZE + 1];
        int minutes;
    } estimate[MAXFUNCS];

    while(fgets(line, sizeof line, estimates) != NULL){
        if(line[0] != '#'){
            printf("%s\n", line);
            for(int i = 0; i < sizeof line; i++){
                //Split line into strings and assign each to struct
            }
        }
    }
    fclose(estimates);
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