#include <stdio.h>
#include "command_log/command_log.h"
#include "utils/utils.h"
#include "time.h"
#include "string.h"

void add_command(char *command) {
    time_t now=time(NULL);
    struct tm tm=*localtime(&now);
    char c[500];
    sprintf(c,"%02d-%02d-%04d %02d:%02d:%02d: ",tm.tm_mday,tm.tm_mon+1,tm.tm_year+1900,tm.tm_hour,tm.tm_min,tm.tm_sec);
    strcat(c,command);
    strcat(c,"\n");
    char f_name[100];
    sprintf(f_name,"%s/log.txt",BASE_DIR);
    FILE *f=fopen(f_name,"a");
    fprintf(f,"%s\n",c);
    fclose(f);
}