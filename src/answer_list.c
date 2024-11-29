#include "answer_list.h"
#include "question.h"

// External variables from main.c
extern GtkWidget *answer_buttons[MAX_ANSWERS];

// Function to create and add the answer buttons
void create_answer_buttons(GtkWidget *button_box) {
    for (int i = 0; i < MAX_ANSWERS; i++) {
        answer_buttons[i] = gtk_button_new();
        g_signal_connect(answer_buttons[i], "clicked", G_CALLBACK(on_answer_clicked), GINT_TO_POINTER(i));
        gtk_box_pack_start(GTK_BOX(button_box), answer_buttons[i], TRUE, TRUE, 0);
    }
}