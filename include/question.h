#ifndef QUESTION_H
#define QUESTION_H

#define MAX_ANSWERS 4

// Structure to represent a question
typedef struct Question {
    char *question_text; // The text of the question
    char *answers[MAX_ANSWERS]; // Array of possible answers
    int correct_answer; // Index of the correct answer
} Question;

// Structure to represent a list of questions
typedef struct QuestionList {
    Question **questions; // Array of pointers to questions
    int size; // Current number of questions
    int max_size; // Maximum number of questions
} QuestionList;

// Function prototypes
Question* create_question(const char *text, char *answers[], int correct_answer);
void free_question(Question *q);
int load_questions_from_file(const char *filename, QuestionList *ql);
void trim_newline(char *str);
QuestionList* create_question_list(int max_questions);
void free_question_list(QuestionList *ql);

#endif // QUESTION_H