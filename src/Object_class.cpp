#include "Object_class.h"

Object_class::Object_class() {
    //ctor
    Position = {0,0,0,0};
    ObjectTexture = NULL;

    //Set it to pink
    ErrorColor.r = 255;
    ErrorColor.g = 0;
    ErrorColor.b = 255;
    ErrorColor.a = 0;
}


Object_class::Object_class(SDL_Rect New_Position) {
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



Object_class::Object_class(int new_x,int new_y,int new_w,int new_h) {
    //Setup the new dimensions
    Position.x = new_x;
    Position.y = new_y;
    Position.w = new_w;
    Position.h = new_h;

    //Just in case
    ObjectTexture = NULL;
    //Set it to pink
    ErrorColor.r = 255;
    ErrorColor.g = 0;
    ErrorColor.b = 255;
    ErrorColor.a = 0;
}



Object_class::Object_class(SDL_Color New_ErrorColor) {
    //Set it to pink as the std default
    ErrorColor.r = 255;
    ErrorColor.g = 0;
    ErrorColor.b = 255;
    ErrorColor.a = 0;
    memcpy( (void*)&ErrorColor, (void*)&New_ErrorColor, sizeof(SDL_Color) );

    //Just in case
    Position = {0,0,0,0};
    ObjectTexture = NULL;
}


Object_class::Object_class(SDL_Rect New_Position,SDL_Color New_ErrorColor) {
    //For error checking
    Position = {-1,-1,-1,-1};
    //Set it to pink as the std default
    ErrorColor.r = 255;
    ErrorColor.g = 0;
    ErrorColor.b = 255;
    ErrorColor.a = 0;

    memcpy( (void*)&Position, (void*)&New_Position ,sizeof(SDL_Rect) );
    memcpy( (void*)&ErrorColor, (void*)&New_ErrorColor,sizeof(SDL_Color) );

    //Just in case
    ObjectTexture = NULL;
}


Object_class::~Object_class() {
    //dtor

    if (ObjectTexture != NULL)
        SDL_DestroyTexture(ObjectTexture);

}



void Object_class::printInfo() {
    std::cout << "Position x y w h=>"<<Position.x<<" "<<Position.y<<" "<<Position.w<<" "<<Position.h<<std::endl
         << "ErrorColor r g b a=>"
         <<(int)ErrorColor.r<<" "<<(int)ErrorColor.g<<" "<<(int)ErrorColor.b<<" "<<(int)ErrorColor.a<<std::endl;
}




//!-----------------------------!//
//!     SET-TYPE FUNCTIONS      !//
//!-----------------------------!//

void Object_class::setNew_Position(SDL_Rect New_Position) {
    memcpy( (void*)&Position, (void*)&New_Position, sizeof(SDL_Rect) );
}

void Object_class::setNew_ErrorColor(SDL_Color New_ErrorColor) {
    memcpy( (void*)&ErrorColor, (void*)&New_ErrorColor, sizeof(SDL_Color) );
}

    //'return 0;' on success 'return -1;' on error
int Object_class::setNew_ObjectTexture(const char *Path,SDL_Renderer *TargetRenderer) {

    //Load in the surface
    SDL_Surface *LoadedSurface = NULL;
    LoadedSurface = IMG_Load(Path);
    if (LoadedSurface == NULL) {
        cerr << "Object_class::setNewObjectTexture()=>Failed to load specified file with path!\n"
             << "SDL_GetError()=>" << SDL_GetError() << endl;
        return -1;
    }

    //Create the 'ObjectTexture'
    ObjectTexture = NULL;
    ObjectTexture = SDL_CreateTextureFromSurface(TargetRenderer,LoadedSurface);
    if (ObjectTexture == NULL) {
        cerr << "Object_class::setNewObjectTexture()=>Failed to create texture from surface!\n"
             << "SDL_GetError()=>" << SDL_GetError() << endl;
        return -1;
    }

    return 0;
}



//!-----------------------------!//
//!     GET-TYPE FUNCTIONS      !//
//!-----------------------------!//


SDL_Rect Object_class::getPosition() {
    return Position;
}



//!---------------------------------!//
//!     DRAWING-TYPE FUNCTIONS      !//
//!---------------------------------!//



int Object_class::drawSelfToScreen_Simple(SDL_Renderer *TargetRenderer) {
    cout << "the base func!\n";
    //Obvious error check
    if (TargetRenderer == NULL) {
        cerr << "Object_class::drawSelfToScreen_Simple()=>\'TargetRenderer\' is set to NULL"<<endl;
        return -1;
    }

    //The texture is missing so we have to paint the area of the object with the error color of the object
    if (ObjectTexture == NULL) {
        //Remember the old color switch to the new one then back to the old one
        unsigned char r,g,b,a;
        SDL_GetRenderDrawColor(TargetRenderer,&r,&g,&b,&a);
        SDL_SetRenderDrawColor(TargetRenderer,ErrorColor.r,ErrorColor.g,ErrorColor.b,ErrorColor.a);
        SDL_RenderFillRect(TargetRenderer,&Position);
        SDL_SetRenderDrawColor(TargetRenderer,r,g,b,a);
        return 1;
    }
    if (SDL_RenderCopy(TargetRenderer,ObjectTexture,NULL,&Position) < 0) {
        cerr << "Object_class::drawSelfToScreen_Simple()=>Failed to draw the \'ObjectTextue\' to the renderer\n"
             << "SDL_GetError()=>"<<SDL_GetError() << endl;
        return -1;
    }

    return 0;
}

