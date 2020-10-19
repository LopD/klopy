#ifndef BUTTONOBJECT_CLASS_H
#define BUTTONOBJECT_CLASS_H

//Base class/classes
#include "Object_class.h"
//STD C++/C headers
#include <iostream>
#include <string.h>
#include <string>
#include <stdlib.h>
//SDL headers
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


class ButtonObject_Class : public Object_class
{
    public:
        ButtonObject_Class();
        ButtonObject_Class(SDL_Rect New_Position);
        ButtonObject_Class(SDL_Rect New_Position,SDL_Color New_ErrorColor);
        virtual ~ButtonObject_Class();

        //Setter functions
        int setNew_ButtonText(const char* NewText);


        //Getter functions
        std::string  getButtonText();


        //Drawing functions
        //draws the ObjectTexture only or the error color
            //NOTE: 'return 1' is used when the error color of the object was used instead of the 'ObjectTexture'
        int drawSelfToScreen_Simple(SDL_Renderer *TargetRenderer);

        //Handles the OnClick Event
        int handleEvent_OnClick(SDL_Event &ClickEvent);
        //!FUNCTION POINTER=>WHAT FUNCTION TO CALL WHEN IT'S CLICKED ON
        void* (*doOnClickFunction) ( void*param );



        SDL_Rect* getpPosition();//Returns the pointer to the 'Position' variable
        SDL_Rect getPosition();//Returns just a copy of the 'Position' variable


    //!inherited:
        //The position of the object as a rectangle
        //SDL_Rect Position;

        //The texture that will be drawn as this object
            //If it is 'NULL' then 'SDL_Color ErrorColor' will be drawn/painted instead
        //SDL_Texture *ObjectTexture;

        //The color that will be displayed/painted onto the screen when this objects Texture can't be drawn
        //SDL_Color ErrorColor;


    private:
        std::string ButtonText;
};

#endif // BUTTONOBJECT_CLASS_H
