//////////////////////////////////////////////////////
//!	KAKO POSTAVITI SDL NA CODE BLOCKS	   !//
//////////////////////////////////////////////////////


Tutorijal koji sam koristio da naucim SDL je 
https://lazyfoo.net/tutorials/SDL/index.php


tutorijal za kako skinuti i postaviti SDL za Code Blocks je:
https://lazyfoo.net/tutorials/SDL/01_hello_SDL/windows/codeblocks/index.php


a za SDL_Image je
https://lazyfoo.net/tutorials/SDL/06_extension_libraries_and_loading_other_image_formats/windows/codeblocks/index.php

(SDL_Image je ekstenzija za SDL koju omogucava ucitavanje .jpg i drugih fajlova u SDL) 


//
UKRATKO TUTORIJAL:
//
1. skinuti SDL
2.Postaviti u direktoriji koja je lagana za pronac
3.kreirati obican projekat u Code Blocks
4.kliknuti na Project->Projects build options->Search directories-> compiler
	(onda ici na "Add" i naci "include" direktoriju od SDL2.0 verzije npr: ".../SDL2.0/include")
4,5.Project->Projects build options->linker (ovdje samo linkovati "lib" direktoriju npr:"SDL2.0/lib")
5.Project->Projects build options->Linker settings-> -lSDL2 -lSDL2main -lmingw32


Svi ovi postupci se ponavljaju i kod SDL_Image-a i isto vam preporucujem da pogledate tutorijal kako postaviti 
SDL_Image jer taj tutorijal cak ima i slike i dobro objasnjenje



//
NEKA GRESKA KOJA SE SAMO NEKAD POJAVI

	To bi trebalo biti to ali opet ja radim na linux-u pa mislim da na windows-u treba ici u main.cpp

#define SDL_MAIN_HANDLED




DODATNO:
	Ako ima nekih problema kod postavljanja SDL-a, preporucujem da procitate ovaj tutorijal koji sam vam linkovo jer je covijek 
	stvarno dobro napisao i ima FAQ na svojoj stranici koji mozete pregledat.
