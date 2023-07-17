#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include "LTexture.hpp"
#include <iostream>
#include <iomanip>
#include <chrono>

const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 4;

enum LButtonSprite
{
    BUTTON_SPRITE_MOUSE_OUT = 0,
    BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
    BUTTON_SPRITE_MOUSE_DOWN = 2,
    BUTTON_SPRITE_MOUSE_UP = 3,
    BUTTON_SPRITE_TOTAL = 4
};

class LButton{
    public:
        //Initializes internal variables
        LButton();

        //Sets top left position
        void setPosition( int x, int y );

        //Handles mouse event
        void handleEvent( SDL_Event* e );
    
        //Shows button sprite
        void render();

    private:
        //Top left position
        SDL_Point mPosition;

        //Currently used global sprite
        LButtonSprite mCurrentSprite;
};


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();

void close();

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
TTF_Font* gFont = NULL;

const int EXPLOSION_ANIMATION_FRAMES = 32;
SDL_Rect gSpriteClips[ EXPLOSION_ANIMATION_FRAMES ];
YEngine::LTexture gSpriteSheetTexture;
YEngine::LTexture gBackgroundTexture;
YEngine::LTexture gTextTexture;
YEngine::LTexture gFPSTexture;

SDL_Rect gButtonSpriteClips[ BUTTON_SPRITE_TOTAL ];
YEngine::LTexture gButtonSpriteSheetTexture;

LButton gButtons[ TOTAL_BUTTONS ]; 

void showFPS(){
    static auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed_us = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    start = end;

    double fps = 1000.0/elapsed_us;
    std::stringstream stream;
    stream << std::setprecision(2) << fps;

    gFPSTexture.loadFromRenderedText("FPS:" + stream.str(),SDL_Color{255,255,255});

    gFPSTexture.render(0,SCREEN_HEIGHT-30,-1);
    //std::cout << "FPS:" << (1000.0/elapsed_us) << std::endl;
}

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
            gRenderer = SDL_CreateRenderer(gWindow,-1,SDL_RENDERER_ACCELERATED| SDL_RENDERER_PRESENTVSYNC);

            if(!gRenderer){
                printf("Renderer初始化失败！");
                success = false;
            }else{
                SDL_SetRenderDrawColor(gRenderer,0xCC,0xCC,0xCC,0xFF);

                int flags = IMG_INIT_PNG | IMG_INIT_JPG;

                if(!(IMG_Init(flags) & flags) ){
                    printf("SDL_image初始化失败!%s\n", IMG_GetError());
                    success = false;
                }

                if(TTF_Init()== -1){
                    printf("SDL_ttf初始化失败!%s\n", TTF_GetError());
                    success = false;
                }
            }
        }
    }

    return success;
}

bool loadFont(){
    bool success = true;

    gFont = TTF_OpenFont("resources/fonts/IPix.ttf",28);

    if(!gFont){
        printf( "加载字体失败! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }else
    {
        SDL_Color textColor = {255,255,255};

        if(!gTextTexture.loadFromRenderedText("你好啊，姚辉季!",textColor)){
            printf("渲染文本失败!\n");
            success = false;
        }
    }

    return success;
}

bool loadButtonSprite()
{
	bool success = true;

	//Load sprites
	if( !gButtonSpriteSheetTexture.loadFromFile( "resources/images/button.png" ) )
	{
		printf( "加载按钮精灵表失败\n" );
		success = false;
	}
	else
	{
		//Set sprites
		for( int i = 0; i < BUTTON_SPRITE_TOTAL; ++i )
		{
			gSpriteClips[ i ].x = 0;
			gSpriteClips[ i ].y = i * 200;
			gSpriteClips[ i ].w = BUTTON_WIDTH;
			gSpriteClips[ i ].h = BUTTON_HEIGHT;
		}

		//Set buttons in corners
		gButtons[ 0 ].setPosition( 0, 0 );
		gButtons[ 1 ].setPosition( SCREEN_WIDTH - BUTTON_WIDTH, 0 );
		gButtons[ 2 ].setPosition( 0, SCREEN_HEIGHT - BUTTON_HEIGHT );
		gButtons[ 3 ].setPosition( SCREEN_WIDTH - BUTTON_WIDTH, SCREEN_HEIGHT - BUTTON_HEIGHT );
	}

	return success;
}

bool loadMedia()
{
    bool success = true;

    if( !gSpriteSheetTexture.loadFromFile( "resources/images/explosion.png" ) )
    {
        printf( "加载爆炸动画失败!\n" );
        success = false;
    }else{
        gSpriteSheetTexture.setBlendMode(SDL_BLENDMODE_BLEND);
        
        for(int row = 0; row < 4; row++){
            for(int col = 0; col < 8; col++){
                gSpriteClips[ row*8+col ].x =   col*512;
                gSpriteClips[ row*8+col ].y =   row*512;
                gSpriteClips[ row*8+col ].w = 512;
                gSpriteClips[ row*8+col ].h = 512;
            }
        }

        //Load background texture
        if( !gBackgroundTexture.loadFromFile( "resources/images/Yao_Logo_1.jpeg" ) )
        {
            printf( "Failed to load background texture!\n" );
            success = false;
        }
    }

    return success;
}

void close(){
    gButtonSpriteSheetTexture.free();

    gTextTexture.free();
    TTF_CloseFont( gFont );
    gFont = NULL;

    gSpriteSheetTexture.free();

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;
    gRenderer = nullptr;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* args[])
{
    Uint8 r = 255;
    Uint8 g = 255;
    Uint8 b = 255;
    int frame = 0;

    if(init()&&loadMedia()&&loadFont()&&loadButtonSprite()){
        SDL_Event e; 
        bool quit = false; 

        double degrees = 0;

        SDL_RendererFlip flipType = SDL_FLIP_NONE;
        
        while( quit == false ){ 
            while( SDL_PollEvent( &e ) )
            { 
                if( e.type == SDL_QUIT ) quit = true; 
                else if (e.type== SDL_KEYDOWN)
                {
                     switch( e.key.keysym.sym )
                        {
                            case SDLK_a:
                            degrees -= 60;
                            break;
                            
                            case SDLK_d:
                            degrees += 60;
                            break;

                            case SDLK_q:
                            flipType = SDL_FLIP_HORIZONTAL;
                            break;

                            case SDLK_w:
                            flipType = SDL_FLIP_NONE;
                            break;

                            case SDLK_e:
                            flipType = SDL_FLIP_VERTICAL;
                            break;
                            case SDLK_UP:
                            frame+=1;
                            if(frame>=EXPLOSION_ANIMATION_FRAMES){
                                frame = 0;
                            }
                            break;
                            
                            case SDLK_DOWN:
                            frame-=1;
                            if(frame<0){
                                frame = EXPLOSION_ANIMATION_FRAMES-1;
                            }
                            break;
                        }
                }
                
                //Handle button events
                for( int i = 0; i < TOTAL_BUTTONS; ++i )
                {
                    gButtons[ i ].handleEvent( &e );
                }
            } 
            SDL_SetRenderDrawColor( gRenderer, 0xCC, 0xCC, 0xCC, 0xFF );
            SDL_RenderClear(gRenderer);

            gBackgroundTexture.setAlpha(255);
            gBackgroundTexture.render(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);

            //gSpriteSheetTexture.setBlendMode(SDL_BLENDMODE_MUL);
            //gSpriteSheetTexture.setAlpha(128);
            gSpriteSheetTexture.setColor( 255, 255, 255 );

            SDL_Rect* currentClip = &gSpriteClips[ frame / 4 ];

            const double scale = 0.3;
            const int spriteWidth = 512;

            gSpriteSheetTexture.render(SCREEN_WIDTH/2-spriteWidth*scale/2,SCREEN_HEIGHT/2-spriteWidth*scale/2, currentClip, scale, degrees,nullptr,flipType);
            
            ++frame;

            if(frame/4 >= EXPLOSION_ANIMATION_FRAMES){
                frame = 0;
            }

            //渲染文本
            gTextTexture.render(0,0,-1,-1);

            //渲染buttons
            for( int i = 0; i < TOTAL_BUTTONS; ++i )
            {
                gButtons[ i ].render();
            }

            showFPS();

            SDL_RenderPresent(gRenderer);
        }
    }

    close();
    
    return 0;
}

LButton::LButton()
{
    mPosition.x = 0;
    mPosition.y = 0;

    mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void LButton::setPosition( int x, int y )
{
    mPosition.x = x;
    mPosition.y = y;
}

void LButton::handleEvent( SDL_Event* e )
{
    //If mouse event happened
    if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
    {
        //Get mouse position
        int x, y;
        SDL_GetMouseState( &x, &y );

        //Check if mouse is in button
        bool inside = true;

        //Mouse is left of the button
        if( x < mPosition.x )
        {
            inside = false;
        }
        //Mouse is right of the button
        else if( x > mPosition.x + BUTTON_WIDTH )
        {
            inside = false;
        }
        //Mouse above the button
        else if( y < mPosition.y )
        {
            inside = false;
        }
        //Mouse below the button
        else if( y > mPosition.y + BUTTON_HEIGHT )
        {
            inside = false;
        }
        //Mouse is outside button
        if( !inside )
        {
            mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
        }
        //Mouse is inside button
        else
        {
            //Set mouse over sprite
            switch( e->type )
            {
                case SDL_MOUSEMOTION:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
                break;
            
                case SDL_MOUSEBUTTONDOWN:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
                break;
                
                case SDL_MOUSEBUTTONUP:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
                break;
            }
        }
    }
}

void LButton::render()
{
    //Show current button sprite
    gButtonSpriteSheetTexture.render( mPosition.x, mPosition.y, &gSpriteClips[ mCurrentSprite ] );
}