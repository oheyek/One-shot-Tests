#include "window.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to initialize SDL, create a window, renderer, and load the font
int init_SDL(SDLApp *app, const char *title, int width, int height) {
    // Initialize SDL video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    // Initialize SDL_ttf for font rendering
    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! TTF_Error: %s\n", TTF_GetError());
        SDL_Quit();
        return 0;
    }

    // Create the SDL window with given title, width, and height
    app->window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (app->window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 0;
    }

    // Create the SDL renderer for the window
    app->renderer = SDL_CreateRenderer(app->window, -1, SDL_RENDERER_ACCELERATED);
    if (app->renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(app->window);
        TTF_Quit();
        SDL_Quit();
        return 0;
    }

    // Load the font from the assets directory with a size of 24
    app->font = TTF_OpenFont("assets/font.ttf", 24);
    if (app->font == NULL) {
        printf("Failed to load font: %s\n", TTF_GetError());
        SDL_DestroyRenderer(app->renderer);
        SDL_DestroyWindow(app->window);
        TTF_Quit();
        SDL_Quit();
        return 0;
    }

    return 1; // Initialization successful
}

// Function to clear the screen with a specific RGBA color
void clear_screen(SDLApp *app, int r, int g, int b, int a) {
    SDL_SetRenderDrawColor(app->renderer, r, g, b, a);
    SDL_RenderClear(app->renderer);
}

// Function to render text at a specified position
void render_text(SDLApp *app, const char *text, int x, int y) {
    if (text == NULL) return; // Check for null text

    // Set text color (white)
    SDL_Color color = {255, 255, 255};

    // Create a surface from the text
    SDL_Surface *text_surface = TTF_RenderText_Blended(app->font, text, color);
    if (text_surface == NULL) return;

    // Create a texture from the surface
    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(app->renderer, text_surface);

    // Define the rectangle where the text will be drawn
    SDL_Rect text_rect = {x, y, text_surface->w, text_surface->h};

    // Render the texture to the screen
    SDL_RenderCopy(app->renderer, text_texture, NULL, &text_rect);

    // Clean up resources
    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);
}

// Function to clean up all SDL resources
void close_SDL(SDLApp *app) {
    // Close the font
    TTF_CloseFont(app->font);

    // Destroy the renderer and window
    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);

    // Quit SDL_ttf and SDL subsystems
    TTF_Quit();
    SDL_Quit();
}