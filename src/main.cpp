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

void drawARectangle(int x=SCREEN_WIDTH/4, int y=SCREEN_HEIGHT/4, int w=SCREEN_WIDTH/2, int h=SCREEN_HEIGHT/2);

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
                SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );

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

        SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
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

void drawARectangle(int x, int y, int w, int h){
    SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
    SDL_RenderClear(gRenderer);

    SDL_SetRenderDrawColor(gRenderer,0xF1,0xA0,0x50,0xFF);
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderFillRect(gRenderer,&rect);

    SDL_Rect outlineRect = { SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3 };
    SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0x00, 0xFF );        
    SDL_RenderDrawRect( gRenderer, &outlineRect );

    SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0xFF );
    SDL_RenderDrawLine( gRenderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2 );

    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0x00, 0xFF );
    
    for( int i = 0; i < SCREEN_HEIGHT; i += 4 )
    {
        SDL_RenderDrawPoint( gRenderer, SCREEN_WIDTH / 2, i );
    }

    SDL_RenderPresent(gRenderer);
}

void drawBezierCurve(SDL_Renderer* renderer, SDL_Point p0, SDL_Point p1, SDL_Point p2, SDL_Point p3, float step)
{
    SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
    SDL_RenderClear(gRenderer);

    SDL_SetRenderDrawColor(renderer, 0xFF, 00, 0, 255);
    SDL_RenderDrawPoint(renderer, p0.x, p0.y);

    SDL_SetRenderDrawColor(renderer, 0xFF, 0x20, 0, 255);
    SDL_RenderDrawPoint(renderer, p1.x, p1.y);

    SDL_SetRenderDrawColor(renderer, 0xFF, 0x30, 0, 255);
    SDL_RenderDrawPoint(renderer, p2.x, p2.y);

    SDL_SetRenderDrawColor(renderer, 0xFF, 0x40, 0, 255);
    SDL_RenderDrawPoint(renderer, p3.x, p3.y);

    float t = 0.0f;
    while (t <= 1.0f)
    {
        // 计算贝塞尔曲线上的点
        float u = 1.0f - t;
        float tt = t * t;
        float uu = u * u;
        float uuu = uu * u;
        float ttt = tt * t;
        SDL_Point p = {
            static_cast<int>(uuu * p0.x + 3 * uu * t * p1.x + 3 * u * tt * p2.x + ttt * p3.x),
            static_cast<int>(uuu * p0.y + 3 * uu * t * p1.y + 3 * u * tt * p2.y + ttt * p3.y)
        };

        // 绘制贝塞尔曲线上的点
        SDL_SetRenderDrawColor(renderer, 123, 125, 0, 255);
        SDL_RenderDrawPoint(renderer, p.x, p.y);

        // 增加步长
        t += step;
    }

     SDL_RenderPresent(gRenderer);
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
                    case SDLK_r:
                        {
                            drawARectangle();
                        }
                        break;
                    case SDLK_b:
                        {
                            SDL_Point p0 = { 100, 100 };
                            SDL_Point p1 = { 200, 50 };
                            SDL_Point p2 = { 300, 200 };
                            SDL_Point p3 = { 400, 100 };
                            float step = 0.001f;
                            drawBezierCurve(gRenderer, p0, p1, p2, p3, step);
                        }
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