#include "CustomFunctions.h"


//draws text in a simple way to this rectangle
int drawTextToRenderer_simple(SDL_Renderer *TargetRenderer,SDL_Rect dest,const char* text)
{
    //get the length
    int text_length = strlen(text);
    //cout << "text_length="<<text_length <<endl;


    //each letter will be in a 1:1=w:h ratio
    int LetterDimension = 5;//in pixels
    while ( int(dest.w / LetterDimension)*int(dest.h / LetterDimension) > text_length ) {
        LetterDimension += 5;
    }
    LetterDimension -= 5;
    //cout << "LetterDimension =" <<LetterDimension <<endl;

    //The number of letters inside one row
    int RowLength = dest.w / LetterDimension;
    //cout << "RowLength="<<RowLength <<endl;

    for (int i=0;i<text_length;++i) {

        //not an uppercase letter
        if (text[i] < 65 || text[i] > 90)  {
            continue;
        }

        //The position of the letter in ASC2 as in the alphabet (A=0,B=1,C=2...)
        int pos = text[i] % 65;
        //cout << "pos="<<pos<<" "<<text[i]<<endl;

        //The positon of the character in the texture
        SDL_Rect src = { 0,pos*20,20,20 };
        //cout << "SDL_Rect src=>" <<src.x << " "<<src.y << " "<<src.w << " "<<src.h<<endl;

        //The block to display it
        SDL_Rect dst = {dest.x+ (i%RowLength)*LetterDimension, dest.y + int(i/RowLength)*LetterDimension,
                        LetterDimension,LetterDimension};

        //cout << "SDL_Rect dst=>"<<dst.x<<" "<<dst.y<<" "<<dst.w<<" "<<dst.h << endl;

        //drawing call
        if (SDL_RenderCopy(TargetRenderer,AlphabetTexture,&src,&dst) <0) {
            std::cerr << "GameWindow_class::drawTextToArea()=>Failed to draw letter at i="<<i
                 << "\nSDL_GetError()=>"<< SDL_GetError() << std::endl;
            return -1;
        }
    }
    return 0;

}




//draws text in into the specified dest but with a fixed font
    //return 0=>on success -1=>on fail
int drawTextToRenderer(SDL_Renderer *TargetRenderer,SDL_Rect dest,const char* text,int font_px) {
//get the length
    int text_length = strlen(text);
    //cout << "text_length="<<text_length <<endl;

    //The number of letters inside one row
    int RowLength = dest.w / font_px;
    //cout << "RowLength="<<RowLength <<endl;

    //Check whether the font is too large and no characters can be drawn

    if ( (dest.h / font_px) * (dest.w / font_px) < text_length) {
        std::cerr<<"CustomFunctions.cpp=>drawTextToRenderer()=>\'font_px\' is too large for the 'dest' rectangle!"
                 <<std::endl;
        return -1;
    }

    for (int i=0;i<text_length;++i) {

        //not an uppercase letter
        if (text[i] < 65 || text[i] > 90)  {
            continue;
        }

        //The position of the letter in ASC2 as in the alphabet (A=0,B=1,C=2...)
        int pos = text[i] % 65;
        //cout << "pos="<<pos<<" "<<text[i]<<endl;

        //The positon of the character in the texture
        SDL_Rect src = { 0,pos*20,20,20 };
        //cout << "SDL_Rect src=>" <<src.x << " "<<src.y << " "<<src.w << " "<<src.h<<endl;

        //The block to display it
        SDL_Rect dst = {dest.x+ (i%RowLength)*font_px, dest.y + int(i/RowLength)*font_px,
                        font_px,font_px};

        //cout << "SDL_Rect dst=>"<<dst.x<<" "<<dst.y<<" "<<dst.w<<" "<<dst.h << endl;

        //drawing call
        if (SDL_RenderCopy(TargetRenderer,AlphabetTexture,&src,&dst) <0) {
            std::cerr << "GameWindow_class::drawTextToArea()=>Failed to draw letter at i="<<i
                 << "\nSDL_GetError()=>"<< SDL_GetError() << std::endl;
            return -1;
        }
    }
    return 0;
}



//Loads the 'AlphabetTextrue'
int loadAlphabetTextrue(SDL_Renderer *TargetRenderer) {

    //if (TargetRenderer == NULL) return -99;

    SDL_Surface *LoadedSurface = NULL;
    LoadedSurface = IMG_Load("./graphics/alphabet.jpeg");
    if (LoadedSurface == NULL) {
        std::cerr << SDL_GetError()<<std::endl;
        return -1;
    }

    if (SDL_SetColorKey(LoadedSurface, 1 , SDL_MapRGB(LoadedSurface->format,0,0,0)) < 0 ) {
        std::cerr << "CustomFunctions=>load_AlphabetTexture()=>Failed to set color key of loaded surface to white\n"
             << "\nSDL_GetError()=>"<<SDL_GetError()<<std::endl;
        return -1;
    }


    //set the 'BackgroundTexture'
    AlphabetTexture = NULL;
    AlphabetTexture = SDL_CreateTextureFromSurface(TargetRenderer,LoadedSurface);
    if (AlphabetTexture == NULL) {
        std::cerr << "CustomFunctions=>loadAlphabetTextrue()=>Failed to create the AlphabetTexture texture\n"
                  << "SDL_GetError()=>"<<SDL_GetError()<<std::endl;
        return -2;
    }

    SDL_FreeSurface(LoadedSurface);
    return 0;
}



//Returns a random int between the value of 'from' to 'to'
int getRandomINT(int from,int to) {
    //'rand() % (to - from)'=>get a number that is '<=' their difference
    //'+from' now it's actually in between them
    int result = (rand() % (to - from)) + from;

    return result;
}


//loads in the standard info about like 7 units (unit [0] is empty aka. will be an empty field in 'FieldMap')
void loadBasicUnit_std(BasicUnit_struct trgt_arr[64]) {
    //struct BasicUnit_struct *BasicUnit = {};
    //Clear the memory
    //memset((void*)&BasicUnit,0,sizeof(BasicUnit_struct)*64);


    //[0]=>empty
    trgt_arr[0].max_HP = -1;
    trgt_arr[0].max_Speed = -1;
    trgt_arr[0].max_AttackRange = -1;
    //[1]=>civilian
    trgt_arr[1].max_HP = 50;
    trgt_arr[1].max_Speed = 3;
    trgt_arr[1].max_AttackRange = 1;
    //[2]=>scouts
    trgt_arr[2].max_HP = 50;
    trgt_arr[2].max_Speed = 3;
    trgt_arr[2].max_AttackRange = 1;
    //[3]=>AInfantry
    trgt_arr[3].max_HP = 100;
    trgt_arr[3].max_Speed = 2;
    trgt_arr[3].max_AttackRange = 1;
    //[4]=>MInfantry
    trgt_arr[4].max_HP = 120;
    trgt_arr[4].max_Speed = 5;
    trgt_arr[4].max_AttackRange = 1;
    //[5]=>CmdInfantry
    trgt_arr[5].max_HP = 175;
    trgt_arr[5].max_Speed = 3;
    trgt_arr[5].max_AttackRange = 2;
    //[6]=>LTank
    trgt_arr[6].max_HP = 200;
    trgt_arr[6].max_Speed = 4;
    trgt_arr[6].max_AttackRange = 1;
    //[7]=>HTank
    trgt_arr[7].max_HP = 400;
    trgt_arr[7].max_Speed = 2;
    trgt_arr[7].max_AttackRange = 2;
    //[8]=>LArtillery
    trgt_arr[8].max_HP = 70;
    trgt_arr[8].max_Speed = 2;
    trgt_arr[8].max_AttackRange = 2;
    //[9]=>HArtillery
    trgt_arr[9].max_HP = 100;
    trgt_arr[9].max_Speed = 1;
    trgt_arr[9].max_AttackRange = 4;
    //[10]=>destroyer (ship)
    trgt_arr[10].max_HP = 250;
    trgt_arr[10].max_Speed = 3;
    trgt_arr[10].max_AttackRange = 2;

}



