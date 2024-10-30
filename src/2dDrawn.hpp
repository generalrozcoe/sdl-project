#ifndef Drawn_hpp
#define Drawn_hpp
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <utility>
#include <iostream>
#include <map>
#include <vector>

#include <FreeImage.h>

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

class base2D
{
public:
    SDL_Rect body;

    int color[4];

    SDL_Rect viewport;
    virtual bool collision(base2D *other){
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
    virtual void render(SDL_Renderer *Render) {};
    virtual SDL_Rect location() {
    };
};

class D2 : public base2D
{
public:
typedef std::pair<int, int> location;

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

    void move(float x, float y)
    {
        body.x += x;
        body.y += y;
    }
    void set(float x, float y)
    {
        body.x = x;
        body.y = y;
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
        // std::cout << filetype << std::endl;
        FreeImage_FlipVertical(freeimage_bitmap);
        // std::cout << SDL_GetError() << std::endl;

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

        // std::cout << SDL_GetError() << std::endl;

        // media = SDL_CreateTextureFromSurface(Renderer, loadedSurface);

        // std::cout << SDL_GetError() << std::endl;
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
    void scale(float scale)
    {
        viewport.w *= scale;
        viewport.h *= scale;
    }
    void move(float x, float y)
    {
        viewport.x += x;
        viewport.y += y;
    }
    void set(float x, float y)
    {
        viewport.x = x;
        viewport.y = y;
    }
};

class circle : public D2
{
private:
    int angleStart;
    int angleEnd;

public:
    location location;
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


class square : public D2
{
};
class shape : public D2
{
public:
typedef std::pair<int, int> location;

    location origin;
    int size;
    int shapeType;
    std::vector<location> points;
    std::vector<location,location> lines;
    float offset;

    shape()
    {
        origin = {0, 0};
        size = 0;
        shapeType = 0;
        offset = 0;
    }
    void setAll(int locX, int locY, float off, int sides, int newSize)
    {
        origin.first = locX;
        origin.second = locY;
        offset = off;
        shapeType = sides;
        size = newSize;
        findPoints();
    }
    void shift(int X, int Y)
    {
        origin.first += X;
        origin.second += Y;
    }

    void changeOffset(int change)
    {
        offset += change;
    }
    void changeSize(int change)
    {
        size += change;
    }
    void changeShape(int change)
    {
        shapeType += change;
    }
    void findPoints()
    {
        points.clear();
        for (float i = 0; i < 360; i += (360 / shapeType))
        {
            double x = origin.first + (cos((i + offset) * (M_PI / 180))) * size;
            double y = origin.second + (sin((i + offset) * (M_PI / 180))) * size;
            std::pair<double, double> temp = {x, y};
            points.emplace_back(temp);
        }
    }
    void drawLines()
    {
        lines.clear();
        for (auto it = points.begin(); it != points.end(); ++it)
        {
            if ((it) == points.begin())
            {
                lines.emplace_back(points.back(), (*it));
            }
            else
            {
                lines.emplace_back((*(it - 1), (*it)));
            }
        }
    }
    void render(SDL_Renderer *Render){
        for (auto it = lines.begin(); it != lines.end(); ++it){


        SDL_RenderDrawLine(Render, (*it).first.first+origin.first, (*it).first.second+origin.second, (*it).second.first +origin.first, (*it).second.second.second+origin.second
        );
    }
    }
};

class text : public image
{
private:
public:
    static inline std::map<std::string, TTF_Font *> fonts;

    std::string string;
    SDL_Texture *media;
    TTF_Font *font;
    SDL_Color color;

    text()
    {

        color.r = 255;
        color.g = 255;
        color.b = 255;
        color.a = 255;
    }
    static void load(std::string location)
    {
        std::string name = location.substr(location.find("FONT-") + 5);
        TTF_Font *temp = TTF_OpenFont(location.data(), 24);
        fonts.emplace(name, temp);

        return;
    }

    void updateString(std::string newtext, SDL_Renderer *Renderer)
    {
        string = newtext;
        SDL_Surface *temp = TTF_RenderText_Solid(font, string.c_str(), color);
        media = SDL_CreateTextureFromSurface(Renderer, temp);
    }
    void render(SDL_Renderer *Renderer)
    {

        SDL_RenderCopy(Renderer, media, NULL, &viewport);
    }
    void setFont(std::string set)
    {
        font = fonts.at(set);
    }
    SDL_Rect location()
    {
        return viewport;
    }
};

#endif

