#include "results.h"
#include "question.h"

// External variables from main.c
extern GtkWidget *question_label;
extern GtkWidget *timer_label;
extern GtkWidget *answer_buttons[MAX_ANSWERS];
extern GtkWidget *restart_button;
extern int score;
extern int total_questions;
extern gboolean quiz_finished;

// Function to save the result to a CSV file
void save_result_to_file(const char *name, int score, int total_questions) {
    FILE *file = fopen("results.csv", "a");
    if (file) {
        fprintf(file, "%s,%d,%d\n", name, score, total_questions);
        fclose(file);
    } else {
        g_print("Error: Unable to open results file\n");
    }
}

// Function to read the best score from the CSV file
void get_best_score(char *best_name, int *best_score) {
    FILE *file = fopen("results.csv", "r");
    if (!file) {
        *best_score = -1;
        return;
    }

    char line[256];
    *best_score = -1;
    while (fgets(line, sizeof(line), file)) {
        char name[100];
        int score, total;
        sscanf(line, "%99[^,],%d,%d", name, &score, &total);
        if (score > *best_score) {
            *best_score = score;
            strcpy(best_name, name);
        }
    }
    fclose(file);
}

// Function to display the final score
void display_final_score() {
    char result[256];
    snprintf(result, sizeof(result), "Quiz finished!\nYour score: %d/%d", score, total_questions);
    gtk_label_set_text(GTK_LABEL(question_label), result);

    // Hide the timer label
    gtk_widget_hide(timer_label);

    // Hide the answer buttons after the quiz ends
    for (int i = 0; i < MAX_ANSWERS; i++) {
        gtk_widget_hide(answer_buttons[i]);
    }

    g_print("%s\n", result);

    // Enable the Restart button only after the quiz finishes
    quiz_finished = TRUE; // Set the flag to true when the quiz is finished
    gtk_widget_set_sensitive(restart_button, TRUE); // Enable restart button after the quiz ends

    // Prompt for the user's name and save the result
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Enter Your Name",
                                                    GTK_WINDOW(gtk_widget_get_toplevel(question_label)),
                                                    GTK_DIALOG_MODAL,
                                                    "_OK", GTK_RESPONSE_OK,
                                                    NULL);
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(content_area), entry, TRUE, TRUE, 0);
    gtk_widget_show_all(dialog);

    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_OK) {
        const char *name = gtk_entry_get_text(GTK_ENTRY(entry));
        save_result_to_file(name, score, total_questions);

        // Get the best score and name
        char best_name[100];
        int best_score;
        get_best_score(best_name, &best_score);

        // Display the best score and name if available
        if (best_score != -1) {
            char best_result[256];
            snprintf(best_result, sizeof(best_result), "\nBest score: %d by %s", best_score, best_name);
            gtk_label_set_text(GTK_LABEL(question_label), g_strconcat(result, best_result, NULL));
        }
    }
    gtk_widget_destroy(dialog);
}