
#include <vector>
#include <map>
#include <list>

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
    D2 player;
    image snack;
    std::pair<int, int> sunSize = {50, 50};
    int speed = 10;
    int size = 1;
    int lastTick = 0;
    std::vector<D2> bones;

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

    void tail()
    {
        bones.emplace_back(player);
        if (bones.size() > size)
        {
            bones.erase(bones.begin());
        }
    }
    void run()
    {
        if (lastTick < SDL_GetTicks())
        {
            if (SDL_GetTicks() % 32 == 0)
            {
                render();
                movement();
                go();
                tail();
                if (player.collision(&snack))
                {
                    size += 1;
                    sunLoc();
                }

                for (auto it = bones.begin(); it != bones.end() - 1; ++it)
                {
                    if ((*it).collision(&player))
                    {
                        gameOver();
                        break;
                    }
                }
            }
        }
        lastTick = SDL_GetTicks();
    }
    void movement()
    {
        if (currentKeyStates[SDL_SCANCODE_UP])
        {

            if (move != std::make_pair(0, 1))
            {
                move = {0, -1};
            }
        }
        else if (currentKeyStates[SDL_SCANCODE_DOWN])
        {

            if (move != std::make_pair(0, -1))
            {

                move = {0, 1};
            }
        }
        else if (currentKeyStates[SDL_SCANCODE_LEFT])
        {

            if (move != std::make_pair(1, 0))
            {

                move = {-1, 00};
            }
        }
        else if (currentKeyStates[SDL_SCANCODE_RIGHT])
        {
            if (move != std::make_pair(-1, 0))
            {

                move = {1, 00};
            }
        }
    }
    void render(){
        SDL_RenderClear(gRenderer);
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderFillRect(gRenderer, &background);
        player.render(gRenderer);
        snack.render(gRenderer);
        for (auto it = bones.begin(); it != bones.end(); ++it){
            (*it).render(gRenderer);
        }
        SDL_RenderPresent(gRenderer);
        SDL_UpdateWindowSurface(gWindow);
    }
    void sunLoc(){
        do{
            snack.viewport.x = (SDL_GetTicks() % (SCREEN_WIDTH - 100));
            snack.viewport.y = (SDL_GetTicks() % (SCREEN_HEIGHT - 100));

        } while (tailCollision());
        return;
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
        size = 1;
        bones.erase(bones.begin(), bones.end() - 1);
        sunLoc();
        player.body.x = SCREEN_WIDTH / 2;
        player.body.y = SCREEN_HEIGHT / 2;
    }
    bool tailCollision()
    {
        for (auto it = bones.begin(); it != bones.end(); ++it)
        {
            if ((*it).collision(&snack))
            {
                return true;
            }
        }

        return false;
    }
};