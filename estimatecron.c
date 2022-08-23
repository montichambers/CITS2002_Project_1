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

#define CRONTAB_FILE
#define ESTIMATES_FILE
#define LINESIZE 100

void file_opener(char filename[]) {
    FILE *name = fopen(filename, "r");
    char line[LINESIZE];

    if(name == NULL) {
        fprintf(stderr, "cannot open file '%s'\n", filename);
        exit(EXIT_FAILURE);
    } else {
        printf("Successfully opened %s", filename);
    }

    while(fgets(line, sizeof line, name) != NULL){
        if(line[0] != '#'){

        }
    }
    fclose(name);
}

void crontab_process(char filename[]){
    file_opener(filename);
}

void estimates_process(char filename[]){
    FILE *estimates = fopen(filename, "r");
    char line[LINESIZE];

    if(estimates == NULL) {
        fprintf(stderr, "cannot open file '%s'\n", filename);
        exit(EXIT_FAILURE);
    }
    while(fgets(line, sizeof line, estimates) != NULL){
        if(line[0] != '#'){

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

    crontab_process(crontab_file);

}