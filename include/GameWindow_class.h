#ifndef GAMEWINDOW_CLASS_H
#define GAMEWINDOW_CLASS_H

//STD C++/C HEADERS
#include <iostream>
#include <cstdlib>
#include <string.h>
#include <vector>
#include <dirent.h>
#include <stdlib.h>
//My classes
#include "Object_class.h"
#include "ButtonObject_Class.h"
#include "CustomFunctions.h"
#include "UnitObject_class.h"
//SDL HEADERS
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

//!!!!!!!!!!!!!!!!!//
using namespace std;
//!!!!!!!!!!!!!!!!!//

class GameWindow_class
{
    public:
        GameWindow_class();
        ~GameWindow_class();


        //PLAYER CAMERA: (moving around the field)
            //The rectangle that the player will see on the screen
            SDL_Rect PlayerCamera;
            //The amount of pixels that the Players camera will move each time it has to
            int PlayerCamera_OffsetChange;

            //Change the PlayerCamera values
            void setNew_PlayerCamera(SDL_Rect new_PlayerCamera);
            void setNew_PlayerCamera(int x,int y,int w,int h);
            //return the address of the 'PlayerCamera' object
            SDL_Rect* get_PlayerCamera();




        struct Field_struct {
            //Holds a status of the field
            int status;
            UnitObject_class *PlacedUnit;
        };

        //FIELDMAP VARIABLE
            //2D array that holds all the info about each field
            Field_struct FieldMap[500][500];
            //The width and height of FieldMap in units
            int FieldMap_w,FieldMap_h;
            //Field width and height in pixels
            int Field_w,Field_h;
            //Self explanatory (this is the width and height of the 2D array that holds the info about the field)
            void setNew_FieldMap_w(int new_FieldMap_w);
            void setNew_FieldMap_h(int new_FieldMap_h);
            //handle the left mouse button click on a field in field map
            int handle_LMBClickOnFieldMap();
            //handle keyboard presses on the field map
            int handle_keyboardOnFieldMap(SDL_Event &event);
            //handles unit movement
            int movePlacedUnit(int fromFieldMap_xpoz,int fromFieldMap_ypoz,int toFieldMap_xpoz,int toFieldMap_ypoz);





        //GAME FUNCTIONS
            //LOAD FUNCTIONS
                //Loads a random game
                int loadRandomGame();
                //loads a new game with the w and h specified
                int loadNewGame(int new_FieldMap_w,int new_FieldMap_h);
                //same as 'loadNewGame' except that this here takes the 'FieldMap_w' and 'FieldMap_h' that was set and uses them
                int loadSetNewGame(GameWindow_class *trgt);

            //DRAWING AND HIGHLIGHTING
                //Draws the FieldMap
                int drawFieldMap();
                //Highlight this field with this color
                int highlightField(int Field_xpoz,int Field_ypoz,SDL_Color Color);
                //Highlights all the fields around the highlighted units position
                    //it takes the 'max_speed' from 'BasicUnit[64]' and highlights all the fields within that radius
                int highlightFieldsAroundHighlightedField(SDL_Color Color);





        //Main Menu functions
            //Holds all the Buttons of the main menu
            std::vector<ButtonObject_Class> MainMenuButton; //Actually holds 'ButtonObject_class' objects

            //LOADING FUNCTIONS
                //Loads the main menu and allocates all the buttons into 'MainMenuButton' (clearing it beforehand)
                int loadMainMenu();

            //DRAWING
                //Draws the MainMenu but doesn't present it (also clears the renderer duh)
                int drawMainMenu();

            //EVENT HANDLING
                //Handles the main menu mouse events
                int handleMainMenuMouseEvent(SDL_Event &Event);


        int testFunction() {
            std::cout << "Hello!"<<std::endl;
            return 0;
        }





        //SETTER functions
            //Calls 'SDL_CreateWindow()' on win and returns the 'return' of the mentioned function
            int setNew_win(const char* title,int x,int y,int w,int h,unsigned int flags);
            //Calls 'GameWindow_class()' on rendy,returns 0 on success and -1 on error
            int setNew_rendy(int index,unsigned long int flags);
            //Returns 0 on success and -1 on fail
            int setNew_BackgroundTexture(const char *path);
            //sets 'DrawnMainMenu' to the 'to'
            void set_DrawnMainMenu(bool to);
            //Calls 'SDL_SetRenderDrawColor()' and returns the 'return' of the mentioned function
            int setRenderDrawColor(unsigned char r,unsigned char g,unsigned char b,unsigned char a);
            //self explanatory
            void setNew_HighlightedField_xpoz(int new_HighlightedField_xpoz);
            void setNew_HighlightedField_ypoz(int new_HighlightedField_ypoz);
            //Self explanatory (this is the width and height of each 'FieldMap' field that will drawn in PIXELS)
            void setNew_Field_w(int new_Field_w);
            void setNew_Field_h(int new_Field_h);




        //Calls 'SDL_RenderPresent()'
        void renderPresent();
        //Calls 'SDL_RenderClear()'
        void renderClear();




        //GETTER functions
            //Returns the renderer apointed to this object
            SDL_Renderer *getRendererPointer();
            int get_WindowWidth();
            int get_WindowHeight();
            //Return true or false depending on the 'DrawnMainMenu' variable
            bool getDrawnMainMenu();
            int get_HighlightedField_xpoz();
            int get_HighlightedField_ypoz();


    private:
        //The w and h of the window
        int WindowWidth,WindowHeight;

        //The position of the highlighted field
        int HighlightedField_xpoz,HighlightedField_ypoz;

        //self explanatory
        SDL_Window *win;
        SDL_Renderer *rendy;

        //The background texture
        SDL_Texture *BackgroundTexture;

        //Is the main menu drawn,If the player is on the main menu
        bool DrawnMainMenu;

};

#endif // GAMEWINDOW_CLASS_H
