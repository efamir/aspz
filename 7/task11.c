#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h> // sleep()
#include <ctype.h>  // isspace()

#define MAX_TASKS 50
#define CRON_FILE "mycron.tab"
#define COMMAND_LEN 256
#define LINE_BUF_SIZE 512

typedef struct {
    int interval_seconds;
    char command[COMMAND_LEN];
    time_t last_run_time;
} SimpleCronTask;

SimpleCronTask tasks[MAX_TASKS];
int num_tasks = 0;

void load_simple_cron_tasks() {
    FILE *fp = fopen(CRON_FILE, "r");
    if (fp == NULL) {
        printf("Warning: Cron file %s not found or not readable. No tasks loaded.\n", CRON_FILE);
        printf("Create %s with entries like: <seconds_interval> <command>\n", CRON_FILE);
        return;
    }

    char line_buf[LINE_BUF_SIZE];
    time_t current_load_time = time(NULL);

    while (fgets(line_buf, sizeof(line_buf), fp) != NULL && num_tasks < MAX_TASKS) {
        char *trimmed_line = line_buf;
        while(isspace((unsigned char)*trimmed_line)) trimmed_line++; // Trim

        if (trimmed_line[0] == '#' || trimmed_line[0] == '\0') { // Skip comments and empty lines
            continue;
        }

        // Trim trailing newline/whitespace
        char *end = trimmed_line + strlen(trimmed_line) - 1;
        while(end > trimmed_line && isspace((unsigned char)*end)) end--;
        *(end + 1) = 0;

        SimpleCronTask current_task;
        int interval_val;
        char *command_start_ptr;

        if (sscanf(trimmed_line, "%d", &interval_val) == 1) {
            command_start_ptr = trimmed_line;
            // Move pointer past the interval number
            while (*command_start_ptr && (isdigit((unsigned char)*command_start_ptr) || *command_start_ptr == '-')) command_start_ptr++;
            // Move pointer past any spaces after the number
            while (*command_start_ptr && isspace((unsigned char)*command_start_ptr)) command_start_ptr++;

            if (*command_start_ptr == '\0') {
                fprintf(stderr, "Skipping malformed line (no command after interval): %s\n", trimmed_line);
                continue;
            }

            strncpy(current_task.command, command_start_ptr, COMMAND_LEN - 1);
            current_task.command[COMMAND_LEN - 1] = '\0';
            current_task.interval_seconds = interval_val;

            if (current_task.interval_seconds <= 0) {
                fprintf(stderr, "Skipping task with non-positive interval: %s\n", trimmed_line);
                continue;
            }
            // Initialize last_run_time to allow task to run "interval seconds ago" from load time
            current_task.last_run_time = current_load_time - current_task.interval_seconds;
            tasks[num_tasks++] = current_task;
        } else {
            fprintf(stderr, "Skipping malformed line (cannot read interval): %s\n", trimmed_line);
        }
    }
    fclose(fp);
    printf("Loaded %d tasks from %s.\n", num_tasks, CRON_FILE);
}

int main() {
    load_simple_cron_tasks();

    if (num_tasks == 0) {
        printf("No tasks to run. Exiting.\n");
        return EXIT_SUCCESS;
    }

    while (1) {
        time_t current_time = time(NULL);

        for (int i = 0; i < num_tasks; ++i) {
            if (current_time >= (tasks[i].last_run_time + tasks[i].interval_seconds)) {
                printf("Running task: %s (interval %d s)\n",
                       tasks[i].command, tasks[i].interval_seconds);
                fflush(stdout);

                system(tasks[i].command); // Return value could be checked for errors

                tasks[i].last_run_time = current_time;
            }
        }
        sleep(1);
    }

    return EXIT_SUCCESS;
}
