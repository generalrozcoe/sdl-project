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
typedef std::pair<float, float> location;
typedef std::pair<location, location> line;

class base
{
public:
    SDL_Color color;

    location origin;
    SDL_Rect viewport;
    std::map<int, location> points;
    std::map<int, std::pair<location, location>> lines;
    virtual void render()
    {
    }
    static bool collision(location x, location y, int sensitivity)
    {
        float a = y.first - x.first;
        float b = y.second - x.second;
        float c = std::sqrt((a * a) + (b * b));
        if (c < sensitivity)
        {
            return true;
        }
        return false;
    }

    void findPointsOnLine(location x, location y)
    {

        float tempX = x.first;
        std::cout << x.first << " " << x.second << std::endl;
        float tempY = x.second;
        std::cout << y.first << " " << y.second << std::endl;
        float a = x.first - y.first;
        float b = x.second - y.second;
        float c = std::sqrt((a * a) + (b * b));
        if (c == 0)
        {
            return;
        }

        float angleSin = asin((a / c));

        float angleCos = acos((b / c));

        for (int i = 0; i < c; i++)
        {

            tempX += cos(angleCos);

            tempY -= sin(angleSin);
            points.emplace(points.end(), std::make_pair(tempX, tempY));
        }
    }
    void findLines()
    {
        lines.clear();
        int i= 0;
        for (auto it = points.begin(); it != points.end(); ++it)
        {
            if ((it) == points.begin())
            { 
                lines.emplace(i++, std::make_pair(points.end(), (*it)));
                
            }
            else
            {   
                lines.emplace(i++, std::make_pair(*(--it), (*it)));
            }
        }
        for (auto it = lines.begin(); it != lines.end(); ++it)
        {
            findPointsOnLine((*it).second.first, (*it).second.second);
        }
    }
    static bool collision(base x, base y, int sensitivity, int tightness)
    {
        std::map<int, location> tempX;
        tempX = x.points;
        std::map<int, location> tempY;
        tempY = y.points;
        std::map<int, location> tempresult;


        std::set_intersection(tempX.begin(), tempX.end(), tempY.begin(), tempY.end(), back_inserter(tempresult));
        if (!tempresult.empty())
        {
            return true;
        }
        return false;
    }
};

class shape : public base
{
public:
    shape()
    {
        points.clear();
        lines.clear();
        shapeType = 0;
        size = 0;
        rotation = 0;
        viewport = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        color = {0, 0, 0, 0};
    }
    int shapeType;
    int size;
    float rotation;
    std::vector<location> collisionPoints()
    {
        std::vector<location> temp;
        for (auto it = points.begin(); it != points.end(); ++it)
        {
            temp.emplace_back(std::make_pair((*it).second.first + origin.first, (*it).second.second + origin.second));
        }
        return temp;
    };
    void findPoints()
    {
        points.clear();
        int point=0;
        for (float i = 0; i < 360; i += (360 / shapeType))
        {
            double x = origin.first + (cos((i + rotation) * (M_PI / 180))) * size;
            double y = origin.second + (sin((i + rotation) * (M_PI / 180))) * size;
            std::pair<double, double> temp = {x, y};
            points.emplace(point++,temp);
        }
    }

    void renderPoints(SDL_Renderer *Render)
    {
        for (auto it = points.begin(); it != points.end(); ++it)
        {
            SDL_RenderDrawPoint(Render, (*it).second.first + origin.first, (*it).second.second + origin.second);
        }
    }
    void renderLines(SDL_Renderer *Render)
    {
        SDL_SetRenderDrawColor(Render, color.r, color.g, color.b, color.a);

        for (auto it = lines.begin(); it != lines.end(); ++it)
        {
            SDL_RenderDrawLine(Render, (*it).second.first.first + origin.first, (*it).second.first.second + origin.second, (*it).second.second.first + origin.first, (*it).second.second.second + origin.second);
        }
    }
    void renderShaded(SDL_Renderer *Render) {}
};
class image : public base
{
public:
    SDL_Texture *media;

    image()
    {
        viewport = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        color = {0, 0, 0, 0};
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
        media = SDL_CreateTextureFromSurface(Renderer, loadedSurface);

        return true;
    }

    void render(SDL_Renderer *Renderer)
    {
        SDL_SetRenderDrawColor(Renderer, 0xFF, 0x00, 0x00, 0x00);

        for (auto it = points.begin(); it != points.end(); ++it)
        {
            SDL_RenderDrawPoint(Renderer, (*it).second.first + origin.first, (*it).second.second + origin.first);
        }
        // SDL_RenderCopy(Renderer, media, NULL, &viewport);
    }

    void findPoints()
    {
        int i = 0;
        points.emplace(i++,std::make_pair( viewport.x, viewport.y));
        points.emplace(i++,std::make_pair( viewport.x + viewport.w, viewport.y));
        points.emplace(i++,std::make_pair( viewport.x + viewport.w, viewport.y + viewport.h));
        points.emplace(i++,std::make_pair( viewport.x, viewport.y + viewport.h));
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
class text : public image
{
public:
    static inline std::map<std::string, TTF_Font *> fonts;
    std::string string;
    SDL_Texture *media;
    TTF_Font *font;
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