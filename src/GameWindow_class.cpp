#include "GameWindow_class.h"

GameWindow_class::GameWindow_class()
{
    //ctor
    //should I draw the main menu first OF COURSE!
    DrawnMainMenu = true;

    //There will be no field highlighted (1st the main menu is drawn;2nd the player has to click to higlhight a field)
    HighlightedField_xpoz = -1;
    HighlightedField_ypoz = -1;

    //Player camera
    PlayerCamera.w = 500;//default
    PlayerCamera.h = 500;//default
    PlayerCamera.x = 0;
    PlayerCamera.y = 0;
    PlayerCamera_OffsetChange = 5;

}





GameWindow_class::~GameWindow_class()
{
    //dtor
    //clear the vector that holds the info about all the buttons in the main menu
    MainMenuButton.clear();

    //remember that you allocated all the PlacedUnit object pointers onto the heap with 'calloc()' now free it
    for (int i=0;i<FieldMap_w;++i) {
        for (int j=0;j<FieldMap_h;++j) {
            if (FieldMap[i][j].PlacedUnit != NULL) {
                free(FieldMap[i][j].PlacedUnit);
            }
        }
    }
}




//!-----------------------------!//
//!     MAIN MENU FUNCTIONS     !//
//!-----------------------------!//




int GameWindow_class::loadMainMenu() {

    //The player is now on the main menu
    DrawnMainMenu = true;

    //Clear it
    MainMenuButton.clear();

    //Load the background
    SDL_RenderClear(rendy);
    //Across the entire screen
    if (SDL_RenderCopy(rendy,BackgroundTexture,NULL,NULL) <0) {
        std::cerr << "GameWindow_class::loadMainMenu()=>Can't load main menu background\n"
             << "SDL_GetError()=>" << SDL_GetError() << std::endl;
        return -1;
    }

    //Load all the buttons
    ButtonObject_Class tmp_ButtonObject(SDL_Rect({10,WindowHeight-100,200,100}),SDL_Color({0,0,255,0}));
    tmp_ButtonObject.setNew_ButtonText("NEW  GAME");
    tmp_ButtonObject.doOnClickFunction = (void* (*) (void*))&GameWindow_class::loadSetNewGame;
    MainMenuButton.push_back(tmp_ButtonObject);

    tmp_ButtonObject.setNew_Position( SDL_Rect({10,WindowHeight-220,200,100}));
    tmp_ButtonObject.setNew_ErrorColor( SDL_Color({255,0,0,0}) );
    tmp_ButtonObject.setNew_ButtonText("LOAD GAME");
    MainMenuButton.push_back(tmp_ButtonObject);

    return 0;
}


//!-----------------------------!//
//!     HANDLING FUNCTIONS      !//
//!-----------------------------!//


//handle the left mouse button click on a field in field map
int GameWindow_class::handle_LMBClickOnFieldMap() {

    //HIGHLIGHTING FIELDS
    int mouse_x,mouse_y;
    SDL_GetMouseState(&mouse_x,&mouse_y);

    int ClickedField_xpoz = (PlayerCamera.x + mouse_x) / Field_w;
    int ClickedField_ypoz = (PlayerCamera.y + mouse_y) / Field_h;

    //std::cout << "ClickedField_xpoz=>"<<ClickedField_xpoz<<" ClickedField_ypoz=>"<<ClickedField_ypoz<<std::endl;

    //Check whether the highlighted field clicked again
    if (HighlightedField_xpoz == ClickedField_xpoz) {
        if (HighlightedField_ypoz == ClickedField_ypoz) {

            //The highlighted field was clicked on again so we un-highlightgh it
            HighlightedField_xpoz = -1;
            HighlightedField_ypoz = -1;
            return 1;
        }
    }
    //Check whether one of the alredy highlighted fields around him was clicked (aka. if he moved the unit to another position)
    if (HighlightedField_xpoz >= 0 && HighlightedField_ypoz >= 0) {
        if ( movePlacedUnit(HighlightedField_xpoz,HighlightedField_ypoz,ClickedField_xpoz,ClickedField_ypoz) >= 0) {
            //He moved the unit to another position so there's nothing left to highlight
            return 0;
        }
    }


    //Highligght and draw the field to the renderer
    highlightField(ClickedField_xpoz , ClickedField_ypoz,SDL_Color ({255,0,0,0}));

    //Set the variables accordingly
    HighlightedField_xpoz = ClickedField_xpoz;
    HighlightedField_ypoz = ClickedField_ypoz;


    //
    //WE HAVE HIGHLIGHTED THE FIELD BUT NOW WE HAVE TO HIGHLIGHT ALL THE FIELDS THAT IT CAN GO TO
    //
    //Check whether there is a unit placed
    if (FieldMap[HighlightedField_xpoz][HighlightedField_ypoz].PlacedUnit == NULL) {
        return 2;
    }

    //test
    //std::cout << "the maximum speed of the highlighted unit is: "
    //     <<BasicUnit[FieldMap[HighlightedField_xpoz][HighlightedField_ypoz].PlacedUnit->get_UnitType()].max_speed<<std::endl
    //     << "x="<<HighlightedField_xpoz<<" y= "<<HighlightedField_ypoz<<std::endl;


    highlightFieldsAroundHighlightedField(SDL_Color({0,0,255,0}));

    return 0;
}





//handle keyboard presses on the field map
    //return -1; if none of the playing keys was pressed, return 0; if a key was pressed and everything execute well
int GameWindow_class::handle_keyboardOnFieldMap(SDL_Event &event) {

    if (event.key.type == SDL_KEYDOWN) {

        //testing
        //std::cout << PlayerCamera.x <<" "<<PlayerCamera.y <<" "<<PlayerCamera.w <<" "<<PlayerCamera.h <<std::endl;
        //std::cout << FieldMap_h*Field_h<<" : "<<FieldMap_w*Field_w<<std::endl;

        //
        //Check WASD keys for player camera movement
        //
        if (event.key.keysym.sym == SDLK_w) {

            //check for out of bounds
            if (PlayerCamera.y - PlayerCamera_OffsetChange <= 0) {
                //Check whether the user is still pressing the same button even though everything is up to date
                if (PlayerCamera.y == 0) return -1;
                PlayerCamera.y = 0;
                //PlayerCamera.h = FieldMap_h*Field_h / 4;
            }
            else {
                PlayerCamera.y -= PlayerCamera_OffsetChange;
                //PlayerCamera.h = (FieldMap_h*Field_h/4);
            }
            return 0;
        }
        else if (event.key.keysym.sym == SDLK_s) {

            //check for out of bounds
            if (PlayerCamera.y + PlayerCamera_OffsetChange +PlayerCamera.h >= FieldMap_h*Field_h) {
                //Check whether the user is still pressing the same button even though everything is up to date
                if (PlayerCamera.y == (FieldMap_h*Field_h*3/4) ) return -1;
                PlayerCamera.y = (FieldMap_h*Field_h*3/4) ;
                //PlayerCamera.h = FieldMap_h*Field_h / 4;
            }
            else {
                PlayerCamera.y += PlayerCamera_OffsetChange;
                //PlayerCamera.h = (FieldMap_h*Field_h/4);
            }
            return 0;
        }
        else if (event.key.keysym.sym == SDLK_d) {

            //check for out of bounds
            if (PlayerCamera.x + PlayerCamera_OffsetChange +PlayerCamera.w >= FieldMap_w*Field_w) {
                //Check whether the user is still pressing the same button even though everything is up to date
                if (PlayerCamera.x == (FieldMap_w*Field_w*3/4) ) return -1;
                PlayerCamera.x = (FieldMap_w*Field_w*3/4) ;
                //PlayerCamera.w = FieldMap_w*Field_w / 4;
            }
            else {
                PlayerCamera.x += PlayerCamera_OffsetChange;
                //PlayerCamera.w = (FieldMap_w*Field_w/4);
            }
            return 0;
        }
        else if (event.key.keysym.sym == SDLK_a) {

            //check for out of bounds
            if (PlayerCamera.x - PlayerCamera_OffsetChange <= 0) {
                //Check whether the user is still pressing the same button even though everything is up to date
                if (PlayerCamera.x == 0) return -1;
                PlayerCamera.x = 0;
                //PlayerCamera.w = FieldMap_w*Field_w / 4;
            }
            else {
                PlayerCamera.x -= PlayerCamera_OffsetChange;
                //PlayerCamera.w = (FieldMap_w*Field_w/4);
            }
            return 0;
        }

        //A key was pressed but I don't know which (it wasn't binded to a command)
        return -1;
    }

    return 0;
}



//Highlight this field with this color
int GameWindow_class::highlightField(int Field_xpoz,int Field_ypoz,SDL_Color Color) {

    //Get the current render draw color and save before putting on this new one
    unsigned char r,g,b,a;
    SDL_GetRenderDrawColor(rendy,&r,&g,&b,&a);
    SDL_SetRenderDrawColor(rendy,Color.r,Color.g,Color.b,Color.a);

    //The field rectangle that has to be highlighted

    SDL_Rect dest = { Field_xpoz * Field_w - PlayerCamera.x ,
                      Field_ypoz * Field_h - PlayerCamera.y ,
                      Field_w,Field_h};


    //5px width is the highlighted rectangle
    for (unsigned int i=0;i<5;++i) {
        SDL_RenderDrawRect(rendy,&dest);
        dest.x-=1;
        dest.y-=1;
        dest.w+=2;
        dest.h+=2;
    }

    //std::cout << "Field_xpoz=>"<<Field_xpoz<<" Field_ypoz=>"<<Field_ypoz<<std::endl;
    //restore the old render color
    SDL_SetRenderDrawColor(rendy,r,g,b,a);
    return 0;
}



//Highlights all the fields around the highlighted units position
    //it takes the 'max_speed' from 'BasicUnit[64]' and highlights all the fields within that radius
int GameWindow_class::highlightFieldsAroundHighlightedField(SDL_Color Color) {

    if (HighlightedField_xpoz <0 || HighlightedField_xpoz > FieldMap_w) return -1;
    if (HighlightedField_ypoz <0 || HighlightedField_ypoz > FieldMap_h) return -1;

    if (FieldMap[HighlightedField_xpoz][HighlightedField_ypoz].PlacedUnit == NULL) return -2;

//This is the highlighted fields unit maximum speed (how many fields can it move in all directions from it's poisition)
int HighlightedField_PlacedUnit_max_speed = BasicUnit[FieldMap[HighlightedField_xpoz][HighlightedField_ypoz].PlacedUnit->get_UnitType()].max_Speed;

    //THE LONG PART OF HIGHLIGHTING THE FIELDS

        // +x axis (GO RIGHT) also check for end of bounds
        for (int i=HighlightedField_xpoz+1;
            (i<HighlightedField_xpoz + HighlightedField_PlacedUnit_max_speed) && (i<FieldMap_w) ;
            ++i)
        {
            highlightField(i,HighlightedField_ypoz,Color);
        }
        // -x axis (GO LEFT) also check for end of bounds
        for (int i=HighlightedField_xpoz-1;
            (i > HighlightedField_xpoz - HighlightedField_PlacedUnit_max_speed) && (i>-1);
            --i)
        {
            highlightField(i,HighlightedField_ypoz,Color);
        }
        // -y axis (GO UP) also check for end of bounds
        for (int i=HighlightedField_ypoz-1;
            (i > HighlightedField_ypoz - HighlightedField_PlacedUnit_max_speed) && (i>-1);
            --i)
        {
            highlightField(HighlightedField_xpoz,i,Color);
        }
        // +y axis (GO DOWN) also check for end of bounds
        for (int i=HighlightedField_ypoz+1;
            (i < HighlightedField_ypoz + HighlightedField_PlacedUnit_max_speed) && (i<FieldMap_h);
            ++i)
        {
            highlightField(HighlightedField_xpoz,i,Color);
        }


        // +x axis
        for (int i=HighlightedField_xpoz+1;
             (i < HighlightedField_xpoz + HighlightedField_PlacedUnit_max_speed) && (i<FieldMap_w);
             ++i)
        {
            // +y axis (GO TO THE DOWN RIGHT CORNER)
            for (int j=HighlightedField_ypoz+1;
                 (j<HighlightedField_ypoz + HighlightedField_PlacedUnit_max_speed) && (j < FieldMap_h);
                 ++j)
            {
                highlightField(i,j,Color);
            }
            // -y axis (GO TO THE TOP RIGHT CORNER)
            for (int j=HighlightedField_ypoz-1;
                 (j>HighlightedField_ypoz - HighlightedField_PlacedUnit_max_speed) && (j > -1);
                 --j)
            {
                highlightField(i,j,Color);
            }
        }

        // -x axis
        for (int i=HighlightedField_xpoz-1;
             (i > HighlightedField_xpoz - HighlightedField_PlacedUnit_max_speed) && (i>-1);
             --i)
        {
            // +y axis (GO TO THE DOWN LEFT CORNER)
            for (int j=HighlightedField_ypoz+1;
                 (j<HighlightedField_ypoz + HighlightedField_PlacedUnit_max_speed) && (j < FieldMap_h);
                 ++j)
            {
                highlightField(i,j,Color);
            }
            // -y axis (GO TO THE TOP LEFT CORNER)
            for (int j=HighlightedField_ypoz-1;
                 (j>HighlightedField_ypoz - HighlightedField_PlacedUnit_max_speed) && (j > -1);
                 --j)
            {
                highlightField(i,j,Color);
            }
        }

    //testing
    //SDL_RenderPresent(rendy);
    //SDL_Delay(4000);

    return 0;
}


//Handles the main menu mouse events
int GameWindow_class::handleMainMenuMouseEvent(SDL_Event &Event) {

    int mouse_xpoz,mouse_ypoz;
    SDL_GetMouseState(&mouse_xpoz,&mouse_ypoz);

    for (unsigned int i=0;i<MainMenuButton.size();++i) {

        //Check whether it is inside it
        if (mouse_xpoz < MainMenuButton[i].getPosition().x)
            continue;
        if (mouse_xpoz > MainMenuButton[i].getPosition().x + MainMenuButton[i].getPosition().w)
            continue;
        if (mouse_ypoz < MainMenuButton[i].getPosition().y)
            continue;
        if (mouse_ypoz > MainMenuButton[i].getPosition().y + MainMenuButton[i].getPosition().h)
            continue;

        std::cout << "MainMenuButton["<<i<<"].doOnClickFunction(NULL)=>is now working!"<<std::endl;
        //!WAIT WHAT THE FUCK WHY DO I HAVE TO SEND IT A REFERENCE TO 'this'!! it works fine but I wouldn't keep
        //!this a secret because it's definitely a bug or something miscellaneous
        MainMenuButton[i].doOnClickFunction((void*)this);
        std::cout << "MainMenuButton["<<i<<"].doOnClickFunction(NULL)=>has now stopped working!"<<std::endl;
        return 1;
        //break;
    }

    return 0;
}



//!-------------------------!//
//!     LOAD FUNCTIONS      !//
//!-------------------------!//

//!NOT USED
//Loads a random game
int GameWindow_class::loadRandomGame() {

    //The main menu shouldn't be drawn anymore
    this->DrawnMainMenu = false;

    //load the std basic units for playing aka. no mods
    loadBasicUnit_std(BasicUnit);

    //Get the random width and height + error check
    this->FieldMap_w = getRandomINT(4,10);
    if (this->FieldMap_w < 0) this->FieldMap_w*= (-1);
    this->FieldMap_h = getRandomINT(4,10);
    if (this->FieldMap_h < 0) this->FieldMap_h*= (-1);
    std::cout << "GameWindow_class::loadRandomGame()=>FieldMap_w= "
         <<this->FieldMap_w <<" FieldMap_h= "<<this->FieldMap_h <<std::endl;

    for (int i=0;i<this->FieldMap_w;++i) {
        for (int j=0;j<this->FieldMap_h;++j) {
            this->FieldMap[i][j].status = 0;
            this->FieldMap[i][j].PlacedUnit = NULL;
        }
    }

    return 0;
}

//!NOT USED
//loads a new game with the w and h specified
int GameWindow_class::loadNewGame(int new_FieldMap_w,int new_FieldMap_h) {

    //The main menu shouldn't be drawn anymore
    this->DrawnMainMenu = false;

    //load the std basic units for playing aka. no mods
    loadBasicUnit_std(BasicUnit);

    //set the new variables and clear the memory of the 2D array
    this->FieldMap_w = new_FieldMap_w;
    this->FieldMap_h = new_FieldMap_h;

     for (int i=0;i < this->FieldMap_w;++i) {
        for (int j=0;j < this->FieldMap_h;++j) {
            this->FieldMap[i][j].status = 0;
            this->FieldMap[i][j].PlacedUnit = NULL;
        }
    }


    return 0;
}


//same as 'loadNewGame' except that this here takes the 'FieldMap_w' and 'FieldMap_h' that was set and uses them
int GameWindow_class::loadSetNewGame(GameWindow_class *trgt) {

    //!THIS MAKES THE ABOVE ONE WHEN CALLING THE FUNCTION POINTER EVEN WEIRDER???
    //std::cout << "this="<<this <<" trgt="<<trgt<<endl;//this = 0x... while trgt= 0x1 (segmentation fault)
    //std::cout << "GameWindow_class::loadSetNewGame()=>FieldMap_w="
    //     <<this->FieldMap_w<<" FieldMap_h="<<this->FieldMap_h<<std::endl;

    //The main menu shouldn't be drawn anymore
    this->DrawnMainMenu = false;

    //load the std basic units for playing aka. no mods
    loadBasicUnit_std(BasicUnit);

    //std::cout << "GameWindow_class::loadSetNewGame()=> everything down here is from this function!"<<std::endl;

     for (int i=0;i < this->FieldMap_w;++i) {
        for (int j=0;j < this->FieldMap_h;++j) {

            //10% chance to spawn a unit here
            int RandomInt = getRandomINT(0,100);

            //No unit here
            if (RandomInt > 10) {
                this->FieldMap[i][j].status = 0;
                this->FieldMap[i][j].PlacedUnit = NULL;
                continue;
            }

            //Put a random unit here
            this->FieldMap[i][j].PlacedUnit = (UnitObject_class*)calloc(1,sizeof(UnitObject_class));
                //since we aren't calling the constructor to set it all to 0 we have to set the memory to it

            //reset the 'RandomInt' to a new random number that will be the unit type
            RandomInt = getRandomINT(0,7);
            this->FieldMap[i][j].PlacedUnit->changeType( RandomInt );

            //std::cout << "i= "<<i<<" j= "<<j<<"=>";
            //std::cout << "this newly set unit type is: "<<this->FieldMap[i][j].PlacedUnit->get_UnitType()<<std::endl;
        }
    }

    //setup the player camera
    PlayerCamera.x = 0;
    PlayerCamera.y = 0;
    PlayerCamera.w = FieldMap_w*Field_w/4;
    PlayerCamera.h = FieldMap_h*Field_h/4;

    //std::cout <<"GameWindow_class::loadSetNewGame()=>function has ended!"<<std::endl;

    return 0;
}



//handles unit movement
    //retruns <0; on failure and return 0; on success
int GameWindow_class::movePlacedUnit(int fromFieldMap_xpoz,int fromFieldMap_ypoz,int toFieldMap_xpoz,int toFieldMap_ypoz) {

    //std::cout << "fromFieldMap_xpoz=>"<<fromFieldMap_xpoz<<" fromFieldMap_ypoz=>"<<fromFieldMap_ypoz
    //          << " toFieldMap_xpoz=>"<<toFieldMap_xpoz<<" toFieldMap_ypoz=>"<<toFieldMap_ypoz<<std::endl;

    //Check whether they are the same field
    if (fromFieldMap_xpoz == toFieldMap_xpoz && fromFieldMap_ypoz == toFieldMap_ypoz) {
        return -1;
    }
    //Check whether there even is a placed unit on this field
    if (FieldMap[fromFieldMap_xpoz][fromFieldMap_ypoz].PlacedUnit == NULL) {
        return -1;
    }


    //check for invalid out of bounds move
    //Check whether the player wants to move the unit out of bounds (it has to be inside the FieldMap it shouldn't go outisde it)
    if (toFieldMap_xpoz >= FieldMap_w || toFieldMap_xpoz < 0) {
        return -1;
    }
    if (toFieldMap_ypoz >= FieldMap_h || toFieldMap_ypoz < 0) {
        return -1;
    }



    //There is a unit placed there already so go and see whether you can destroy it
    if (FieldMap[toFieldMap_xpoz][toFieldMap_ypoz].PlacedUnit != NULL) {
        //Get the units max_AttackRange (the range from which it can attack)
        int MovingUnit_max_AttackRange = BasicUnit[FieldMap[toFieldMap_xpoz][toFieldMap_ypoz].PlacedUnit->get_UnitType()].max_AttackRange;

        //Check the x axis
        if (fromFieldMap_xpoz + MovingUnit_max_AttackRange <= toFieldMap_xpoz || fromFieldMap_xpoz - MovingUnit_max_AttackRange >= toFieldMap_xpoz)
        {
            return -2;
        }
        //Check the y axis
        if (fromFieldMap_ypoz + MovingUnit_max_AttackRange <= toFieldMap_ypoz || fromFieldMap_ypoz - MovingUnit_max_AttackRange >= toFieldMap_ypoz)
        {
            return -2;
        }

        //First we free the allocated memory of the unit that is now destroyed
        free(FieldMap[toFieldMap_xpoz][toFieldMap_ypoz].PlacedUnit);

    }
    //If there is no unit then move this unit to the specified position
    else {

        //Check whether you can even move to that position from your current position
        int MovingUnit_max_speed = BasicUnit[FieldMap[fromFieldMap_xpoz][fromFieldMap_ypoz].PlacedUnit->get_UnitType()].max_Speed;
        //Check the x axis
        if (fromFieldMap_xpoz + MovingUnit_max_speed <= toFieldMap_xpoz || fromFieldMap_xpoz - MovingUnit_max_speed >= toFieldMap_xpoz)
        {
            return -2;
        }
        //Check the y axis
        if (fromFieldMap_ypoz + MovingUnit_max_speed <= toFieldMap_ypoz || fromFieldMap_ypoz - MovingUnit_max_speed >= toFieldMap_ypoz)
        {
            return -2;
        }

    }

    //MOVING THE PLACEDUNIT FROM TO:
        //Allocate the memory
        FieldMap[toFieldMap_xpoz][toFieldMap_ypoz].PlacedUnit = (UnitObject_class*) malloc(sizeof(UnitObject_class));
        memset( (void*)FieldMap[toFieldMap_xpoz][toFieldMap_ypoz].PlacedUnit,0,sizeof(UnitObject_class));
        //Copy the values from to it

        memcpy( (void*)FieldMap[toFieldMap_xpoz][toFieldMap_ypoz].PlacedUnit,
                (void*)FieldMap[fromFieldMap_xpoz][fromFieldMap_ypoz].PlacedUnit,
                sizeof(UnitObject_class) );


    //CLEAR THE MEMORY OF THE OLD POSITION OF THE UNIT:
        //Don't froget to clear the memory
        free(FieldMap[fromFieldMap_xpoz][fromFieldMap_ypoz].PlacedUnit);
        FieldMap[fromFieldMap_xpoz][fromFieldMap_ypoz].PlacedUnit = NULL;



    //FieldMap[toFieldMap_xpoz][toFieldMap_ypoz].PlacedUnit = FieldMap[fromFieldMap_xpoz][fromFieldMap_ypoz].PlacedUnit;

    return 0;
}




//!-----------------------------!//
//!     DRAWING FUNCTIONS       !//
//!-----------------------------!//


//Draws the MainMenu but doesn't present it (also clears the renderer duh)
int GameWindow_class::drawMainMenu() {

    //std::cout << "renderer setup"<<std::endl;
    //standard setup
    SDL_RenderClear(rendy);
    SDL_RenderCopy(rendy,BackgroundTexture,NULL,NULL);

    //cout << "time to draw\n";
    for (unsigned int i=0;i<MainMenuButton.size();++i) {
        MainMenuButton[i].drawSelfToScreen_Simple(rendy);
        drawTextToRenderer_simple(rendy,MainMenuButton[i].getPosition(),MainMenuButton[i].getButtonText().c_str());
        //the same as the above except that it's faster since I know the font size
        //drawTextToRenderer(rendy,MainMenuButton[i].getPosition(),MainMenuButton[i].getButtonText().c_str(),40);
    }
    //std::cout << "all drawn!"<<std::endl;

    return 0;
}




//Draws the FieldMap
int GameWindow_class::drawFieldMap() {

//std::cout<<"sizeof(FieldMap)= "<<sizeof(FieldMap)<<"B="<<sizeof(FieldMap)/1024<<"KB="<<(sizeof(FieldMap)/1024)/1024<<std::endl;

    //DRAWING THE BACKGROUND
    SDL_RenderCopy(rendy,BackgroundTexture,&PlayerCamera,NULL);

    //DRAW THE UNITS TO THE SCREEN
    for (int i=0;i < FieldMap_w ;++i) {
        for (int j=0;j < FieldMap_h ;++j) {

            //There is no unit placed here so just skip it
            if (FieldMap[i][j].PlacedUnit == NULL) continue;


            /*
            //Check whether this field is going to be even visible along the x axis
            if (i*Field_w+Field_w < PlayerCamera.x) continue;
            if (i*Field_w > PlayerCamera.x+PlayerCamera.w) continue;
            //Check whether this field is going to be even visible along the y axis
            if (j*Field_h+Field_h < PlayerCamera.y) continue;
            if (j*Field_h > PlayerCamera.y+PlayerCamera.h) continue;
            */


                //"Field_w*2,Field_h*2" is because of the 'drawTextToRenderer_simple()' optimizing it's size
            SDL_Rect dst = {i*Field_w-PlayerCamera.x,j*Field_h-PlayerCamera.y,
                            Field_w*2,Field_h*2};


            //std::cout << dst.x<<" "<< dst.y<<" "<< dst.w<<" "<< dst.h<<std::endl;


            //there is a unit here
            //cast a char into const char*
            char CastingCharacter = (char)(65+FieldMap[i][j].PlacedUnit->get_UnitType());
            char *FieldCharacter = static_cast<char*> (&CastingCharacter);
            drawTextToRenderer_simple(rendy,dst,FieldCharacter);
            //just putting the other version of the function here just in case
            //drawTextToRenderer(rendy,dst,FieldCharacter,60);


        }
    }

    //HIGHLIGHTED UNIT
    //check whether a field is highlighted
    if (HighlightedField_xpoz < 0 ) return 1;
    if (HighlightedField_ypoz < 0 ) return 1;
    if (HighlightedField_xpoz > FieldMap_w ) return 1;
    if (HighlightedField_ypoz > FieldMap_h ) return 1;
    //highlight the field
    highlightField(HighlightedField_xpoz,HighlightedField_ypoz,SDL_Color({255,0,0,0}));
    //highlight the fields that this unit can move to (the fields around this one if a unit is highlighted)
    highlightFieldsAroundHighlightedField(SDL_Color({0,0,255,0}));


    return 0;
}



//!-----------------------------!//
//!     UNUSED FUNCTIONS        !//
//!-----------------------------!//


//Calls 'SDL_RenderPresent()' and returns the 'return' of the mentioned function
void GameWindow_class::renderPresent() {
    SDL_RenderPresent(rendy);
}



//Calls 'SDL_RenderClear()'
void GameWindow_class::renderClear() {
    SDL_RenderClear(rendy);
}









//!-------------------------------!//
//!     SET-TYPE FUNCTIONS        !//
//!-------------------------------!//



//Returns 0 on success and -1 on fail
int GameWindow_class::setNew_BackgroundTexture(const char *path) {

    SDL_Surface *LoadedSurface = NULL;
    LoadedSurface = IMG_Load(path);

    if (LoadedSurface == NULL) {
        std::cerr << "GameWindow_class::setNew_BackGroundTexture()=>Failed to load the path file\n"
        << "SDL_GetError()=>" << SDL_GetError() << std::endl;
        return -1;
    }



    //set the 'BackgroundTexture'
    BackgroundTexture = NULL;
    BackgroundTexture= SDL_CreateTextureFromSurface(rendy,LoadedSurface);
    if (BackgroundTexture == NULL) {
        std::cerr << "GameWindow_class::setNew_BackgroundTexture()=>\'BackgroundTexture\'=NULL\nSDL_GetError()=>"
        <<SDL_GetError()<<std::endl;
        return -1;
    }

    SDL_FreeSurface(LoadedSurface);

    return 0;
}




//Calls 'SDL_CreateWindow()' on win and returns 0 on success and -1 on error
int GameWindow_class::setNew_win(const char* title,int x,int y,int w,int h,unsigned int flags) {
    //reset the width and height
    WindowWidth = 0;
    WindowHeight = 0;

    //set the window* to Null for error checking
    win = NULL;
    win = SDL_CreateWindow(title,x,y,w,h,flags);

    //Error check
    if (win == NULL) {
        std::cerr << "GameWindow_class::setNew_win()=>Failed to create window\n"
             << "SDL_GetError()=>" << SDL_GetError()<<std::endl;
        return -1;
    }

    //set the new width and height
    WindowWidth = w;
    WindowHeight = h;
    return 0;
}



//sets 'DrawnMainMenu' to 'to'
void GameWindow_class::set_DrawnMainMenu(bool to) {
    DrawnMainMenu = to;
}




//Calls 'GameWindow_class()' on rendy,returns 0 on success and -1 on error
int GameWindow_class::setNew_rendy(int index,unsigned long int flags) {
    rendy = NULL;
    rendy = SDL_CreateRenderer(win,index,flags);
    if (rendy == NULL) {
        std::cerr << "GameWindow_class::setNew_rendy()=>Failed to create renderer\n"
             << "SDL_GetError()=>" << SDL_GetError() << std::endl;
        return -1;
    }
    return 0;
}



//Self explanatory (this is the width and height of the 2D array that holds the info about the field)
void GameWindow_class::setNew_FieldMap_w(int new_FieldMap_w) {
    FieldMap_w = new_FieldMap_w;
}
void GameWindow_class::setNew_FieldMap_h(int new_FieldMap_h) {
    FieldMap_h = new_FieldMap_h;
}


//Self explanatory (this is the width and height of each 'FieldMap' field that will drawn in PIXELS)
void GameWindow_class::setNew_Field_w(int new_Field_w) {
    Field_w = new_Field_w;
}
void GameWindow_class::setNew_Field_h(int new_Field_h) {
    Field_h = new_Field_h;
}

//Calls 'SDL_SetRenderDrawColor()' and returns the 'return' of the mentioned function
int GameWindow_class::setRenderDrawColor(unsigned char r,unsigned char g,unsigned char b,unsigned char a) {
    return SDL_SetRenderDrawColor(rendy,r,g,b,a);
}

void GameWindow_class::setNew_HighlightedField_xpoz(int new_HighlightedField_xpoz) {
    HighlightedField_xpoz = new_HighlightedField_xpoz;
}
void GameWindow_class::setNew_HighlightedField_ypoz(int new_HighlightedField_ypoz) {
    HighlightedField_ypoz = new_HighlightedField_ypoz;
}

//Change the PlayerCamera values
void GameWindow_class::setNew_PlayerCamera(SDL_Rect new_PlayerCamera) {
    PlayerCamera.x = new_PlayerCamera.x;
    PlayerCamera.y = new_PlayerCamera.y;
    PlayerCamera.w = new_PlayerCamera.w;
    PlayerCamera.h = new_PlayerCamera.h;
}
void GameWindow_class::setNew_PlayerCamera(int x,int y,int w,int h) {
    PlayerCamera.x = x;
    PlayerCamera.y = y;
    PlayerCamera.w = w;
    PlayerCamera.h = h;
}

//!-------------------------------!//
//!     GET-TYPE FUNCTIONS        !//
//!-------------------------------!//



//Returns the renderer apointed to this object
SDL_Renderer * GameWindow_class::getRendererPointer() {
    return rendy;
}

//Return true or false depending on the 'DrawnMainMenu' variable
bool GameWindow_class::getDrawnMainMenu() {
    return DrawnMainMenu;
}

int GameWindow_class::get_WindowWidth() {
    return WindowWidth;
}
int GameWindow_class::get_WindowHeight() {
    return WindowHeight;
}

int GameWindow_class::get_HighlightedField_xpoz() {
    return HighlightedField_xpoz;
}
int GameWindow_class::get_HighlightedField_ypoz() {
    return HighlightedField_ypoz;
}
//returns the address of the 'PlayerCamera' object
SDL_Rect* GameWindow_class::get_PlayerCamera() {
    return &PlayerCamera;
}

