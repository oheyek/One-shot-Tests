#ifndef QUESTION_H
#define QUESTION_H

// Define the maximum number of answers for each question
#define MAX_ANSWERS 4

// Structure to represent a single question
typedef struct {
    char *question_text; // The text of the question
    char *answers[MAX_ANSWERS]; // Array of answer options
    int correct_answer; // Index of the correct answer in the answers array
} Question;

// Structure to represent a list of questions
typedef struct {
    Question **questions; // Array of pointers to questions
    int size; // Number of questions in the list
} QuestionList;

// Function to create an empty question list
QuestionList* create_question_list();

// Function to load questions from a file
QuestionList* load_questions_from_file(const char *filename);

// Function to free the memory allocated for the question list
void free_question_list(QuestionList *list);

#endif // QUESTION_H