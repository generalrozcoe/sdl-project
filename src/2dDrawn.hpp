#ifndef Drawn_hpp
#define Drawn_hpp
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <utility>
#include <iostream>
#include <map>
#include <FreeImage.h>

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

class base2D
{
public:
    SDL_Rect body;

    int color[4];

    SDL_Rect viewport;
    virtual bool collision(base2D *other)
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
        SDL_Rect B = other->location();

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
    virtual void render() {};
    virtual SDL_Rect location() {};
};

class D2 : public base2D
{
public:
    D2()
    {
        body = {0, 0, 0, 0};
        color[0] = 0;
        color[1] = 0;
        color[2] = 0;
        color[3] = 0;
        viewport = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    };

    void render(SDL_Renderer *Renderer)
    {

        SDL_RenderSetViewport(Renderer, &viewport);
        SDL_SetRenderDrawColor(Renderer, color[0], color[1], color[2], color[3]);
        SDL_RenderFillRect(Renderer, &body);
    }
    SDL_Rect location()
    {
        return body;
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
    bool load(std::string path, SDL_Renderer *Renderer)
    {
        SDL_SetRenderDrawColor(Renderer, 0x00, 0x00, 0x00, 0x00);

        FREE_IMAGE_FORMAT filetype = FreeImage_GetFileType(path.data(), 0);
        FIBITMAP *freeimage_bitmap = FreeImage_Load(filetype, path.data(), 0);
        std::cout << filetype << std::endl;
        FreeImage_FlipVertical(freeimage_bitmap);
        std::cout << SDL_GetError() << std::endl;

        SDL_Surface *loadedSurface = SDL_CreateRGBSurfaceFrom(
            FreeImage_GetBits(freeimage_bitmap),
            FreeImage_GetWidth(freeimage_bitmap),
            FreeImage_GetHeight(freeimage_bitmap),
            FreeImage_GetBPP(freeimage_bitmap),
            FreeImage_GetPitch(freeimage_bitmap),
            FreeImage_GetRedMask(freeimage_bitmap),
            FreeImage_GetGreenMask(freeimage_bitmap),
            FreeImage_GetBlueMask(freeimage_bitmap),
            0);

        std::cout << SDL_GetError() << std::endl;

        media = SDL_CreateTextureFromSurface(Renderer, loadedSurface);

        std::cout << SDL_GetError() << std::endl;
        return true;
    }
    void render(SDL_Renderer *Renderer)
    {

        SDL_RenderCopy(Renderer, media, NULL, &viewport);
    }
    SDL_Rect location()
    {
        return viewport;
    }
};

class circle : public D2
{
private:
    int angleStart;
    int angleEnd;

public:
    std::pair<int, int> location;
    float size;
    float angles;
    std::map<float, std::pair<double, double>> edge;

    circle() {};

    void changeSize(float newSize)
    {
    }

    void angleCount()
    {
        float temp = (1 / log(size));
        angles = temp;
        return;
    }

    void drawEdges()
    {
        for (float i = 0; i < 360; i += angles)
        {
            double x = location.first + (cos(i * (M_PI / 180))) * size;
            double y = location.second + (sin(i * (M_PI / 180))) * size;
            std::pair<double, double> temp = {x, y};
            edge.insert({i, temp});
        }
    }

    void render(SDL_Renderer *Renderer)
    {
        angleCount();
        drawEdges();
        for (float i = 0; i < 360; i += angles)
        {
            SDL_RenderDrawPoint(Renderer, edge[i].first, edge[i].second);
        }
    }
};
// bool collision(base2D A, base2D B){};
// bool collision(base2D A, circle B ){};

/*bool collision(circle A, circle B){
    float distance = sqrt((( A.location.first - B.location.first)^2)+(A.location.second-B.location.second)^2);
    float sin = asin(( A.location.first - B.location.first)/distance);
    float cos = acos(( A.location.second - B.location.second)/distance);




}*/

class square : public D2
{
};
class shape : public D2
{
};

#endif