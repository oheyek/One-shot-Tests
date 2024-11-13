#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <gtk/gtk.h>
#include "question.h"
#include <stdlib.h>

// Function to render text using SDL with trimming
void render_text(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y) {
    if (text == NULL) return;

    // Trim trailing newlines (in case there are any)
    char trimmed_text[256];
    snprintf(trimmed_text, sizeof(trimmed_text), "%s", text);
    size_t len = strlen(trimmed_text);
    if (len > 0 && (trimmed_text[len - 1] == '\n' || trimmed_text[len - 1] == '\r')) {
        trimmed_text[len - 1] = '\0';
    }

    SDL_Color color = {255, 255, 255}; // White color
    SDL_Surface *text_surface = TTF_RenderText_Blended(font, trimmed_text, color);
    if (!text_surface) {
        printf("Error creating surface: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    SDL_Rect text_rect = {x, y, text_surface->w, text_surface->h};
    SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);
}

int main(int argc, char *argv[]) {
    // Initialize GTK
    gtk_init(&argc, &argv);

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! TTF_Error: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    // Create SDL window
    SDL_Window *window = SDL_CreateWindow("One-shot-Tests", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600,
                                          SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Create SDL renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Load font
    TTF_Font *font = TTF_OpenFont("assets/font.ttf", 24);
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Create a QuestionList structure
    QuestionList *question_list = create_question_list(10);

    // Load questions from file
    int question_count = load_questions_from_file("questions.txt", question_list);

    if (question_count == -1) {
        printf("Failed to load questions.\n");
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        free_question_list(question_list);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    int quit = 0;
    SDL_Event e;

    // Main loop
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }

        // Clear screen with blue color
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderClear(renderer);

        // Render the first question and its answers
        if (question_list->size > 0) {
            render_text(renderer, font, question_list->questions[0]->question_text, 100, 100);
            for (int i = 0; i < MAX_ANSWERS; i++) {
                render_text(renderer, font, question_list->questions[0]->answers[i], 100, 150 + i * 30);
            }
        }

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Free allocated memory for questions
    free_question_list(question_list);

    // Clean up SDL and TTF resources
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
