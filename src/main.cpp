#include <SDL.h>
#include <SDL_image.h>
#include <cstdio>
#include <string>

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();

void close();

void handleEvent(int8_t& imageIndex);

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

SDL_Texture* loadTexture(string path);

SDL_Texture* gTexture = NULL;

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
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );

            if( gRenderer == NULL )
            {
                printf( "Renderer创建失败! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                SDL_SetRenderDrawColor( gRenderer, 0x33, 0x33, 0x33, 0xFF );

                int imgFlags = IMG_INIT_JPG;

                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }
            }
        }
    }

    return success;
}

SDL_Texture* loadTexture(string path){
    SDL_Texture* newTexture = NULL;

    SDL_Surface* loadedFurface = IMG_Load(path.c_str());

    if(loadedFurface == NULL){
        printf("无法加载图片 %s SDL_image Error: %s\n", path.c_str(), SDL_GetError());
    }else{
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedFurface);

        if(newTexture == NULL){
            printf("无法用‘%s’创建贴图！ SDL_image Error: %\n", path.c_str(), SDL_GetError());
        }

        SDL_FreeSurface(loadedFurface);
    }

    return newTexture;
}

bool loadLogo(int8_t& logoIndex){
    bool success = true;

    if(logoIndex>6)
        logoIndex = 1;

    if(logoIndex < 1)
        logoIndex = 6;

    string imageFile = "resources/images/Yao_Logo_" + to_string(logoIndex) + ".jpeg";
    
    gTexture = loadTexture(imageFile);

    if( gTexture == NULL ){
        printf("加载贴图失败!\n");
        success = false;
    }else{

        int w = 0;
        int h = 0;

        SDL_QueryTexture(gTexture,NULL, NULL, &w, &h);
        
        float scale = h / SCREEN_HEIGHT;
        float destWidth = w/scale;
        float offsetX = (SCREEN_WIDTH - destWidth)/2;

        SDL_Rect stretchRect;
				stretchRect.x = offsetX;
				stretchRect.y = 0;
				stretchRect.w = w/scale;
				stretchRect.h = SCREEN_HEIGHT;

        SDL_RenderClear(gRenderer);
        SDL_RenderCopy(gRenderer, gTexture, NULL, &stretchRect);
        SDL_RenderPresent(gRenderer);
    }

    return success;
}

void close(){
    SDL_DestroyTexture(gTexture);
    gTexture = NULL;

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    IMG_Quit();
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