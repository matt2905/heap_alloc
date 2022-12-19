#include "time.h"
#include <stdio.h>
#include <stdlib.h>

void clean_log(void)
{
    FILE *log_file = fopen("log", "w");

    if (log_file == NULL)
    {
        fprintf(stderr, "logger can't open log file\n");
        exit(0);
    }
    fclose(log_file);
}

void write_log(char *action, size_t size, void *ptr)
{
    time_t timestamp = time(NULL);
    struct tm *time_infos = localtime(&timestamp);
    FILE *log_file = fopen("log", "a");

    if (log_file == NULL)
    {
        fprintf(stderr, "logger can't open log file\n");
        exit(0);
    }
    fprintf(log_file, "[%02d/%02d/%04d][%02d:%02d:%02d] %s %ld bytes at address %p\n",
            time_infos->tm_mday, time_infos->tm_mon + 1, time_infos->tm_year + 1900,
            time_infos->tm_hour, time_infos->tm_min, time_infos->tm_sec,
            action, size, ptr);
    fclose(log_file);
}

void read_log(void)
{
    FILE *log_file = fopen("log", "r");
    int ret = 1;
    size_t size;
    char adress[14];
    char trash[30];

    if (log_file == NULL)
    {
        fprintf(stderr, "logger can't open log file\n");
        exit(0);
    }
    while (ret != EOF)
    {
        ret = fscanf(log_file, "%s %s %lu %s %s %s %s\n",
                     trash, trash, &size,
                     trash, trash, trash, adress);
        if (ret != EOF)
            printf("%s %ld bytes\n", adress, size);
    }
    fclose(log_file);
}