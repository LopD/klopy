//STD C++/C headers
#include <iostream>
//SDL C++/C headers
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//My classes
#include "CustomFunctions.h" //holds a few helping functions
#include "GameWindow_class.h" //holds the renderer,resolution,window*
#include "Object_class.h"
#include "ButtonObject_Class.h"

//!!!!!!!!!!!!!!!!!!//
//using namespace std;
//!!!!!!!!!!!!!!!!!!//

//Holds the renderer and window pointer of the main game window
GameWindow_class MainGameWindow;

//Immediately quit the game (Used inside the main loop)
bool QuitGame = false;



int main (int argc,char** argv) {

    //////////////////////////////////////////////
    //      SETTING UP SDL AND SDL_Image        //
    //////////////////////////////////////////////

    //init SDL2.0
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cerr << "Failed to init \'SDL_INIT_EVERYTHING\'!"<<std::endl;
        return 0;
    }

    //!init png-s in this project/program
    IMG_Init(IMG_INIT_PNG);



    //////////////////////////////////////////////
    //      SETTING UP THE WINDOW OBJECT        //
    //////////////////////////////////////////////

    //Create the window and renderer
    MainGameWindow.setNew_win("CMD&CONQ",0,10,800,600,0);
    MainGameWindow.setNew_rendy(1,0);
    MainGameWindow.renderClear();
    MainGameWindow.setRenderDrawColor(0,0,0,0);
    MainGameWindow.setNew_BackgroundTexture("./graphics/MainMenuBackground_default.png");

    //Present it now so that the user knows that it didn't crash immediately :)
    MainGameWindow.renderPresent();

    //Now since we have the renderer and the window we can init the 'AlphabetTexture' that we use for drawing text
    if (loadAlphabetTextrue( MainGameWindow.getRendererPointer() ) <0 ) {
        std::cerr << "Failed to init the \'AlphabetTexture\'\n"
             << "SDL_GetError()=>"<<SDL_GetError() << std::endl;
        return -1;
    }


    //Now load the main menu
    MainGameWindow.loadMainMenu();
    MainGameWindow.renderPresent();



    //!OK I DON'T KNOW HOW YOU DID THIS BUT NOW YOU HAVE TO GIVE THE USER/PLAYER THE ABILITY TO
    //!CLICK ON TROOPS AND MOVE THEM
    ////////////////////////////////

    //!I'm gonna make the above one
    //!YEAHHH.... I CAN HIGHLIGHT IT BUT I NEED TO MAKE A MAIN MENU NOW BECAUSE I JUST KEEP SKIPPING IT
    //!BECAUSE I CAN'T CALL ANY FUNCTIONS WITH THE FUNCTION POINTER

    //!YEAHHH... YOU MADE IT SOMEHOW WITH THAT BUG WHERE YOU PASS 'this' AND INSIDE THE FUNCTION 'this' BECOMES 'this'
    //!ANYWAY;LETS SAY THAT YOU DID IT BUT NOW YOU HAVE TO MAKE....UMMM...LET'S SAY THE UNITS MOVEABLE AND
    //!ALSO CLEAR THE MAP AND MAKE THE MAP HAVE CLEAR FIELD,AND ALSO HIGHLIGHT ALL THE MOVABLE POSITIONS OF
    //!THE HIGHLIGHTED UNIT WITH A COLOR


    //!GAMEWIDNOW_CLASS.CPP->handlekeyboard events IS WHERE YOU LEFT OFF
    //!YOU HAVE TO MAKE A CAMERA THAT IS MOVED WITH THE KEYBOARD AND THEN MAKE IT ALSO MOVEABLE WITH THE MOUSE
    //!JUST LIKE IN THE ACTUALL GAME

    //!TRY MAKING THE PLAYERCAMERA TO ACTUALLY WORK SINCE I HAVEN'T CHANGED ANYTHING RN BECAUSE YOU
    //!ALSO HAVE TO CHANGE THE WAY INPUT WORKS WITH THE MOUSE
    //!I TRIED (WindowWidth / PlayerCamera.w) TO GET A ZOOM VALUE BETWEEN (0-10) TO ZOOM
    //!INTO A SPECIFIC AREA OF THE GAME BUT IT'S PROBABLY BETTER TO KEEP IT AS A CONSTANT
    //!ALSO TRY MAKING THE ACTUAL GAME LOGIC WHAT IS THE GAME SUPPOSED TO BE
    //!MAKE CITIES AND MAKE A COMBAT SYSTEM

    //!OKAY UNITS CAN NOW "EAT" EACH OTHER JUST LIKE IN CHESS BUT IT'S NOT THE COMBAT SYSTEM I WANT
    //!ANYWAY... NOW I HAVE TO MAKE A REAL RANDOM GAME FUNCTION. ONE THAT ACTUALLY DOES SOMETHING
    //!AND THAT TAKES AS LITTLE PARAMATERS AS POSSIBLE FOR EXAMPLE:
    //!MAKE A WRAPPER FUNCTION FOR ALL THIS MainGameWindow.setNew_Field_w,MainGameWindow.setNew_FieldMap_w...
    //!AND MAKE IT A PART OF THE 'MAINGAMEWINDOW' CLASS

    SDL_RenderPresent(MainGameWindow.getRendererPointer());


    //This is used for the button function pointers so that i don't have to set it when calling the function
    MainGameWindow.setNew_FieldMap_w(10);
    MainGameWindow.setNew_FieldMap_h(10);

    //This sets the width and height of the fields in the game that will drawn onto the screen
    //800*600 = 10*10
    //MainGameWindow.setNew_PlayerCamera(0,0,MainGameWindow.get_WindowWidth(),MainGameWindow.get_WindowHeight());//change the player camera so that he can see the entire field
    MainGameWindow.setNew_Field_w(MainGameWindow.get_WindowWidth() / MainGameWindow.FieldMap_w *//gives the max amount of fields that can be printed
                                  MainGameWindow.get_WindowWidth() / MainGameWindow.get_PlayerCamera()->w);//increases their size depending on the player camera
    MainGameWindow.setNew_Field_h(MainGameWindow.get_WindowHeight() / MainGameWindow.FieldMap_h *
                                  MainGameWindow.get_WindowHeight() / MainGameWindow.get_PlayerCamera()->h);
    //cout << MainGameWindow.Field_w <<" "<<MainGameWindow.Field_h<<endl;

    //!MAIN LOOP
    SDL_Event MainLoopEvent;
    bool VisualChangeHappened = true;


    while (!QuitGame) //MAIN LOOP
    {
        while (SDL_PollEvent(&MainLoopEvent)) //HANDLE EVENT LOOP
        {
            //MOUSE BUTTON PRESSED
            if (MainLoopEvent.type == SDL_MOUSEBUTTONDOWN) {

                //THE LEFT MOUSE BUTTON WASN'T PRESSED SO WE DON'T HIGHLIGHT IT NOR DO WE RESPOND IN THE MAIN MENU
                if (MainLoopEvent.button.button  != SDL_BUTTON_LEFT) {
                    continue;
                }

                //Is the main menu being drawn?
                if (MainGameWindow.getDrawnMainMenu() == false) { //THE MAIN MENU ISN'T DRAWN

                    //We have to re-present the renderer
                    VisualChangeHappened = true;

                    //since the main menu isn't drawn we only have to highlight a field
                    MainGameWindow.handle_LMBClickOnFieldMap();

                }
                else { //THE MAIN MENU IS DRAWN
                    //Handle all button events in the main menu
                    if (MainGameWindow.handleMainMenuMouseEvent(MainLoopEvent) == 1) {

                        //We have to re-present the renderer
                        VisualChangeHappened = true;

                        std::cout << "A button from the main menu has been clicked!"<<std::endl;
                    }
                }


            }
            else if (MainLoopEvent.key.type == SDL_KEYDOWN) {

                //If the main menu isn't drawn (FieldMap is drawn)
                if (!MainGameWindow.getDrawnMainMenu()) {

                    //If a key with a function was then the screen can be updated
                    if (MainGameWindow.handle_keyboardOnFieldMap(MainLoopEvent) >= 0) {
                        VisualChangeHappened = true;
                    }

                }

            }
            else if (MainLoopEvent.type == SDL_QUIT) {
                QuitGame = true;
            }

            //If no visual changes have happened ,if there were some then set it back to false and re-present the renderer
            if (VisualChangeHappened == false) continue;
            else VisualChangeHappened = false;

            //DRAWING!
            if (MainGameWindow.getDrawnMainMenu()) {
                //!cout << "drawing main menu"<<endl;
                SDL_RenderClear(MainGameWindow.getRendererPointer());
                MainGameWindow.drawMainMenu();
                SDL_RenderPresent(MainGameWindow.getRendererPointer());
            }
            else {
                //!cout << "drawing the game"<<endl;
                SDL_RenderClear(MainGameWindow.getRendererPointer());
                MainGameWindow.drawFieldMap();
                SDL_RenderPresent(MainGameWindow.getRendererPointer());
            }

        }

        //Delay for 10ms so that I avoid the 100% CPU usage (yeah I know it's bad but it works for my PC well enough)
        SDL_Delay(10);

    }


    //Quit everything
    SDL_Quit();
    IMG_Quit();

    return 0;
}

