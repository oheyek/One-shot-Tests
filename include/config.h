#ifndef CONFIG_H
#define CONFIG_H

// Structure to hold configuration settings
typedef struct {
    int question_time_limit; // Time limit for each question in seconds
    int shuffle_questions;   // Flag to indicate if questions should be shuffled (1 for true, 0 for false)
    int show_timer;          // Flag to indicate if a timer should be shown (1 for true, 0 for false)
    int shuffle_answers;     // Flag to indicate if answers should be shuffled (1 for true, 0 for false)
} Config;

// Function to load configuration settings from a file
Config* load_config_from_file(const char *filename);

// Function to free the memory allocated for the configuration settings
void free_config(Config *config);

#endif // CONFIG_H