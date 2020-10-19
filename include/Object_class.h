#ifndef OBJECT_CLASS_H
#define OBJECT_CLASS_H

//std C++/C headers
#include <iostream>
#include <string.h>
//SDL headers
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

//!-----------------!//
using namespace std;
//!-----------------!//

//This class will be a class that other objects will inherit from

class Object_class
{
    public:
        Object_class();
        Object_class(SDL_Rect New_Position);
        Object_class(int new_x,int new_y,int new_w,int new_h);
        Object_class(SDL_Color New_ErrorColor);
        Object_class(SDL_Rect New_Position,SDL_Color New_ErrorColor);
        virtual ~Object_class();

        //Prints all the common info about the object ('Position,ErrorColor')
        void printInfo();//Prints all the common info about the object ('Position,ErrorColor')

        //Setter functions
        void setNew_Position(SDL_Rect New_Position);
        void setNew_ErrorColor(SDL_Color New_ErrorColor);
            ////'return 0;' on success 'return -1;' on error
        int setNew_ObjectTexture(const char *Path,SDL_Renderer *TargetRenderer);


        //Getter functions
        SDL_Rect getPosition();


        //Drawing functions
        //Just calls the 'SDL_RenderCopy()' function and returns it's return
            //NOTE: 'return 1' is used when the error color of the object was used instead of the 'ObjectTexture'
        int drawSelfToScreen_Simple(SDL_Renderer *TargetRenderer);


    protected:
        //The position of the object as a rectangle
        SDL_Rect Position;

        //The texture that will be drawn as this object
            //If it is 'NULL' then 'SDL_Color ErrorColor' will be drawn/painted instead
        SDL_Texture *ObjectTexture;

        //The color that will be displayed/painted onto the screen when this objects Texture can't be drawn
        SDL_Color ErrorColor;


    private:

};

#endif // OBJECT_CLASS_H
