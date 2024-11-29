#include <gtk/gtk.h>
#include "question.h"
#include "config.h"

// Function prototypes
void load_next_question();
gboolean on_time_up(gpointer user_data);
gboolean update_timer_label(gpointer user_data);
void restart_quiz(GtkButton *button, gpointer user_data);
gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data);

// Widgets for displaying the question, answers, timer, and restart button
GtkWidget *question_label;
GtkWidget *answer_buttons[MAX_ANSWERS];
GtkWidget *timer_label;
GtkWidget *restart_button;
QuestionList *question_list;
int current_question = 0;
int score = 0; // Variable to store the user's score
int total_questions = 0; // Total number of questions
GtkApplication *app_ref; // Reference to the application object
guint current_timer_id = 0; // ID of the current timer

// Global variables for configuration
Config *config;

// Define the time limit for answering each question (in seconds)
int question_time_limit = 10;
int time_remaining = 10;
gboolean quiz_finished = FALSE; // Track if the quiz is finished

// Function to save the result to a CSV file
static void save_result_to_file(const char *name, int score, int total_questions) {
    FILE *file = fopen("results.csv", "a");
    if (file) {
        fprintf(file, "%s,%d,%d\n", name, score, total_questions);
        fclose(file);
    } else {
        g_print("Error: Unable to open results file\n");
    }
}

// Function to read the best score from the CSV file
static void get_best_score(char *best_name, int *best_score) {
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

// Function to handle time-out (when time for the current question is up)
gboolean on_time_up(gpointer user_data) {
    g_print("Time's up!\n");

    // Move to the next question
    current_question++;
    load_next_question();

    return G_SOURCE_REMOVE; // Stop the timer
}

// Function to update the timer label
gboolean update_timer_label(gpointer user_data) {
    if (time_remaining > 0) {
        char timer_text[32];
        snprintf(timer_text, sizeof(timer_text), "Time left: %d seconds", time_remaining);
        gtk_label_set_text(GTK_LABEL(timer_label), timer_text);

        time_remaining--;
        return G_SOURCE_CONTINUE; // Continue calling this function
    } else {
        on_time_up(NULL);
        return G_SOURCE_REMOVE; // Stop updating the timer
    }
}

// Function to restart the quiz
void restart_quiz(GtkButton *button, gpointer user_data) {
    // Reset quiz variables
    current_question = 0;
    score = 0;
    quiz_finished = FALSE; // Reset quiz finished flag

    // Shuffle the questions if the option is enabled
    if (config->shuffle_questions) {
        shuffle_questions(question_list);
    }

    // Reload the first question
    load_next_question();

    // Ensure the restart button is visible and disabled after restart
    gtk_widget_show(restart_button);
    gtk_widget_set_sensitive(restart_button, FALSE);

    // Show the answer buttons again
    for (int i = 0; i < MAX_ANSWERS; i++) {
        gtk_widget_show(answer_buttons[i]);
    }

    // Reset the timer for the new quiz start
    time_remaining = config->question_time_limit; // Reset time to the question time limit
    if (config->show_timer) {
        gtk_widget_show(timer_label); // Ensure the timer label is visible again
    } else {
        gtk_widget_hide(timer_label);
    }
}

// Function to load the next question
void load_next_question() {
    // Cancel any existing timer
    if (current_timer_id) {
        g_source_remove(current_timer_id);
        current_timer_id = 0;
    }

    if (current_question < question_list->size) {
        // Reset time for the next question
        time_remaining = config->question_time_limit;

        // Update the question label with the new question
        gtk_label_set_text(GTK_LABEL(question_label), question_list->questions[current_question]->question_text);
        for (int i = 0; i < MAX_ANSWERS; i++) {
            gtk_button_set_label(GTK_BUTTON(answer_buttons[i]),
                                 question_list->questions[current_question]->answers[i]);
        }

        // Start the timer for the new question
        if (config->show_timer) {
            current_timer_id = g_timeout_add_seconds(1, update_timer_label, NULL);
            gtk_widget_show(timer_label);
        } else {
            gtk_widget_hide(timer_label);
        }

        // Ensure the Restart button is visible but disabled during the quiz
        gtk_widget_show(restart_button);
        gtk_widget_set_sensitive(restart_button, FALSE); // Keep it disabled during the quiz
    } else {
        // End of the quiz - Display the final score
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
}

// Callback function for when an answer button is clicked
void on_answer_clicked(GtkButton *button, gpointer user_data) {
    int answer_index = GPOINTER_TO_INT(user_data);
    Question *q = question_list->questions[current_question];

    // Check if the selected answer is correct
    if (answer_index == q->correct_answer) {
        g_print("Correct!\n");
        score++; // Increment score for a correct answer
    } else {
        g_print("Wrong.\n");
    }

    // Move to the next question
    current_question++;
    load_next_question();
}

// Function to handle keyboard input
gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
    // Check which key was pressed (keys 1-4 for answers)
    if (event->keyval >= GDK_KEY_1 && event->keyval <= GDK_KEY_4) {
        int answer_index = event->keyval - GDK_KEY_1;
        Question *q = question_list->questions[current_question];

        // Check if the selected answer is correct
        if (answer_index < MAX_ANSWERS) {
            if (answer_index == q->correct_answer) {
                g_print("Correct!\n");
                score++; // Increment score for a correct answer
            } else {
                g_print("Wrong.\n");
            }

            // Move to the next question
            current_question++;
            load_next_question();
        }
    }

    // Check if the 'r' key is pressed after the quiz is finished
    if (event->keyval == GDK_KEY_r && quiz_finished) {
        restart_quiz(NULL, NULL); // Restart the quiz using the 'r' key
    }

    return FALSE; // Continue processing other events
}

// Function to handle the destroy signal
static void on_destroy(GtkWidget *widget, gpointer data) {
    g_application_quit(G_APPLICATION(app_ref));
}

// Function to set up the main application window and widgets
static void activate(GtkApplication *app, gpointer user_data) {
    app_ref = app; // Store the application reference for later use

    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "One-Shot Tests");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Create and add the question label
    question_label = gtk_label_new("Loading questions...");
    gtk_box_pack_start(GTK_BOX(vbox), question_label, TRUE, TRUE, 0);

    // Create and add the timer label
    timer_label = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(vbox), timer_label, TRUE, TRUE, 0);

    // Create a box to hold the answer buttons
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), button_box, TRUE, TRUE, 0);

    // Create and add the answer buttons
    for (int i = 0; i < MAX_ANSWERS; i++) {
        answer_buttons[i] = gtk_button_new();
        g_signal_connect(answer_buttons[i], "clicked", G_CALLBACK(on_answer_clicked), GINT_TO_POINTER(i));
        gtk_box_pack_start(GTK_BOX(button_box), answer_buttons[i], TRUE, TRUE, 0);
    }

    // Create and add the Restart Quiz button (initially hidden)
    restart_button = gtk_button_new_with_label("Restart Quiz");
    g_signal_connect(restart_button, "clicked", G_CALLBACK(restart_quiz), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), restart_button, TRUE, TRUE, 0);
    gtk_widget_hide(restart_button); // Hide it initially
    gtk_widget_set_sensitive(restart_button, FALSE); // Disable it initially

    // Connect the key press event to the window
    g_signal_connect(window, "key-press-event", G_CALLBACK(on_key_press), NULL);

    // Connect the destroy signal to the window
    g_signal_connect(window, "destroy", G_CALLBACK(on_destroy), NULL);

    // Load the questions from a file
    question_list = load_questions_from_file("questions.txt", config);
    if (!question_list) {
        return;
    }

    // Initialize the quiz
    total_questions = question_list->size;
    load_next_question();

    gtk_widget_show_all(window);
}

// Main function
int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("com.example.oneshottests", G_APPLICATION_DEFAULT_FLAGS);
    int status;

    // Load the configuration from a file
    config = load_config_from_file("config.txt");
    if (!config) {
        return 1;
    }

    // Set the question time limit from the configuration
    question_time_limit = config->question_time_limit;

    // Load the questions from a file
    question_list = load_questions_from_file("questions.txt", config);
    if (!question_list) {
        free_config(config);
        return 1;
    }

    // Connect the activate signal to the activate function
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);

    // Free the question list and config, and unref the application object
    free_question_list(question_list);
    free_config(config);
    g_object_unref(app);
    return status;
}