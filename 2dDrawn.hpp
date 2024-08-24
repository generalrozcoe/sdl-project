#ifndef Drawn_hpp
#define Drawn_hpp

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <utility>
#include <iostream>
#include <map>
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

class D2
{
public:
    SDL_Rect body;

    int color[4];

    SDL_Rect viewport;

    SDL_Renderer *Renderer;
    D2()
    {
        body = {0, 0, 0, 0};
        color[0] = 0;
        color[1] = 0;
        color[2] = 0;
        color[3] = 0;
        viewport = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    };

    void render()
    {

        SDL_RenderSetViewport(Renderer, &viewport);
        SDL_SetRenderDrawColor(Renderer, color[0], color[1], color[2], color[3]);
        SDL_RenderFillRect(Renderer, &body);
    }
    SDL_Rect location()
    { return body;
    }
    bool collision(D2 object)
    {
        // The sides of the rectangles
        int leftA, leftB;
        int rightA, rightB;
        int topA, topB;
        int bottomA, bottomB;

        // Calculate the sides of rect A
        SDL_Rect A = location();
        leftA = A.x;
        rightA = A.x + A.w;
        topA = A.y;
        bottomA = A.y + A.h;

        // Calculate the sides of rect B
        SDL_Rect B = object.location();

        leftB = B.x;
        rightB = B.x + B.w;
        topB = B.y;
        bottomB = B.y + B.h;
        if (bottomA <= topB)
        {
            return false;
        }
        if (topA >= bottomB)
        {
            return false;
        }
        if (rightA <= leftB)
        {
            return false;
        }
        if (leftA >= rightB)
        {
            return false;
        }
        // If none of the sides from A are outside B
        return true;
    }
};

class image : public D2
{
public:
    SDL_Texture *media;

    image()
    {
        body = {0, 0, 0, 0};
        color[0] = 0;
        color[1] = 0;
        color[2] = 0;
        color[3] = 0;
        viewport = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    }
    bool load(std::string path)
    {
        SDL_Surface *loadedSurface = IMG_Load(path.c_str());
        media = SDL_CreateTextureFromSurface(Renderer, loadedSurface);
        if (media == NULL)
        {
            return false;
        }
        return true;
    }
    void render()
    {

        SDL_RenderSetViewport(Renderer, &viewport);
        SDL_RenderCopy(Renderer, media, NULL, NULL);
    }
        SDL_Rect location()
    { return viewport;
    }
};

#endif