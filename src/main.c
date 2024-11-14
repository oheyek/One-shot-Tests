#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <gtk/gtk.h>
#include "question.h"
#include "window.h"

// Main function of the application
int main(int argc, char *argv[]) {
    // Initialize GTK
    gtk_init(&argc, &argv);

    // Declare SDLApp structure to hold SDL-related components
    SDLApp app;

    // Initialize SDL with window title and dimensions
    if (!init_SDL(&app, "One-shot-Tests", 800, 600)) {
        return 1; // Exit if initialization fails
    }

    // Create a QuestionList and load questions from file
    QuestionList *question_list = create_question_list(10);
    int question_count = load_questions_from_file("questions.txt", question_list);

    // Check if questions were loaded successfully
    if (question_count == -1) {
        printf("Failed to load questions.\n");
        close_SDL(&app);
        free_question_list(question_list);
        return 1;
    }

    int quit = 0;
    SDL_Event e;

    // Main event loop
    while (!quit) {
        // Poll events
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1; // Exit the loop if the window is closed
            }
        }

        // Clear the screen with a blue color
        clear_screen(&app, 0, 0, 255, 255);

        // Render the first question and its answers
        if (question_list->size > 0) {
            render_text(&app, question_list->questions[0]->question_text, 100, 100);
            for (int i = 0; i < MAX_ANSWERS; i++) {
                render_text(&app, question_list->questions[0]->answers[i], 100, 150 + i * 30);
            }
        }

        // Present the rendered content to the screen
        SDL_RenderPresent(app.renderer);
    }

    // Clean up memory and SDL resources
    free_question_list(question_list);
    close_SDL(&app);

    return 0;
}
