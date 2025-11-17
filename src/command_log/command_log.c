#include <stdio.h>
#include "command_log/command_log.h"
#include "time.h"
#include "string.h"

void add_command(char *command) {
    time_t now=time(NULL);
    struct tm tm=*localtime(&now);
    char c[500];
    sprintf(c,"%02d-%02d-%04d %02d:%02d:%02d: ",tm.tm_mday,tm.tm_mon+1,tm.tm_year+1900,tm.tm_hour,tm.tm_min,tm.tm_sec);
    strcat(c,command);
    char f_name[100];
    sprintf(f_name,"log.txt");
    FILE *f=fopen(f_name,"a");
    fprintf(f,"%s\n",c);
    fclose(f);
}

void clear_logs() {
    char f_name[100];
    sprintf(f_name,"log.txt");
    FILE *f=fopen(f_name,"w");
    fclose(f);
}