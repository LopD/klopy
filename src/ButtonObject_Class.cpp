#include "ButtonObject_Class.h"

ButtonObject_Class::ButtonObject_Class()
{
    //ctor
    Position = {0,0,0,0};
    ObjectTexture = NULL;

    //Set it to pink
    ErrorColor.r = 255;
    ErrorColor.g = 0;
    ErrorColor.b = 255;
    ErrorColor.a = 0;

}


ButtonObject_Class::ButtonObject_Class(SDL_Rect New_Position) {
    //copy the memory and error check along the way
    Position = {-1,-1,-1,-1};
    //static_cast<void*> (&Position);
    memcpy( (void*)&Position, (void*)&New_Position,sizeof(SDL_Rect) );

    //Just in case
    ObjectTexture = NULL;
    //Set it to pink
    ErrorColor.r = 255;
    ErrorColor.g = 0;
    ErrorColor.b = 255;
    ErrorColor.a = 0;

}


ButtonObject_Class::ButtonObject_Class(SDL_Rect New_Position,SDL_Color New_ErrorColor) {
    memcpy( (void*)&Position, (void*)&New_Position, sizeof(SDL_Rect) );
    memcpy( (void*)&ErrorColor, (void*)&New_ErrorColor, sizeof(SDL_Color) );

    ObjectTexture = NULL;

}



ButtonObject_Class::~ButtonObject_Class()
{
    //dtor
    if (ObjectTexture != NULL)
        SDL_DestroyTexture(ObjectTexture);
}


//!-----------------------------!//
//!     SET-TYPE FUNCTIONS      !//
//!-----------------------------!//

    //'return 0;' on success 'return -1;' on error
int ButtonObject_Class::setNew_ButtonText(const char* NewText) {

    ButtonText = NewText;
    return 0;
}


//!-----------------------------!//
//!     GET-TYPE FUNCTIONS      !//
//!-----------------------------!//



std::string ButtonObject_Class::getButtonText() {
    return ButtonText;
}

//Returns the pointer to the 'Position' variable
SDL_Rect* ButtonObject_Class::getpPosition() {
    return &Position;
}

//Returns just a copy of the 'Position' variable
SDL_Rect ButtonObject_Class::getPosition() {
    return Position;
}





//draws the ObjectTexture and then draws the ButtonText on top of it
    //NOTE: 'return 1' is used when the error color of the object was used instead of the 'ObjectTexture'
int ButtonObject_Class::drawSelfToScreen_Simple(SDL_Renderer *TargetRenderer) {

    if (TargetRenderer == NULL) {
        std::cerr << "ButtonObject_Class::drawSelfToScreen_Simple()=>\'TargetRenderer\' = NULL"
                  << "SDL_GetError()=>"<<SDL_GetError() << std::endl;
        return -1;
    }

    //The texture is missing so we have to paint the area of the object with the error color of the object
    if (ObjectTexture == NULL) {

        if (&Position == NULL) {
            return -99;
        }
        //Remember the old color switch to the new one then back to the old one
        unsigned char r,g,b,a;
        SDL_GetRenderDrawColor(TargetRenderer,&r,&g,&b,&a);
        SDL_SetRenderDrawColor(TargetRenderer,ErrorColor.r,ErrorColor.g,ErrorColor.b,ErrorColor.a);
        SDL_RenderFillRect(TargetRenderer,&Position);
        SDL_SetRenderDrawColor(TargetRenderer,r,g,b,a);
    }
    //The texture exists and we just slam it in there
    else {
        if (SDL_RenderCopy(TargetRenderer,ObjectTexture,NULL,&Position) < 0) {
            cerr << "ButtonObject_Class::drawSelfToScreen_Simple()=>Failed to draw the \'ObjectTextue\' to the renderer\n"
                 << "SDL_GetError()=>"<<SDL_GetError() << endl;
            return -1;
        }
    }


    //Now we get to draw the text to the area
    /*
    //get the length
    int text_length = strlen(ButtonText);
    //cout << "text_length="<<text_length <<endl;


    //each letter will be in a 1:1=w:h ratio
    int LetterDimension = 5;//in pixels
    while ( int(Position.w / LetterDimension)*int(Position.h / LetterDimension) > text_length ) {
        LetterDimension += 5;
    }
    LetterDimension -= 5;
    //cout << "LetterDimension =" <<LetterDimension <<endl;

    //The number of letters inside one row
    int RowLength = Position.w / LetterDimension;
    //cout << "RowLength="<<RowLength <<endl;

    for (int i=0;i<text_length;++i) {

        //not an uppercase letter
        if (ButtonText[i] < 65 || ButtonText[i] > 90)  {
            continue;
        }

        //The position of the letter in ASC2 as in the alphabet (A=0,B=1,C=2...)
        int pos = ButtonText[i] % 65;
        //cout << "pos="<<pos<<" "<<ButtonText[i]<<endl;

        //The positon of the character in the texture
        SDL_Rect src = { 0,pos*20,20,20 };
        //cout << "SDL_Rect src=>" <<src.x << " "<<src.y << " "<<src.w << " "<<src.h<<endl;

        //The block to display it
        SDL_Rect dst = {Position.x+ (i%RowLength)*LetterDimension, Position.y + int(i/RowLength)*LetterDimension,
                        LetterDimension,LetterDimension};

        //cout << "SDL_Rect Position=>"<<Position.x<<" "<<Position.y<<" "<<Position.w<<" "<<Position.h << endl;

        //drawing call
        if (SDL_RenderCopy(TargetRenderer,AlphabetTexture,&src,&dst) <0) {
            cerr << "GameWindow_class::drawTextToArea()=>Failed to draw letter at i="<<i
                 << "\nSDL_GetError()=>"<< SDL_GetError() << endl;
            return -1;
        }
    }
    */
    return 0;
}


//!-------------------------------------!//
//!     EVENT HANDLING FUNCTIONS        !//
//!-------------------------------------!//

    //return -1; If the mouse isn't clicking this button
    //return 1; If the mouse has clicked onto this button and the function has executed correctly
int ButtonObject_Class::handleEvent_OnClick(SDL_Event &ClickEvent) {

    //Mouse positions
    int Mouse_x,Mouse_y;
    SDL_GetMouseState(&Mouse_x,&Mouse_y);

    //Check whether the mouse has clicked onto the button
    if (Mouse_x < Position.x)
        return -1;
    if (Mouse_y < Position.y)
        return -1;
    if (Mouse_x > Position.x + Position.w)
        return -1;
    if (Mouse_y > Position.y + Position.h)
        return -1;

    std::cout << "Button has been pressed!But what is it supposed to do?"<<std::endl;

    doOnClickFunction(NULL);

    return 1;
}




