#ifndef QUESTION_H
#define QUESTION_H

#define MAX_ANSWERS 4

// Structure to represent a question
typedef struct Question {
    char *question_text; // The text of the question
    char *answers[MAX_ANSWERS]; // Array of possible answers
    int correct_answer; // Index of the correct answer
} Question;

// Function prototypes
Question* create_question(const char *text, char *answers[], int correct_answer);
void free_question(Question *q);
int load_questions_from_file(const char *filename, Question *questions[], int max_questions);

#endif // QUESTION_H