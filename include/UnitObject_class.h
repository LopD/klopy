#ifndef UNITOBJECT_CLASS_H
#define UNITOBJECT_CLASS_H

//Base class
#include "Object_class.h"
#include "CustomFunctions.h"
//std C/C++ headers
#include <iostream>
//SDL headers
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>

//!--------------!//
using namespace std;
//!--------------!//


class UnitObject_class : public Object_class
{
    public:
        //CONSTRUCTORS AND DESTRUCTORS
            UnitObject_class();
            UnitObject_class(int new_UnitType);
            UnitObject_class(int new_HP,int new_UnitType);
            ~UnitObject_class();

        //GETTER FUNCTIONS
            int get_HP();
            int get_ArmyID();
            int get_UnitType();

        //SETTER FUNCTIONS
            int addHP(int added);
            void changeType(int New_UnitType);
            void setNew_ArmyID(int new_ArmyID);




    //!INHERITED
    //The position of the object as a rectangle
    //SDL_Rect Position;

    //The texture that will be drawn as this object
        //If it is 'NULL' then 'SDL_Color ErrorColor' will be drawn/painted instead
    //SDL_Texture *ObjectTexture;

    //The color that will be displayed/painted onto the screen when this objects Texture can't be drawn
    //SDL_Color ErrorColor;


    private:
        //Which player owns this unit
        int ArmyID;

        //The HP (hit/health points) of the unit
        int HP;

        //32bit Int that points to which type of unit this is (1,2,3,4...)
            //check BasicUnit[64] for all the units and their stats
        int UnitType;


};

#endif // UNITOBJECT_CLASS_H
