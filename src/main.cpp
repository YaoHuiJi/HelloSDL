#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();

bool loadMedia();

void close();

SDL_Window* gWindow = NULL;

//windows所包含的Surface
SDL_Surface* gScreenSurface = NULL;

//用来加载和显示图片的Surface
SDL_Surface* gHelloSDL = NULL;

bool init(){
    bool success = true;

    if( SDL_Init(SDL_INIT_VIDEO) < 0 ){
        printf( "SDL无法初始化! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }else{
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );

        if( gWindow == NULL ){
             printf( "无法创建窗口！SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            gScreenSurface = SDL_GetWindowSurface( gWindow );
        }
    }

    return success;
}

bool loadMedia(){
    bool success = true;

    gHelloSDL = IMG_Load("images/Yao_Logo_1.jpeg");

    if( gHelloSDL == NULL )
    {
        printf( "无法加载图片 %s! SDL Error: %s\n", "images/Yao_Logo_1.jpeg", SDL_GetError() );
        success = false;
    }

    return success;
}

void close(){
    SDL_FreeSurface(gHelloSDL);
    gHelloSDL = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();
}

int main(int argc, char* args[])
{
    if( !init() ){
        printf("初始化失败！\n");
    }else{
        if( !loadMedia() ){
            printf("加载图片失败！\n"); 
        }else{
            SDL_BlitSurface( gHelloSDL, NULL, gScreenSurface, NULL);
        
            SDL_UpdateWindowSurface( gWindow );

            //Hack to get window to stay up
            SDL_Event e; bool quit = false; while( quit == false ){ while( SDL_PollEvent( &e ) ){ if( e.type == SDL_QUIT ) quit = true; } }
        }
    }


    close();

    return 0;
}