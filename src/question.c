#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "question.h"
#include "config.h"

// Function to create an empty question list
QuestionList* create_question_list() {
    QuestionList *list = malloc(sizeof(QuestionList));
    list->questions = NULL;
    list->size = 0;
    return list;
}

// Function to shuffle the questions in the list
void shuffle_questions(QuestionList *ql) {
    srand(time(NULL));
    for (int i = ql->size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Question *temp = ql->questions[i];
        ql->questions[i] = ql->questions[j];
        ql->questions[j] = temp;
    }
}

// Function to load questions from a file
QuestionList* load_questions_from_file(const char *filename, Config *config) {
    QuestionList *ql = create_question_list();
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        free(ql);
        return NULL;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character

        // Allocate memory for a new question
        Question *q = malloc(sizeof(Question));
        q->question_text = strdup(buffer);

        // Read the answers for the question
        for (int i = 0; i < MAX_ANSWERS; i++) {
            if (fgets(buffer, sizeof(buffer), file)) {
                buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character
                q->answers[i] = strdup(buffer);
            }
        }

        // Read the correct answer index
        fscanf(file, "%d\n", &(q->correct_answer));

        // Add the question to the question list
        ql->questions = realloc(ql->questions, sizeof(Question*) * (ql->size + 1));
        ql->questions[ql->size++] = q;
    }

    fclose(file);

    // Shuffle questions if the option is enabled
    if (config->shuffle_questions) {
        shuffle_questions(ql);
    }

    return ql;
}

// Function to free the memory allocated for the question list
void free_question_list(QuestionList *ql) {
    if (!ql) return;
    for (int i = 0; i < ql->size; i++) {
        free(ql->questions[i]->question_text);
        for (int j = 0; j < MAX_ANSWERS; j++) {
            free(ql->questions[i]->answers[j]);
        }
        free(ql->questions[i]);
    }
    free(ql->questions);
    free(ql);
}