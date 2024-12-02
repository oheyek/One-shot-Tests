#include <stdio.h>
#include <stdlib.h>
#include "config.h"

// Function to load configuration settings from a file
Config* load_config_from_file(const char *filename) {
    Config *config = malloc(sizeof(Config));
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to open config file %s\n", filename);
        free(config);
        return NULL;
    }

    // Read configuration settings from the file
    fscanf(file, "question_time_limit=%d\n", &config->question_time_limit);
    fscanf(file, "shuffle_questions=%d\n", &config->shuffle_questions);
    fscanf(file, "show_timer=%d\n", &config->show_timer);
    fscanf(file, "shuffle_answers=%d\n", &config->shuffle_answers);

    fclose(file);
    return config;
}

// Function to free the memory allocated for the configuration settings
void free_config(Config *config) {
    if (config) {
        free(config);
    }
}