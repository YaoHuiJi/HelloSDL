#include <SDL.h>
#include <SDL_image.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include "LTexture.hpp"
#include <iostream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();

void close();

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;

YEngine::LTexture gFooTexture;
YEngine::LTexture gBackgroundTexture;

bool init(){
    bool success = true;

    if(SDL_Init(SDL_INIT_VIDEO)<0){
        printf("SDL初始化失败！");
        success = false;
    }else{
        gWindow = SDL_CreateWindow("你好",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);

        if(!gWindow){
            printf("Window初始化失败！");
            success = false;
        }else{
            gRenderer = SDL_CreateRenderer(gWindow,-1,SDL_RENDERER_ACCELERATED);

            if(!gRenderer){
                printf("Renderer初始化失败！");
                success = false;
            }else{
                SDL_SetRenderDrawColor(gRenderer,0xCC,0xCC,0xCC,0xFF);

                if(!(IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG) ){
                    printf("SDL_image初始化失败!%s\n", IMG_GetError());
                    success = false;
                }
            }
        }
    }

    return success;
}

bool loadMedia()
{
    bool success = true;

    if( !gFooTexture.loadFromFile( "resources/images/logo.jpg" ) )
    {
        printf( "Failed to load Foo' texture image!\n" );
        success = false;
    }
    
    //Load background texture
    if( !gBackgroundTexture.loadFromFile( "resources/images/logo.jpg" ) )
    {
        printf( "Failed to load background texture image!\n" );
        success = false;
    }

    return success;
}

void close(){

    gFooTexture.free();
    gBackgroundTexture.free();

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;
    gRenderer = nullptr;

    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* args[])
{
    int x = 0;
    int y = 0;

    if(init()&&loadMedia()){
        SDL_Event e; 
        bool quit = false; 
        
        while( quit == false ){ 
            while( SDL_PollEvent( &e ) )
            { 
                if( e.type == SDL_QUIT ) quit = true; 
                else if (e.type== SDL_KEYDOWN)
                {
                    if(e.key.keysym.sym == SDLK_r){
                        x = rand()%SCREEN_WIDTH+1;
                        y = rand()%SCREEN_HEIGHT+1;
                    }
                }
                
            } 
            SDL_SetRenderDrawColor( gRenderer, 0xCC, 0xCC, 0xCC, 0xFF );
            SDL_RenderClear(gRenderer);

            gBackgroundTexture.render(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);

            int w,h;
            SDL_QueryTexture(gFooTexture.getTexture(),nullptr,nullptr,&w,&h);
            gFooTexture.render(60,60,w,h);

            SDL_RenderPresent(gRenderer);
        }
    }

    close();
    
    return 0;
}