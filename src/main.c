#include <SDL2/SDL.h>
#include <gtk/gtk.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    // Initialize GTK with command line arguments
    gtk_init(&argc, &argv);

    // Initialize SDL with video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "SDL initialization error: %s\n", SDL_GetError());
        return 1; // Return with error code
    }

    // Print success message if initialization is successful
    printf("Initialization completed successfully.\n");

    // Clean up and quit SDL
    SDL_Quit();
    return 0;
}
