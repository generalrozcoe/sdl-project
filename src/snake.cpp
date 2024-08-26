
#include <vector>
#include <map>
#include <functional>
#include <iostream>
#include <utility>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "2dDrawn.hpp"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const Uint8 *currentKeyStates;
extern SDL_Renderer *gRenderer;
extern SDL_Window *gWindow;
extern const Uint8 *currentKeyStates;

class snake
{
public:
    SDL_Rect background = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    std::pair<int, int> start = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
    std::pair<int, int> snakeSize = {10, 10};
    std::pair<int, int> move = {1, 0};
    std::map<int, D2> bones;
    std::map<int, D2> ligaments;
    D2 player;
    image snack;
    std::pair<int, int> sunSize = {50, 50};
    int speed = 10;
    int size = 0;
    int lastTick = 0;
    snake()
    {
        player.body.x = start.first;
        player.body.y = start.second;
        player.body.w = snakeSize.first;
        player.body.h = snakeSize.second;

        player.color[0] = 255;

        player.color[2] = 255;
        if (!snack.load("assets/diamond.png", gRenderer))
        {
            std::cout << "stop" << std::endl;
        };
        sunLoc();

        snack.viewport.w = sunSize.first;
        snack.viewport.h = sunSize.second;

        snack.color[0] = 255;
    }
    void makeTail()
    {
    }
    void run()
    {
        if (lastTick < SDL_GetTicks())
        {
            if (SDL_GetTicks() % 32 == 0)
            {
                if (currentKeyStates[SDL_SCANCODE_UP])
                {

                    up();
                }
                else if (currentKeyStates[SDL_SCANCODE_DOWN])
                {

                    down();
                }
                else if (currentKeyStates[SDL_SCANCODE_LEFT])
                {

                    left();
                }
                else if (currentKeyStates[SDL_SCANCODE_RIGHT])
                {
                    right();
                }
                render();
                go();
                std::cout << player.collision(&snack) << std::endl;
                if (snack.collision(&player))
                {
                    size += speed;
                    sunLoc();
                }

                makeTail();

                for (auto it = bones.begin(); it != bones.end(); ++it)
                {
                    if ((*it).second.collision(&player))
                    {
                        gameOver();
                    }
                }
            }
        }
        lastTick = SDL_GetTicks();
    }
    void render()
    {
        SDL_RenderClear(gRenderer);

        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderFillRect(gRenderer, &background);

        player.render(gRenderer);
        snack.render(gRenderer);

        for (auto it = bones.begin(); it != bones.end(); ++it)
        {
            (*it).second.render(gRenderer);
        }
        SDL_RenderPresent(gRenderer);
        SDL_UpdateWindowSurface(gWindow);
    }

    void sunLoc()
    {
        snack.viewport.x = (SDL_GetTicks() % (SCREEN_WIDTH - 100));
        snack.viewport.y = (SDL_GetTicks() % (SCREEN_HEIGHT - 100));
        do
        {
            snack.viewport.x = (SDL_GetTicks() % (SCREEN_WIDTH - 100));
            snack.viewport.y = (SDL_GetTicks() % (SCREEN_HEIGHT - 100));

        } while (tailCollision());
        return;
    }
    void up()
    {
        if (move != std::make_pair(0, 1))
        {
            move = {0, -1};
        }
    }
    void down()
    {
        if (move != std::make_pair(0, -1))
        {

            move = {0, 1};
        }
    }
    void left()
    {
        if (move != std::make_pair(1, 0))
        {

            move = {-1, 00};
        }
    }
    void right()
    {
        if (move != std::make_pair(-1, 0))
        {

            move = {1, 00};
        }
    }
    void go()
    {
        player.body.x += move.first * speed;
        player.body.y += move.second * speed;

        if (player.body.x >= SCREEN_WIDTH)
        {
            player.body.x -= SCREEN_WIDTH;
        }
        if (player.body.x < 0)
        {
            player.body.x += SCREEN_WIDTH;
        }
        if (player.body.y >= SCREEN_HEIGHT)
        {
            player.body.y -= SCREEN_HEIGHT;
        }
        if (player.body.y < 0)
        {
            player.body.y += SCREEN_HEIGHT;
        }
    }
    void gameOver()
    {
    }

    bool tailCollision()
    {
        for (auto it = bones.begin(); it != bones.end(); ++it)
        {
            if ((*it).second.collision(&snack))
            {
                return true;
            }
        }

        return false;
    }
};