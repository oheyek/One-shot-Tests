#include "question.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Function to create a list of questions
QuestionList* create_question_list(int max_questions) {
    QuestionList *ql = (QuestionList*)malloc(sizeof(QuestionList));
    if (ql == NULL) {
        return NULL;
    }
    ql->questions = (Question**)malloc(max_questions * sizeof(Question*));
    if (ql->questions == NULL) {
        free(ql);
        return NULL;
    }
    ql->size = 0;
    ql->max_size = max_questions;
    return ql;
}

// Function to free the memory allocated for a list of questions
void free_question_list(QuestionList *ql) {
    if (ql != NULL) {
        for (int i = 0; i < ql->size; i++) {
            free_question(ql->questions[i]);
        }
        free(ql->questions);
        free(ql);
    }
}

// Function to trim newline characters from a string
void trim_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[len - 1] = '\0';
    }
}

// Function to create a question
Question* create_question(const char *text, char *answers[], int correct_answer) {
    // Allocate memory for the question
    Question *q = (Question*)malloc(sizeof(Question));
    if (q == NULL) {
        return NULL;
    }
    // Duplicate the question text
    q->question_text = strdup(text);
    if (q->question_text == NULL) {
        free(q);
        return NULL;
    }
    // Duplicate each answer
    for (int i = 0; i < MAX_ANSWERS; i++) {
        q->answers[i] = strdup(answers[i]);
        if (q->answers[i] == NULL) {
            free_question(q);
            return NULL;
        }
    }
    // Set the correct answer index
    q->correct_answer = correct_answer;
    return q;
}

// Function to free the memory allocated for a question
void free_question(Question *q) {
    if (q != NULL) {
        free(q->question_text);
        for (int i = 0; i < MAX_ANSWERS; i++) {
            free(q->answers[i]);
        }
        free(q);
    }
}

// Function to load questions from a file
int load_questions_from_file(const char *filename, QuestionList *ql) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open file %s\n", filename);
        return -1;
    }

    int count = 0;
    char buffer[256];
    while (count < ql->max_size && fgets(buffer, sizeof(buffer), file)) {
        trim_newline(buffer); // Remove newline characters
        char *question_text = strdup(buffer);

        char *answers[MAX_ANSWERS];
        for (int i = 0; i < MAX_ANSWERS; i++) {
            if (fgets(buffer, sizeof(buffer), file) == NULL) {
                printf("Error: Not enough answers for question %d\n", count + 1);
                fclose(file);
                return -1;
            }
            trim_newline(buffer); // Remove newline characters
            answers[i] = strdup(buffer);
        }

        if (fgets(buffer, sizeof(buffer), file) == NULL) {
            printf("Error: Missing correct answer index for question %d\n", count + 1);
            fclose(file);
            return -1;
        }
        trim_newline(buffer); // Remove newline characters
        int correct_answer = atoi(buffer);

        ql->questions[count] = create_question(question_text, answers, correct_answer);
        if (ql->questions[count] == NULL) {
            printf("Error: Could not create question %d\n", count + 1);
            fclose(file);
            return -1;
        }
        count++;
    }

    fclose(file);
    ql->size = count;
    return count;
}