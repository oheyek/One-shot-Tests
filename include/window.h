#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// Structure that holds SDL-related elements
typedef struct {
    SDL_Window *window;    // SDL window
    SDL_Renderer *renderer; // SDL renderer
    TTF_Font *font;        // Font used for rendering text
} SDLApp;

// Function prototypes

// Initialize SDL, window, renderer, and font
int init_SDL(SDLApp *app, const char *title, int width, int height);

// Clear the screen with a specified color
void clear_screen(SDLApp *app, int r, int g, int b, int a);

// Render text at the specified position
void render_text(SDLApp *app, const char *text, int x, int y);

// Clean up SDL resources
void close_SDL(SDLApp *app);

#endif // WINDOW_H
