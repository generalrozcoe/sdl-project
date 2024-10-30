#include <string>
#include <vector>
#include <utility>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "snake.cpp"

// Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);

// Starts up SDL and creates window
void init();

// Frees media and shuts down SDL
void close();

// The window we'll be rendering to
SDL_Window *gWindow = NULL;

// The surface contained by the window
SDL_Surface *gScreenSurface = NULL;

// Current displayed image
SDL_Surface *gCurrentSurface = NULL;
SDL_Texture *gTexture = NULL;
SDL_Renderer *gRenderer = NULL;

void init()
{
    bool success = true;

    SDL_Init(SDL_INIT_VIDEO);

    gWindow = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    TTF_Init();
}
void close()
{
    // Free loaded image
    SDL_DestroyTexture(gTexture);
    gTexture = NULL;

    // Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}
int main(int argc, char *args[])
{
    // Start up SDL and create window

    init();
    bool quit = false;

    SDL_Event e;
    snake game;
    // While application is running
    while (!quit)
    {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            // User requests quit
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            // User presses a key
        }

        game.run();
    }

    close();

    return 0;
}
