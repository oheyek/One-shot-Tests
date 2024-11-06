#ifndef QUESTION_H
#define QUESTION_H

// Define the maximum number of answers a question can have
#define MAX_ANSWERS 4

// Structure to represent a question
typedef struct
{
    char *question_text;        // The text of the question
    char *answers[MAX_ANSWERS]; // Array of possible answers
    int correct_answer_index;   // Index of the correct answer in the answers array
    int answer_count;           // Number of answers provided
} Question;

// Function to create a new question
// Parameters:
// - text: The text of the question
// - answers: Array of possible answers
// - correct_answer_index: Index of the correct answer in the answers array
// - answer_count: Number of answers provided
// Returns: Pointer to the newly created Question
Question *create_question(const char *text, const char *answers[], int correct_answer_index, int answer_count);

// Function to free the memory allocated for a question
// Parameters:
// - question: Pointer to the Question to be freed
void free_question(Question *question);

#endif
