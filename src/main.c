#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <gtk/gtk.h>
#include "question.h"

// Define window dimensions
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

// Function to render text using SDL
void render_text(SDL_Renderer *renderer, const char *text, int x, int y, TTF_Font *font, SDL_Color color)
{
    // Create surface from text
    SDL_Surface *surface = TTF_RenderText_Blended(font, text, color);
    // Create texture from surface
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    // Define destination rectangle for the texture
    SDL_Rect dstrect = {x, y, surface->w, surface->h};
    // Copy texture to renderer
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);

    // Free resources
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

// Function to render a question and its answers
void render_question(SDL_Renderer *renderer, Question *question, TTF_Font *font)
{
    // Set background color to blue
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    // Clear the renderer
    SDL_RenderClear(renderer);

    // Define text color as white
    SDL_Color textColor = {255, 255, 255, 255};

    // Render the question text
    render_text(renderer, question->question_text, 50, 50, font, textColor);

    // Render each answer
    for (int i = 0; i < question->answer_count; i++)
    {
        render_text(renderer, question->answers[i], 50, 100 + i * 30, font, textColor);
    }

    // Present the renderer
    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[])
{
    // Initialize GTK
    gtk_init(&argc, &argv);

    // Initialize SDL video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "SDL initialization error: %s\n", SDL_GetError());
        return 1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() != 0)
    {
        fprintf(stderr, "SDL_ttf initialization error: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    // Load font
    TTF_Font *font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", 24);
    if (!font)
    {
        fprintf(stderr, "Font loading error: %s\n", TTF_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Create SDL window
    SDL_Window *window = SDL_CreateWindow("One-shot-Tests", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window)
    {
        fprintf(stderr, "SDL window creation error: %s\n", SDL_GetError());
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Create SDL renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        fprintf(stderr, "SDL renderer creation error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Define answers for the question
    const char *answers[] = {"Answer A", "Answer B", "Answer C", "Answer D"};
    // Create question structure
    Question *question = create_question("Example question?", answers, 1, 4);

    if (!question)
    {
        fprintf(stderr, "Question structure creation error.\n");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Main event loop
    int running = 1;
    SDL_Event event;
    while (running)
    {
        // Poll for events
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }
        }

        // Render the question and answers
        render_question(renderer, question, font);
        // Delay to control frame rate
        SDL_Delay(100);
    }

    // Free resources
    free_question(question);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
