#ifndef CUSTOMFUNCTIONS_H
#define CUSTOMFUNCTIONS_H

//STD C++/C headers
#include <iostream>
#include <string.h>//memset and other memory functions
#include <stdlib.h>
//SDL C++/C headers
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


//The alphabet texture that should be usable everywhere in the program
static SDL_Texture *AlphabetTexture;
    //NOTE:this variable is only accessed inside this 'treanslation unit' (only inside these listed functions)
    //all other 'translation units' (files) will have a NULL as this variable I should make it static

//Loads the 'AlphabetTextrue'
int loadAlphabetTextrue(SDL_Renderer *TargetRenderer);

//Draws text in a simple way to this rectangle
int drawTextToRenderer_simple(SDL_Renderer *TargetRenderer,SDL_Rect dest,const char* text);
//Draws text in into the specified dest but with a fixed font
int drawTextToRenderer(SDL_Renderer *TargetRenderer,SDL_Rect dest,const char* text,int font_px);

//Returns a random int between the value of 'from' to 'to'
int getRandomINT(int from,int to);

//The struct that holds basic information about each units
struct BasicUnit_struct {

    //basic stats
    int max_HP;
    int max_Speed;
    int max_AttackRange;
};

//The array that holds all the different types of unit(note that 'BasicUnit[0]' is no unit but it means that there is no unit )
    //When making this 'BasicUnit' gets initialized 3 times from all includes
    //const  BasicUnit_struct BasicUnit[64] = { {0,-1,-1},{1,1,1} };
    static BasicUnit_struct BasicUnit[64] = {0};
//loads in the standard info about like 7 units (unit [0] is empty aka. will be an empty field in 'FieldMap')
    void loadBasicUnit_std(BasicUnit_struct trgt_arr[64]);




#endif // CUSTOMFUNCTIONS_H
