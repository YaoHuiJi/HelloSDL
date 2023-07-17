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

//是否启用垂直同步(不启用的话使用自定义的fps capping逻辑)
#define Enable_VSync

class LTimer
{
    public:
        //Initializes variables
        LTimer();

        //The various clock actions
        void start();
        void stop();
        void pause();
        void unpause();

        //Gets the timer's time
        Uint32 getTicks();

        //Checks the status of the timer
        bool isStarted();
        bool isPaused();

    private:
        //The clock time when the timer started
        Uint32 mStartTicks;

        //The ticks stored when the timer was paused
        Uint32 mPausedTicks;

        //The timer status
        bool mPaused;
        bool mStarted;
};

class Dot
{
    public:
        //The dimensions of the dot
        static const int DOT_WIDTH = 20;
        static const int DOT_HEIGHT = 20;

        //Maximum axis velocity of the dot
        static const int DOT_VEL = 4;

        //Initializes the variables
        Dot();

        //Takes key presses and adjusts the dot's velocity
        void handleEvent( SDL_Event& e );

        //Moves the dot
        void move();

        //Shows the dot on the screen
        void render();

    private:
        //The X and Y offsets of the dot
        int mPosX, mPosY;

        //The velocity of the dot
        int mVelX, mVelY;
};

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
const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

bool init();

void close();

//The music that will be played
Mix_Music* gMusic = NULL;

//The sound effects that will be used
Mix_Chunk* gMeow1 = NULL;
Mix_Chunk* gMeow2 = NULL;

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
TTF_Font* gFont = NULL;

const int EXPLOSION_ANIMATION_FRAMES = 32;
SDL_Rect gSpriteClips[ EXPLOSION_ANIMATION_FRAMES ];
YEngine::LTexture gSpriteSheetTexture;
YEngine::LTexture gBackgroundTexture;
YEngine::LTexture gTextTexture;
YEngine::LTexture gFPSTexture;
YEngine::LTexture gTimeTextTexture;
YEngine::LTexture gDotTexture;

SDL_Rect gButtonSpriteClips[ BUTTON_SPRITE_TOTAL ];
YEngine::LTexture gButtonSpriteSheetTexture;

LButton gButtons[ TOTAL_BUTTONS ]; 


/**
 * @brief 用两种算法显示FPS
 * 
 * @param useSDLTicks 是否使用SDL Ticks来计数
 */
void showFPS(bool useSDLTicks, LTimer& fpsTimer, std::stringstream& timeText, int& countedFrames){
    if(useSDLTicks){

        float avgFPS = countedFrames / ( fpsTimer.getTicks() / 1000.f );
        if( avgFPS > 2000000 )
        {
            avgFPS = 0;
        }

        timeText.str( "" );
        timeText << "A.FPS:" << avgFPS; 

        //Render text
        if( !gFPSTexture.loadFromRenderedText( timeText.str().data(), SDL_Color{255,255,255} ) )
        {
            printf( "Unable to render FPS texture!\n" );
        }

        gFPSTexture.render(0,SCREEN_HEIGHT-gFPSTexture.getHeight(),-1);

        ++countedFrames;

        // printf("FRames:%i Secs:%i\n ",countedFrames, fpsTimer.getTicks() / 1000);

    }else{
        static auto start = std::chrono::high_resolution_clock::now();
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed_us = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        start = end;

        double fps = 1000.0/elapsed_us;
        std::stringstream stream;
        stream << std::setprecision(2) << fps;

        gFPSTexture.loadFromRenderedText("FPS:" + stream.str(),SDL_Color{255,255,255});

        gFPSTexture.render(0,SCREEN_HEIGHT-gFPSTexture.getHeight(),-1);
        //std::cout << "FPS:" << (1000.0/elapsed_us) << std::endl;
    }
}

bool init(){
    bool success = true;

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)<0){
        printf("SDL初始化失败！");
        success = false;
    }else{
        gWindow = SDL_CreateWindow("你好",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);

        if(!gWindow){
            printf("Window初始化失败！");
            success = false;
        }else{
            
            #ifdef Enable_VSync
            gRenderer = SDL_CreateRenderer(gWindow,-1,SDL_RENDERER_ACCELERATED| SDL_RENDERER_PRESENTVSYNC);
            #else
            gRenderer = SDL_CreateRenderer(gWindow,-1,SDL_RENDERER_ACCELERATED);
            #endif // DEBUG

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

                if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
                {
                    printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
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
			gButtonSpriteClips[ i ].x = 0;
			gButtonSpriteClips[ i ].y = i * 200;
			gButtonSpriteClips[ i ].w = BUTTON_WIDTH;
			gButtonSpriteClips[ i ].h = BUTTON_HEIGHT;
		}

		gButtons[ 0 ].setPosition( 0, 0 );
		gButtons[ 1 ].setPosition( SCREEN_WIDTH - BUTTON_WIDTH, 0 );
		gButtons[ 2 ].setPosition( 0, SCREEN_HEIGHT - BUTTON_HEIGHT );
		gButtons[ 3 ].setPosition( SCREEN_WIDTH - BUTTON_WIDTH, SCREEN_HEIGHT - BUTTON_HEIGHT );
	}

	return success;
}

bool loadSound()
{
    //Loading success flag
    bool success = true;

    //Load music
    gMusic = Mix_LoadMUS( "resources/sounds/wall_clock.wav" );
    if( gMusic == NULL )
    {
        printf( "加载音乐失败! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    
    //Load sound effects
    gMeow1 = Mix_LoadWAV( "resources/sounds/meow_1.wav" );
    if( gMeow1 == NULL )
    {
        printf( "加载音效失败! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    
    gMeow2 = Mix_LoadWAV( "resources/sounds/meow_2.wav" );
    if( gMeow2 == NULL )
    {
        printf( "加载音效失败! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
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

    //Load dot texture
	if( !gDotTexture.loadFromFile( "resources/images/logo.png" ) )
	{
		printf( "Failed to load dot texture!\n" );
		success = false;
	}

    return success;
}

void close(){

    gDotTexture.free();

    Mix_FreeChunk(gMeow1);
    Mix_FreeChunk(gMeow2);
    gMeow1 = nullptr;
    gMeow2 = nullptr;

    Mix_FreeMusic(gMusic);
    gMusic = nullptr;

    gButtonSpriteSheetTexture.free();

    gTextTexture.free();
    TTF_CloseFont( gFont );
    gFont = NULL;

    gSpriteSheetTexture.free();

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;
    gRenderer = nullptr;

    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* args[])
{
    #ifdef Enable_VSync
    printf("启用垂直同步\n");
    #else
    printf("没有启用垂直同步\n");
    #endif
    Uint8 r = 255;
    Uint8 g = 255;
    Uint8 b = 255;
    int frame = 0;

    if(init()&&loadMedia()&&loadFont()&&loadButtonSprite()&&loadSound()){
        
        SDL_Event e; 
        
        bool quit = false; 

        Dot dot;

        bool useLTimerFPS = false;
        LTimer fpsTimer;
        std::stringstream fpsText;
        int countedFrames = 0;
        fpsTimer.start();

        #ifndef Enable_VSync
        LTimer capTimer;
        #endif

        LTimer timer;
        std::stringstream timeText;

        double degrees = 0;

        SDL_RendererFlip flipType = SDL_FLIP_NONE;
        
        while( quit == false ){ 
            #ifndef Enable_VSync
            capTimer.start();
            #endif
            
            while( SDL_PollEvent( &e ) )
            { 
                if( e.type == SDL_QUIT ) quit = true; 
                else if (e.type== SDL_KEYDOWN)
                {
                    Mix_Chunk* meows[] = {gMeow1,gMeow2}; 

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

                            case SDLK_8:
                            Mix_PlayChannel( -1, meows[rand()%2], 0 );
                            break;

                            case SDLK_9:
                            //If there is no music playing
                            if( Mix_PlayingMusic() == 0 )
                            {
                                //Play the music
                                Mix_PlayMusic( gMusic, -1 );
                            }
                            //If music is being played
                            else
                            {
                                //If the music is paused
                                if( Mix_PausedMusic() == 1 )
                                {
                                    //Resume the music
                                    Mix_ResumeMusic();
                                }
                                //If the music is playing
                                else
                                {
                                    //Pause the music
                                    Mix_PauseMusic();
                                }
                            }
                            break;
                            
                            case SDLK_0:
                            //Stop the music
                            Mix_HaltMusic();
                            break;
                            
                            case SDLK_s:
                            if(timer.isStarted()){
                                timer.stop();
                            }else{
                                timer.start();
                            }
                            break;

                            case SDLK_p:
                            if(timer.isPaused()){
                                timer.unpause();
                            }else{
                                timer.pause();
                            }
                            break;

                            case SDLK_y:
                            useLTimerFPS = ! useLTimerFPS;
                            break;
                        }
                }

                dot.handleEvent(e);
                
                //Handle button events
                for( int i = 0; i < TOTAL_BUTTONS; ++i )
                {
                    gButtons[ i ].handleEvent( &e );
                }
            } 

            dot.move();

            const Uint8* currentKeyStates = SDL_GetKeyboardState( nullptr );

            if(currentKeyStates[SDL_SCANCODE_T]){
                //按下键时，此处会被调用很多次
                useLTimerFPS = ! useLTimerFPS;
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
            // for( int i = 0; i < TOTAL_BUTTONS; ++i )
            // {
            //     gButtons[ i ].render();
            // }

            showFPS(useLTimerFPS,fpsTimer,fpsText,countedFrames);

            timeText.str("");
            timeText << "自上次开始的秒数:" << (timer.getTicks()/1000.f);

            if( !gTimeTextTexture.loadFromRenderedText( timeText.str().c_str(),SDL_Color{255,255,255}))
            {
                printf( "无法渲染计数器纹理!\n" );
            }
            gTimeTextTexture.render(SCREEN_WIDTH-330,0,-1,-1);

            dot.render();

            SDL_RenderPresent(gRenderer);

            #ifndef Enable_VSync
            //如果帧结束的太早则延迟
            int frameTicks = capTimer.getTicks();
            if( frameTicks < SCREEN_TICKS_PER_FRAME )
            {
                SDL_Delay( SCREEN_TICKS_PER_FRAME - frameTicks );
            }
            #endif
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
    gButtonSpriteSheetTexture.setAlpha(60);
    //Show current button sprite
    gButtonSpriteSheetTexture.render( mPosition.x, mPosition.y, &gButtonSpriteClips[ mCurrentSprite ] );
}

LTimer::LTimer()
{
    //Initialize the variables
    mStartTicks = 0;
    mPausedTicks = 0;

    mPaused = false;
    mStarted = false;
}

void LTimer::start()
{
    mStarted = true;

    mPaused = false;

    mStartTicks = SDL_GetTicks();
    mPausedTicks = 0;
}

void LTimer::stop()
{
    //Stop the timer
    mStarted = false;

    //Unpause the timer
    mPaused = false;

    //Clear tick variables
    mStartTicks = 0;
    mPausedTicks = 0;
}

void LTimer::pause()
{
    //If the timer is running and isn't already paused
    if( mStarted && !mPaused )
    {
        //Pause the timer
        mPaused = true;

        //Calculate the paused ticks
        mPausedTicks = SDL_GetTicks() - mStartTicks;
        mStartTicks = 0;
    }
}

void LTimer::unpause()
{
    //If the timer is running and paused
    if( mStarted && mPaused )
    {
        //Unpause the timer
        mPaused = false;

        //Reset the starting ticks
        mStartTicks = SDL_GetTicks() - mPausedTicks;

        //Reset the paused ticks
        mPausedTicks = 0;
    }
}

Uint32 LTimer::getTicks()
{
    //The actual timer time
    Uint32 time = 0;

    //If the timer is running
    if( mStarted )
    {
        //If the timer is paused
        if( mPaused )
        {
            //Return the number of ticks when the timer was paused
            time = mPausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            time = SDL_GetTicks() - mStartTicks;
        }
    }

    return time;
}

bool LTimer::isStarted()
{
    //Timer is running and paused or unpaused
    return mStarted;
}

bool LTimer::isPaused()
{
    //Timer is running and paused
    return mPaused && mStarted;
}

Dot::Dot()
{
    //Initialize the offsets
    mPosX = 0;
    mPosY = 0;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;
}

void Dot::handleEvent( SDL_Event& e )
{
    //If a key was pressed
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY -= DOT_VEL; break;
            case SDLK_DOWN: mVelY += DOT_VEL; break;
            case SDLK_LEFT: mVelX -= DOT_VEL; break;
            case SDLK_RIGHT: mVelX += DOT_VEL; break;
        }
    }
     //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY += DOT_VEL; break;
            case SDLK_DOWN: mVelY -= DOT_VEL; break;
            case SDLK_LEFT: mVelX += DOT_VEL; break;
            case SDLK_RIGHT: mVelX -= DOT_VEL; break;
        }
    }
}

void Dot::move()
{
    //Move the dot left or right
    mPosX += mVelX;

    //If the dot went too far to the left or right
    if( ( mPosX < 0 ) || ( mPosX + DOT_WIDTH > SCREEN_WIDTH ) )
    {
        //Move back
        mPosX -= mVelX;
    }
    //Move the dot up or down
    mPosY += mVelY;

    //If the dot went too far up or down
    if( ( mPosY < 0 ) || ( mPosY + DOT_HEIGHT > SCREEN_HEIGHT ) )
    {
        //Move back
        mPosY -= mVelY;
    }
}

void Dot::render()
{
    //Show the dot
    gDotTexture.render( mPosX, mPosY,new SDL_Rect{0,0,gDotTexture.getWidth(),gDotTexture.getHeight()},0.5 );
}
