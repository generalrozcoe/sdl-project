#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <iostream>

class menu{
    int cursorX = 0;
    int cursorY = 0;





public:
menu(){

};

void left(){
    cursorX = cursorX-1;
    std::cout <<"left"<<std::endl;
}
void right(){
    cursorX = cursorX+1;
}
void up(){
    cursorY = cursorY-1;
}
void down(){
    cursorY = cursorY+1;
}
};