#include <gtk/gtk.h>
#include "question.h"

// Widgets for displaying the question and answer buttons
static GtkWidget *question_label;
static GtkWidget *answer_buttons[MAX_ANSWERS];
static QuestionList *question_list;
static int current_question = 0;
static GtkApplication *app_ref; // Reference to the application object

// Helper function to set button labels with key number
static void set_button_label_with_key(GtkButton *button, const char *text, int key_number) {
    char label_with_key[256];
    snprintf(label_with_key, sizeof(label_with_key), "%d: %s", key_number + 1, text);
    gtk_button_set_label(button, label_with_key);
}

// Function to load the next question
static void load_next_question() {
    if (current_question < question_list->size) {
        // Update the question label with the new question
        gtk_label_set_text(GTK_LABEL(question_label), question_list->questions[current_question]->question_text);
        for (int i = 0; i < MAX_ANSWERS; i++) {
            set_button_label_with_key(GTK_BUTTON(answer_buttons[i]),
                                      question_list->questions[current_question]->answers[i], i);
        }
    } else {
        // End of the quiz
        g_print("Quiz finished!\n");
        g_application_quit(G_APPLICATION(app_ref)); // Correctly quit the application
    }
}

// Callback function for when an answer button is clicked
static void on_answer_clicked(GtkButton *button, gpointer user_data) {
    int answer_index = GPOINTER_TO_INT(user_data);
    Question *q = question_list->questions[current_question];

    // Check if the selected answer is correct
    if (answer_index == q->correct_answer) {
        g_print("Correct!\n");
    } else {
        g_print("Wrong.\n");
    }

    // Move to the next question
    current_question++;
    load_next_question();
}

// Function to handle keyboard input
static gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
    // Check which key was pressed (keys 1-4)
    if (event->keyval >= GDK_KEY_1 && event->keyval <= GDK_KEY_4) {
        int answer_index = event->keyval - GDK_KEY_1;
        Question *q = question_list->questions[current_question];

        // Check if the selected answer is correct
        if (answer_index < MAX_ANSWERS) {
            if (answer_index == q->correct_answer) {
                g_print("Correct!\n");
            } else {
                g_print("Wrong.\n");
            }

            // Move to the next question
            current_question++;
            load_next_question();
        }
    }
    return FALSE; // Continue processing other events
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
    question_label = gtk_label_new(question_list->questions[current_question]->question_text);
    gtk_box_pack_start(GTK_BOX(vbox), question_label, TRUE, TRUE, 0);

    // Create a box to hold the answer buttons
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), button_box, TRUE, TRUE, 0);

    // Create and add the answer buttons
    for (int i = 0; i < MAX_ANSWERS; i++) {
        answer_buttons[i] = gtk_button_new();
        set_button_label_with_key(GTK_BUTTON(answer_buttons[i]), question_list->questions[current_question]->answers[i],
                                  i);
        g_signal_connect(answer_buttons[i], "clicked", G_CALLBACK(on_answer_clicked), GINT_TO_POINTER(i));
        gtk_box_pack_start(GTK_BOX(button_box), answer_buttons[i], TRUE, TRUE, 0);
    }

    // Connect the key press event to the window
    g_signal_connect(window, "key-press-event", G_CALLBACK(on_key_press), NULL);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("com.example.oneshottests", G_APPLICATION_DEFAULT_FLAGS);
    int status;

    // Load the questions from a file
    question_list = load_questions_from_file("questions.txt");
    if (!question_list) {
        return 1;
    }

    // Connect the activate signal to the activate function
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);

    // Free the question list and unref the application object
    free_question_list(question_list);
    g_object_unref(app);
    return status;
}
