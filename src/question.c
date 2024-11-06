#include <stdlib.h>
#include <string.h>
#include "question.h"

// Function to create a new question
// Parameters:
// - text: The text of the question
// - answers: Array of possible answers
// - correct_answer_index: Index of the correct answer in the answers array
// - answer_count: Number of answers provided
// Returns: Pointer to the newly created Question, or NULL if an error occurs
Question *create_question(const char *text, const char *answers[], int correct_answer_index, int answer_count)
{
    // Check if the number of answers exceeds the maximum allowed or if the correct answer index is invalid
    if (answer_count > MAX_ANSWERS || correct_answer_index >= answer_count)
    {
        return NULL;
    }

    // Allocate memory for the Question structure
    Question *question = malloc(sizeof(Question));
    if (!question)
    {
        return NULL;
    }

    // Duplicate the question text
    question->question_text = strdup(text);
    if (!question->question_text)
    {
        free(question);
        return NULL;
    }

    // Duplicate each answer text
    for (int i = 0; i < answer_count; i++)
    {
        question->answers[i] = strdup(answers[i]);
        if (!question->answers[i])
        {
            // Free previously allocated memory if duplication fails
            for (int j = 0; j < i; j++)
            {
                free(question->answers[j]);
            }
            free(question->question_text);
            free(question);
            return NULL;
        }
    }

    // Set the correct answer index and the number of answers
    question->correct_answer_index = correct_answer_index;
    question->answer_count = answer_count;

    return question;
}

// Function to free the memory allocated for a question
// Parameters:
// - question: Pointer to the Question to be freed
void free_question(Question *question)
{
    if (!question)
        return;

    // Free the question text
    free(question->question_text);
    // Free each answer text
    for (int i = 0; i < question->answer_count; i++)
    {
        free(question->answers[i]);
    }
    // Free the Question structure itself
    free(question);
}
