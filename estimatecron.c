//
// CITS2002 Project 1 2022
// Student1: 23057892 Chambers Monti
// Student2: 23058262 Nguyen Nathan



#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define CRONTAB_FILE
#define ESTIMATES_FILE
#define LINESIZE 100

void crontab_process(char filename[]){
    printf("howdy");
    //howdy
}

void estimates_process(char filename[]){
    FILE *estimates = fopen(filename, "r");
    char line[LINESIZE];

    if(estimates == NULL) {
        fprintf(stderr, "cannot open file '%s'\n", filename);
        exit(EXIT_FAILURE);
    }
    while(fgets(line, sizeof line, estimates) != NULL){

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


}