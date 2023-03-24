#include <SDL.h>
#include <SDL_image.h>
#include <cstdio>
#include <string>

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();

bool loadMedia();

void close();

void handleEvent();

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

bool loadMedia(const char* imageFile){
    bool success = true;

    gHelloSDL = IMG_Load(imageFile);

    if( gHelloSDL == NULL )
    {
        printf( "无法加载图片 %s! SDL Error: %s\n", imageFile, SDL_GetError() );
        success = false;
    }

    return success;
}

bool loadLogo(int8_t& logoIndex){

    if(logoIndex>6)
        logoIndex = 1;

    if(logoIndex < 1)
        logoIndex = 6;

    string imageFile = "resources/images/Yao_Logo_" + to_string(logoIndex) + ".jpeg";
    
    bool result = loadMedia(imageFile.c_str());

    if(result){
        SDL_BlitSurface( gHelloSDL, NULL, gScreenSurface, NULL);
        SDL_UpdateWindowSurface( gWindow );
    }else{
        printf( "加载图片失败: %s\n" , SDL_GetError() );
    }

    return result;
}

void close(){
    SDL_FreeSurface(gHelloSDL);
    gHelloSDL = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();
}

void handleEvent(int8_t& imageIndex){
    SDL_Event e;
    bool quit = false; 
            
    while( quit == false ){ 
        while( SDL_PollEvent( &e ) ){ 
            if( e.type == SDL_QUIT ) quit = true; 

            if( e.type == SDL_KEYDOWN){
                // printf("按下按键: %d\n", e.key.keysym.sym);
                switch (e.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                case SDLK_EQUALS:
                    loadLogo(++imageIndex);
                    break;
                case SDLK_MINUS:
                    loadLogo(--imageIndex);
                    break;
                default:
                    break;
                }
            }
        } 
    }
}

int main(int argc, char* args[])
{
    int8_t imageIndex = 1;

    if(init()){
        if(loadLogo(imageIndex)){
            handleEvent(imageIndex);
        }
    }else{
        printf("初始化失败！\n");
    }

    close();

    return 0;
}