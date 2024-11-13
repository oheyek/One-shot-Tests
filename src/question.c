#include "question.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

    // Duplicate the answers
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

// Function to free memory for a question
void free_question(Question *q) {
    if (q != NULL) {
        free(q->question_text);
        for (int i = 0; i < MAX_ANSWERS; i++) {
            free(q->answers[i]);
        }
        free(q);
    }
}

// Function to validate correct answer index
int is_valid_answer_index(const char *line) {
    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] < '0' || line[i] > '9') return 0;
    }
    int index = atoi(line);
    return (index >= 0 && index < MAX_ANSWERS);
}

// Function to load questions from a file with validation
int load_questions_from_file(const char *filename, Question *questions[], int max_questions) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open file %s\n", filename);
        return -1;
    }

    int count = 0;
    char buffer[256];
    while (count < max_questions) {
        // Read the question text
        if (fgets(buffer, sizeof(buffer), file) == NULL) break;
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strlen(buffer) == 0) {
            printf("Error: Missing question text for question %d\n", count + 1);
            continue;
        }
        char *question_text = strdup(buffer);

        // Read answers
        char *answers[MAX_ANSWERS];
        for (int i = 0; i < MAX_ANSWERS; i++) {
            if (fgets(buffer, sizeof(buffer), file) == NULL) {
                printf("Error: Missing answer %d for question %d\n", i + 1, count + 1);
                free(question_text);
                return -1;
            }
            buffer[strcspn(buffer, "\n")] = '\0';
            if (strlen(buffer) == 0) {
                printf("Error: Empty answer %d for question %d\n", i + 1, count + 1);
                free(question_text);
                return -1;
            }
            answers[i] = strdup(buffer);
        }

        // Read the correct answer index
        if (fgets(buffer, sizeof(buffer), file) == NULL) {
            printf("Error: Missing correct answer index for question %d\n", count + 1);
            free(question_text);
            return -1;
        }
        buffer[strcspn(buffer, "\n")] = '\0';
        if (!is_valid_answer_index(buffer)) {
            printf("Error: Invalid correct answer index for question %d\n", count + 1);
            free(question_text);
            return -1;
        }
        int correct_answer = atoi(buffer);

        // Create question and add to the list
        questions[count] = create_question(question_text, answers, correct_answer);
        if (questions[count] == NULL) {
            printf("Error: Could not create question %d\n", count + 1);
            free(question_text);
            return -1;
        }

        count++;
    }

    fclose(file);
    return count;
}