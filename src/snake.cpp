#include <vector>
#include <map>
#include <list>

#include <functional>
#include <iostream>
#include <utility>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "shapes.hpp"

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
    location start = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
    std::pair<int, int> move = {1, 0};
    std::string points;
    shape player;
    image snack;
    std::pair<int, int> sunSize = {50, 50};
    int speed = 10;
    int size = 1;
    int lastTick = 0;
    //std::vector<shape> bones;
    text title;
    text score;

    shape test;

    snake()
    {
        // player creation
        player.origin = start;
        player.color.r = 255;
        player.color.b = 255;
        int i = 0;
        player.points.emplace(i++,std::make_pair(0, 0));
        player.points.emplace(i++,std::make_pair(0, 10));
        player.points.emplace(i++,std::make_pair(10, 10));
        player.points.emplace(i++,std::make_pair(10, 0));
        player.findLines();
        // objective creation
        snack.load("assets/diamond.png", gRenderer);
        sunLoc();
        snack.viewport.w = sunSize.first;
        snack.viewport.h = sunSize.second;
        snack.color.r = 255;

        snack.findPoints();
        snack.findLines();
        // text creation
        text::load("assets/FONT-pricedown.ttf");
        title.setFont("pricedown.ttf");
        title.updateString("SNAKE", gRenderer);
        title.scale(.125);
        title.set(SCREEN_WIDTH / 2 - (title.viewport.w / 2), 0 - (title.viewport.h / 4));
        score.setFont("pricedown.ttf");
        score.scale(.125);
        score.set(0, 0 - (title.viewport.h / 4));
        points = "Score: " + std::to_string(size - 1);

        // test

        test.origin = std::make_pair(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
        test.size = 50;
        test.shapeType = 6;
        test.findPoints();
    }

    void tail()
    {
        //bones.emplace_back(player);
        //if (bones.size() > size)
        {
        //    bones.erase(bones.begin());
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
                if (base::collision(player, snack, 20, 10))
                {
                    size += 1;
                    sunLoc();
                }

                /*     for (auto it = bones.begin(); it != bones.end() - 1; ++it)
                     {
                         if (base::collision(player, (*it), 1, 10))
                         {
                             gameOver();
                             break;
                         }
                     }*/
                points = "Score: " + std::to_string(size - 1);

                score.updateString(points, gRenderer);
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
    void render()
    {
        SDL_RenderClear(gRenderer);
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderFillRect(gRenderer, &background);
        player.renderLines(gRenderer);
        //for (auto it = bones.begin(); it != bones.end(); ++it)
        {
        //    (*it).renderLines(gRenderer);
        }
        title.render(gRenderer);
        snack.render(gRenderer);

        score.render(gRenderer);
        test.renderLines(gRenderer);

        SDL_RenderPresent(gRenderer);
        SDL_UpdateWindowSurface(gWindow);
    }
    void sunLoc()
    {
        do
        {
            snack.viewport.x = (SDL_GetTicks() % (SCREEN_WIDTH - 100));
            snack.viewport.y = (SDL_GetTicks() % (SCREEN_HEIGHT - 100));

        } while (tailCollision());
        return;
    }
    void go()
    {
        player.origin.first += move.first * speed;
        player.origin.second += move.second * speed;

        if (player.origin.first >= SCREEN_WIDTH)
        {
            player.origin.first -= SCREEN_WIDTH;
        }
        if (player.origin.first < 0)
        {
            player.origin.first += SCREEN_WIDTH;
        }
        if (player.origin.second >= SCREEN_HEIGHT)
        {
            player.origin.second -= SCREEN_HEIGHT;
        }
        if (player.origin.second < 0)
        {
            player.origin.second += SCREEN_HEIGHT;
        }
    }
    void gameOver()
    {
        size = 1;
        //bones.erase(bones.begin(), bones.end() - 1);
        sunLoc();
        player.origin.first = start.first;
        player.origin.second = start.second;
    }
    bool tailCollision()
    {
        //for (auto it = bones.begin(); it != bones.end(); ++it)
        {
           // if (base::collision((*it), snack, 1, 10))
            {
                return true;
            }
        }

        return false;
    };
};