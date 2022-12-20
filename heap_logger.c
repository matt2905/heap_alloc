#include "time.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Function to emptying the log file
 *
 */
void clean_log(void)
{
    FILE *log_file = fopen("log", "w");

    if (log_file == NULL)
    {
        fprintf(stderr, "clean logger can't open log file\n");
        exit(0);
    }
    fclose(log_file);
}

/**
 * @brief Function that log the different event of heap_alloc
 *
 * @param action
 * @param size
 * @param ptr
 */
void write_log(char *action, size_t size, void *ptr)
{
    time_t timestamp = time(NULL);
    struct tm *time_infos = localtime(&timestamp);
    FILE *log_file = fopen("log", "a");
    char time_buffer[80];

    if (log_file == NULL)
    {
        fprintf(stderr, "write logger can't open log file\n");
        exit(0);
    }
    strftime(time_buffer, 80, "[%B %d %Y][%H:%M:%S]", time_infos);
    fprintf(log_file, "%s %s %ld bytes at address %p\n", time_buffer, action, size, ptr);
    fclose(log_file);
}

/**
 * @brief Function to extract, the most important info from the log
 *
 */
void read_log(void)
{
    FILE *log_file = fopen("log", "r");
    int ret = 1;
    size_t size;
    char action[10];
    char adress[14];

    if (log_file == NULL)
    {
        fprintf(stderr, "read logger can't open log file\n");
        exit(0);
    }
    while (ret != EOF)
    {
        ret = fscanf(log_file, "%*s %*s %*s %s %lu bytes at address %s\n",
                     action, &size, adress);
        if (ret != EOF && strcmp(action, "malloced") == 0)
            printf("%s %ld bytes\n", adress, size);
    }
    fclose(log_file);
}