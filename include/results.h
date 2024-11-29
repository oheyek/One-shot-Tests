#ifndef RESULTS_H
#define RESULTS_H

#include <gtk/gtk.h>

// Function prototypes for results module
void display_final_score();
void save_result_to_file(const char *name, int score, int total_questions);
void get_best_score(char *best_name, int *best_score);

#endif // RESULTS_H