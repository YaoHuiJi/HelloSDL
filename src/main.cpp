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
#include <sstream>

// 是否启用垂直同步(不启用的话使用自定义的fps capping逻辑)
#define Enable_VSync

//Particle count
const int TOTAL_PARTICLES = 20;

class Particle
{
    public:
        //Initialize position and animation
        Particle( int x, int y );

        //Shows the particle
        void render();

        //Checks if particle is dead
        bool isDead();

    private:
        //Offsets
        int mPosX, mPosY;

        //Current frame of animation
        int mFrame;

        //Type of particle
        YEngine::LTexture *mTexture;
};

class LWindow
{
public:
    // Intializes internals
    LWindow();

    // Creates window
    bool init();

    // Creates renderer from internal window
    SDL_Renderer *createRenderer(Uint32 flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // Handles window events
    void handleEvent(SDL_Event &e);

    // Deallocates internals
    void free();

    // Window dimensions
    int getWidth();
    int getHeight();

    // Window focii
    bool hasMouseFocus();
    bool hasKeyboardFocus();
    bool isMinimized();

private:
    // Window data
    SDL_Window *mWindow;

    // Window dimensions
    int mWidth;
    int mHeight;

    // Window focus
    bool mMouseFocus;
    bool mKeyboardFocus;
    bool mFullScreen;
    bool mMinimized;
};

class LTimer
{
public:
    // Initializes variables
    LTimer();

    // The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    // Gets the timer's time
    Uint32 getTicks();

    // Checks the status of the timer
    bool isStarted();
    bool isPaused();

private:
    // The clock time when the timer started
    Uint32 mStartTicks;

    // The ticks stored when the timer was paused
    Uint32 mPausedTicks;

    // The timer status
    bool mPaused;
    bool mStarted;
};

class Dot
{
public:
    // The dimensions of the dot
    static const int DOT_WIDTH = 20;
    static const int DOT_HEIGHT = 20;

    // Maximum axis velocity of the dot
    static const int DOT_VEL = 10;

    // Initializes the variables
    Dot();

    //Deallocates particles
    ~Dot();

    // Takes key presses and adjusts the dot's velocity
    void handleEvent(SDL_Event &e);

    // Moves the dot
    void move();

    // Shows the dot on the screen
    void render();

    // Shows the dot on the screen relative to the camera
    void render(int camX, int camY);

    // Position accessors
    int getPosX();
    int getPosY();

private:
    //The particles
    Particle* particles[ TOTAL_PARTICLES ];

    //Shows the particles
    void renderParticles();
    // The X and Y offsets of the dot
    int mPosX, mPosY;

    // The velocity of the dot
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

class LButton
{
public:
    // Initializes internal variables
    LButton();

    // Sets top left position
    void setPosition(int x, int y);

    // Handles mouse event
    void handleEvent(SDL_Event *e);

    // Shows button sprite
    void render();

private:
    // Top left position
    SDL_Point mPosition;

    // Currently used global sprite
    LButtonSprite mCurrentSprite;
};

const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

bool init();

void close();

// The music that will be played
Mix_Music *gMusic = NULL;

// The sound effects that will be used
Mix_Chunk *gMeow1 = NULL;
Mix_Chunk *gMeow2 = NULL;

LWindow gWindow;
SDL_Renderer *gRenderer = nullptr;
TTF_Font *gFont = NULL;

const int EXPLOSION_ANIMATION_FRAMES = 32;
SDL_Rect gSpriteClips[EXPLOSION_ANIMATION_FRAMES];
YEngine::LTexture gSpriteSheetTexture;
YEngine::LTexture gBackgroundTexture;
YEngine::LTexture gTextTexture;
YEngine::LTexture gFPSTexture;
YEngine::LTexture gTimeTextTexture;

//Scene textures
YEngine::LTexture gDotTexture;
YEngine::LTexture gRedTexture;
YEngine::LTexture gGreenTexture;
YEngine::LTexture gBlueTexture;
YEngine::LTexture gShimmerTexture;

SDL_Rect gButtonSpriteClips[BUTTON_SPRITE_TOTAL];
YEngine::LTexture gButtonSpriteSheetTexture;

LButton gButtons[TOTAL_BUTTONS];

/**
 * @brief 用两种算法显示FPS
 *
 * @param useSDLTicks 是否使用SDL Ticks来计数
 */
void showFPS(bool useSDLTicks, LTimer &fpsTimer, std::stringstream &timeText, int &countedFrames)
{
    if (useSDLTicks)
    {

        float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
        if (avgFPS > 2000000)
        {
            avgFPS = 0;
        }

        timeText.str("");
        timeText << "A.FPS:" << avgFPS;

        // Render text
        if (!gFPSTexture.loadFromRenderedText(timeText.str().data(), SDL_Color{255, 255, 255}))
        {
            printf("Unable to render FPS texture!\n");
        }

        gFPSTexture.render(0, SCREEN_HEIGHT - gFPSTexture.getHeight(), -1);

        ++countedFrames;

        // printf("FRames:%i Secs:%i\n ",countedFrames, fpsTimer.getTicks() / 1000);
    }
    else
    {
        static auto start = std::chrono::high_resolution_clock::now();
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed_us = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        start = end;

        double fps = 1000.0 / elapsed_us;
        std::stringstream stream;
        stream << std::setprecision(2) << fps;

        gFPSTexture.loadFromRenderedText("FPS:" + stream.str(), SDL_Color{255, 255, 255});

        gFPSTexture.render(0, SCREEN_HEIGHT - gFPSTexture.getHeight(), -1);
        // std::cout << "FPS:" << (1000.0/elapsed_us) << std::endl;
    }
}

bool init()
{
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("SDL初始化失败！");
        success = false;
    }
    else
    {
        if (!gWindow.init())
        {
            printf("Window初始化失败！");
            success = false;
        }
        else
        {

#ifdef Enable_VSync
            gRenderer = gWindow.createRenderer();
#else
            gRenderer = SDL_CreateRenderer(SDL_RENDERER_ACCELERATED);
#endif // DEBUG

            if (!gRenderer)
            {
                printf("Renderer初始化失败！");
                success = false;
            }
            else
            {
                //设置一个跟设备无关的逻辑分辨率
                SDL_RenderSetLogicalSize(gRenderer,SCREEN_WIDTH,SCREEN_HEIGHT);

                SDL_SetRenderDrawColor(gRenderer, 0xCC, 0xCC, 0xCC, 0xFF);

                int flags = IMG_INIT_PNG | IMG_INIT_JPG;

                if (!(IMG_Init(flags) & flags))
                {
                    printf("SDL_image初始化失败!%s\n", IMG_GetError());
                    success = false;
                }

                if (TTF_Init() == -1)
                {
                    printf("SDL_ttf初始化失败!%s\n", TTF_GetError());
                    success = false;
                }

                if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
                {
                    printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
                    success = false;
                }
            }
        }
    }

    return success;
}

bool loadFont()
{
    bool success = true;

    gFont = TTF_OpenFont("resources/fonts/IPix.ttf", 28);

    if (!gFont)
    {
        printf("加载字体失败! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    }
    else
    {
        SDL_Color textColor = {255, 255, 255};

        if (!gTextTexture.loadFromRenderedText("你好啊，姚辉季!", textColor))
        {
            printf("渲染文本失败!\n");
            success = false;
        }
    }

    return success;
}

bool loadButtonSprite()
{
    bool success = true;

    // Load sprites
    if (!gButtonSpriteSheetTexture.loadFromFile("resources/images/button.png"))
    {
        printf("加载按钮精灵表失败\n");
        success = false;
    }
    else
    {
        // Set sprites
        for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
        {
            gButtonSpriteClips[i].x = 0;
            gButtonSpriteClips[i].y = i * 200;
            gButtonSpriteClips[i].w = BUTTON_WIDTH;
            gButtonSpriteClips[i].h = BUTTON_HEIGHT;
        }

        gButtons[0].setPosition(0, 0);
        gButtons[1].setPosition(SCREEN_WIDTH - BUTTON_WIDTH, 0);
        gButtons[2].setPosition(0, SCREEN_HEIGHT - BUTTON_HEIGHT);
        gButtons[3].setPosition(SCREEN_WIDTH - BUTTON_WIDTH, SCREEN_HEIGHT - BUTTON_HEIGHT);
    }

    return success;
}

bool loadSound()
{
    // Loading success flag
    bool success = true;

    // Load music
    gMusic = Mix_LoadMUS("resources/sounds/wall_clock.wav");
    if (gMusic == NULL)
    {
        printf("加载音乐失败! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    // Load sound effects
    gMeow1 = Mix_LoadWAV("resources/sounds/meow_1.wav");
    if (gMeow1 == NULL)
    {
        printf("加载音效失败! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    gMeow2 = Mix_LoadWAV("resources/sounds/meow_2.wav");
    if (gMeow2 == NULL)
    {
        printf("加载音效失败! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    return success;
}

bool loadMedia()
{
    bool success = true;

    if (!gSpriteSheetTexture.loadFromFile("resources/images/explosion.png"))
    {
        printf("加载爆炸动画失败!\n");
        success = false;
    }
    else
    {
        gSpriteSheetTexture.setBlendMode(SDL_BLENDMODE_BLEND);

        for (int row = 0; row < 4; row++)
        {
            for (int col = 0; col < 8; col++)
            {
                gSpriteClips[row * 8 + col].x = col * 512;
                gSpriteClips[row * 8 + col].y = row * 512;
                gSpriteClips[row * 8 + col].w = 512;
                gSpriteClips[row * 8 + col].h = 512;
            }
        }

        // Load background texture
        if (!gBackgroundTexture.loadFromFile("resources/images/Yao_Logo_1.jpeg"))
        {
            printf("Failed to load background texture!\n");
            success = false;
        }
    }

    //Load dot texture
    if( !gDotTexture.loadFromFile( "resources/images/dot.bmp",0, 0xFF, 0xFF) )
    {
        printf( "Failed to load dot texture!1\n" );
        success = false;
    }

    //Load red texture
    if( !gRedTexture.loadFromFile( "resources/images/red.bmp" ,0, 0xFF, 0xFF) )
    {
        printf( "Failed to load red texture!2\n" );
        success = false;
    }

    //Load green texture
    if( !gGreenTexture.loadFromFile( "resources/images/green.bmp" ,0, 0xFF, 0xFF) )
    {
        printf( "Failed to load green texture!3\n" );
        success = false;
    }

    //Load blue texture
    if( !gBlueTexture.loadFromFile( "resources/images/blue.bmp" ,0, 0xFF, 0xFF) )
    {
        printf( "Failed to load blue texture!4\n" );
        success = false;
    }

    //Load shimmer texture
    if( !gShimmerTexture.loadFromFile( "resources/images/shimmer.bmp" ,0, 0xFF, 0xFF) )
    {
        printf( "Failed to load shimmer texture!5\n" );
        success = false;
    }
    
    //Set texture transparency
    gRedTexture.setAlpha( 192 );
    gGreenTexture.setAlpha( 192 );
    gBlueTexture.setAlpha( 192 );
    gShimmerTexture.setAlpha( 192 );

    return success;
}

void close()
{

    gDotTexture.free();

    Mix_FreeChunk(gMeow1);
    Mix_FreeChunk(gMeow2);
    gMeow1 = nullptr;
    gMeow2 = nullptr;

    Mix_FreeMusic(gMusic);
    gMusic = nullptr;

    gButtonSpriteSheetTexture.free();

    gTextTexture.free();
    TTF_CloseFont(gFont);
    gFont = NULL;

    gSpriteSheetTexture.free();

    SDL_DestroyRenderer(gRenderer);
    gWindow.free();
    gRenderer = nullptr;

    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char *args[])
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

    if (init() && loadMedia() && loadFont() && loadButtonSprite() && loadSound())
    {

        SDL_Event e;

        bool quit = false;

        Dot dot;

        // The camera area
        SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

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

        while (quit == false)
        {
#ifndef Enable_VSync
            capTimer.start();
#endif

            while (SDL_PollEvent(&e))
            {
                if (e.type == SDL_QUIT)
                    quit = true;
                else if (e.type == SDL_KEYDOWN)
                {
                    Mix_Chunk *meows[] = {gMeow1, gMeow2};

                    switch (e.key.keysym.sym)
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
                        Mix_PlayChannel(-1, meows[rand() % 2], 0);
                        break;

                    case SDLK_9:
                        // If there is no music playing
                        if (Mix_PlayingMusic() == 0)
                        {
                            // Play the music
                            Mix_PlayMusic(gMusic, -1);
                        }
                        // If music is being played
                        else
                        {
                            // If the music is paused
                            if (Mix_PausedMusic() == 1)
                            {
                                // Resume the music
                                Mix_ResumeMusic();
                            }
                            // If the music is playing
                            else
                            {
                                // Pause the music
                                Mix_PauseMusic();
                            }
                        }
                        break;

                    case SDLK_0:
                        // Stop the music
                        Mix_HaltMusic();
                        break;

                    case SDLK_s:
                        if (timer.isStarted())
                        {
                            timer.stop();
                        }
                        else
                        {
                            timer.start();
                        }
                        break;

                    case SDLK_p:
                        if (timer.isPaused())
                        {
                            timer.unpause();
                        }
                        else
                        {
                            timer.pause();
                        }
                        break;

                    case SDLK_y:
                        useLTimerFPS = !useLTimerFPS;
                        break;
                    }
                }

                dot.handleEvent(e);
                gWindow.handleEvent(e);

                // Handle button events
                for (int i = 0; i < TOTAL_BUTTONS; ++i)
                {
                    gButtons[i].handleEvent(&e);
                }
            }

            // 只有在窗口没有最小化时才渲染
            if (!gWindow.isMinimized())
            {

                dot.move();

                // Center the camera over the dot
                camera.x = (dot.getPosX() + Dot::DOT_WIDTH / 2) - SCREEN_WIDTH / 2;
                camera.y = (dot.getPosY() + Dot::DOT_HEIGHT / 2) - SCREEN_HEIGHT / 2;

                // Keep the camera in bounds
                if (camera.x < 0)
                {
                    camera.x = 0;
                }
                if (camera.y < 0)
                {
                    camera.y = 0;
                }
                if (camera.x > LEVEL_WIDTH - camera.w)
                {
                    camera.x = LEVEL_WIDTH - camera.w;
                }
                if (camera.y > LEVEL_HEIGHT - camera.h)
                {
                    camera.y = LEVEL_HEIGHT - camera.h;
                }

                const Uint8 *currentKeyStates = SDL_GetKeyboardState(nullptr);

                if (currentKeyStates[SDL_SCANCODE_T])
                {
                    // 按下键时，此处会被调用很多次
                    useLTimerFPS = !useLTimerFPS;
                }

                SDL_SetRenderDrawColor(gRenderer, 0xCC, 0xCC, 0xCC, 0xFF);
                SDL_RenderClear(gRenderer);

                gBackgroundTexture.setAlpha(255);
                gBackgroundTexture.render(0, 0, &camera);

                // gSpriteSheetTexture.setBlendMode(SDL_BLENDMODE_MUL);
                // gSpriteSheetTexture.setAlpha(128);
                gSpriteSheetTexture.setColor(255, 255, 255);

                SDL_Rect *currentClip = &gSpriteClips[frame / 4];

                const double scale = 0.3;
                const int spriteWidth = 512;

                gSpriteSheetTexture.render(SCREEN_WIDTH / 2 - spriteWidth * scale / 2, SCREEN_HEIGHT / 2 - spriteWidth * scale / 2, currentClip, scale, degrees, nullptr, flipType);

                ++frame;

                if (frame / 4 >= EXPLOSION_ANIMATION_FRAMES)
                {
                    frame = 0;
                }

                // 渲染文本
                gTextTexture.render(0, 0, -1, -1);

                // 渲染buttons
                //  for( int i = 0; i < TOTAL_BUTTONS; ++i )
                //  {
                //      gButtons[ i ].render();
                //  }

                showFPS(useLTimerFPS, fpsTimer, fpsText, countedFrames);

                timeText.str("");
                timeText << "自上次开始的秒数:" << (timer.getTicks() / 1000.f);

                if (!gTimeTextTexture.loadFromRenderedText(timeText.str().c_str(), SDL_Color{255, 255, 255}))
                {
                    printf("无法渲染计数器纹理!\n");
                }
                gTimeTextTexture.render(SCREEN_WIDTH - 330, 0, -1, -1);

                dot.render();
                //dot.render(camera.x, camera.y);

                SDL_RenderPresent(gRenderer);

#ifndef Enable_VSync
                // 如果帧结束的太早则延迟
                int frameTicks = capTimer.getTicks();
                if (frameTicks < SCREEN_TICKS_PER_FRAME)
                {
                    SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
                }
#endif
            }
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

void LButton::setPosition(int x, int y)
{
    mPosition.x = x;
    mPosition.y = y;
}

void LButton::handleEvent(SDL_Event *e)
{
    // If mouse event happened
    if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
    {
        // Get mouse position
        int x, y;
        SDL_GetMouseState(&x, &y);

        // Check if mouse is in button
        bool inside = true;

        // Mouse is left of the button
        if (x < mPosition.x)
        {
            inside = false;
        }
        // Mouse is right of the button
        else if (x > mPosition.x + BUTTON_WIDTH)
        {
            inside = false;
        }
        // Mouse above the button
        else if (y < mPosition.y)
        {
            inside = false;
        }
        // Mouse below the button
        else if (y > mPosition.y + BUTTON_HEIGHT)
        {
            inside = false;
        }
        // Mouse is outside button
        if (!inside)
        {
            mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
        }
        // Mouse is inside button
        else
        {
            // Set mouse over sprite
            switch (e->type)
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
    // Show current button sprite
    gButtonSpriteSheetTexture.render(mPosition.x, mPosition.y, &gButtonSpriteClips[mCurrentSprite]);
}

LTimer::LTimer()
{
    // Initialize the variables
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
    // Stop the timer
    mStarted = false;

    // Unpause the timer
    mPaused = false;

    // Clear tick variables
    mStartTicks = 0;
    mPausedTicks = 0;
}

void LTimer::pause()
{
    // If the timer is running and isn't already paused
    if (mStarted && !mPaused)
    {
        // Pause the timer
        mPaused = true;

        // Calculate the paused ticks
        mPausedTicks = SDL_GetTicks() - mStartTicks;
        mStartTicks = 0;
    }
}

void LTimer::unpause()
{
    // If the timer is running and paused
    if (mStarted && mPaused)
    {
        // Unpause the timer
        mPaused = false;

        // Reset the starting ticks
        mStartTicks = SDL_GetTicks() - mPausedTicks;

        // Reset the paused ticks
        mPausedTicks = 0;
    }
}

Uint32 LTimer::getTicks()
{
    // The actual timer time
    Uint32 time = 0;

    // If the timer is running
    if (mStarted)
    {
        // If the timer is paused
        if (mPaused)
        {
            // Return the number of ticks when the timer was paused
            time = mPausedTicks;
        }
        else
        {
            // Return the current time minus the start time
            time = SDL_GetTicks() - mStartTicks;
        }
    }

    return time;
}

bool LTimer::isStarted()
{
    // Timer is running and paused or unpaused
    return mStarted;
}

bool LTimer::isPaused()
{
    // Timer is running and paused
    return mPaused && mStarted;
}

Dot::Dot()
{
    //Initialize the offsets
    mPosX = SCREEN_WIDTH/2 - DOT_WIDTH/2;
    mPosY = SCREEN_HEIGHT/2 - DOT_HEIGHT/2;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;

    //Initialize particles
    for( int i = 0; i < TOTAL_PARTICLES; ++i )
    {
        particles[ i ] = new Particle( mPosX, mPosY );
    }
}

Dot::~Dot()
{
    //Delete particles
    for( int i = 0; i < TOTAL_PARTICLES; ++i )
    {
        delete particles[ i ];
    }
}

void Dot::handleEvent(SDL_Event &e)
{
    // If a key was pressed
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        // Adjust the velocity
        switch (e.key.keysym.sym)
        {
        case SDLK_UP:
            mVelY -= DOT_VEL;
            break;
        case SDLK_DOWN:
            mVelY += DOT_VEL;
            break;
        case SDLK_LEFT:
            mVelX -= DOT_VEL;
            break;
        case SDLK_RIGHT:
            mVelX += DOT_VEL;
            break;
        }
    }
    // If a key was released
    else if (e.type == SDL_KEYUP && e.key.repeat == 0)
    {
        // Adjust the velocity
        switch (e.key.keysym.sym)
        {
        case SDLK_UP:
            mVelY += DOT_VEL;
            break;
        case SDLK_DOWN:
            mVelY -= DOT_VEL;
            break;
        case SDLK_LEFT:
            mVelX += DOT_VEL;
            break;
        case SDLK_RIGHT:
            mVelX -= DOT_VEL;
            break;
        }
    }
}

void Dot::move()
{
    // Move the dot left or right
    mPosX += mVelX;

    // If the dot went too far to the left or right
    if ((mPosX < 0) || (mPosX + DOT_WIDTH > LEVEL_WIDTH))
    {
        // Move back
        mPosX -= mVelX;
    }
    // Move the dot up or down
    mPosY += mVelY;

    // If the dot went too far up or down
    if ((mPosY < 0) || (mPosY + DOT_HEIGHT > LEVEL_HEIGHT))
    {
        // Move back
        mPosY -= mVelY;
    }
}

void Dot::render()
{
    //Show the dot
    gDotTexture.render( mPosX, mPosY, -1 );

    //Show particles on top of dot
    renderParticles();
}

void Dot::renderParticles()
{
    //Go through particles
    for( int i = 0; i < TOTAL_PARTICLES; ++i )
    {
        //Delete and replace dead particles
        if( particles[ i ]->isDead() )
        {
            delete particles[ i ];
            particles[ i ] = new Particle( mPosX, mPosY );
        }
    }

    //Show particles
    for( int i = 0; i < TOTAL_PARTICLES; ++i )
    {
        particles[ i ]->render();
    }
}

void Dot::render(int camX, int camY)
{
    // Show the dot relative to the camera
    gDotTexture.render(mPosX - camX, mPosY - camY, new SDL_Rect{0, 0, gDotTexture.getWidth(), gDotTexture.getHeight()}, 0.5);
}

int Dot::getPosX()
{
    return mPosX;
}

int Dot::getPosY()
{
    return mPosY;
}

LWindow::LWindow()
{
    mWindow = NULL;
    mMouseFocus = false;
    mKeyboardFocus = false;
    mFullScreen = false;
    mMinimized = false;
    mWidth = 0;
    mHeight = 0;
}

bool LWindow::init()
{
    // Create window
    mWindow = SDL_CreateWindow("Lazyfoo's SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (mWindow != NULL)
    {
        mMouseFocus = true;
        mKeyboardFocus = true;
        mWidth = SCREEN_WIDTH;
        mHeight = SCREEN_HEIGHT;
    }

    return mWindow != NULL;
}

SDL_Renderer *LWindow::createRenderer(Uint32 flags)
{
    return SDL_CreateRenderer(mWindow, -1, flags);
}

void LWindow::handleEvent(SDL_Event &e)
{
    // Window event occured
    if (e.type == SDL_WINDOWEVENT)
    {
        // Caption update flag
        bool updateCaption = false;

        switch (e.window.event)
        {
        // Get new dimensions and repaint on window size change
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            mWidth = e.window.data1;
            mHeight = e.window.data2;
            SDL_RenderPresent(gRenderer);
            break;

        // Repaint on exposure
        case SDL_WINDOWEVENT_EXPOSED:
            SDL_RenderPresent(gRenderer);
            break;

            // Mouse entered window
        case SDL_WINDOWEVENT_ENTER:
            mMouseFocus = true;
            updateCaption = true;
            break;

        // Mouse left window
        case SDL_WINDOWEVENT_LEAVE:
            mMouseFocus = false;
            updateCaption = true;
            break;

        // Window has keyboard focus
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            mKeyboardFocus = true;
            updateCaption = true;
            break;

        // Window lost keyboard focus
        case SDL_WINDOWEVENT_FOCUS_LOST:
            mKeyboardFocus = false;
            updateCaption = true;
            break;

        // Window minimized
        case SDL_WINDOWEVENT_MINIMIZED:
            mMinimized = true;
            break;

        // Window maximized
        case SDL_WINDOWEVENT_MAXIMIZED:
            mMinimized = false;
            break;

        // Window restored
        case SDL_WINDOWEVENT_RESTORED:
            mMinimized = false;
            break;
        }

        // Update window caption with new data
        if (updateCaption)
        {
            std::stringstream caption;
            caption << "Lazyfoo's SDL Tutorial - MouseFocus:" << ((mMouseFocus) ? "On" : "Off") << " KeyboardFocus:" << ((mKeyboardFocus) ? "On" : "Off");
            SDL_SetWindowTitle(mWindow, caption.str().c_str());
        }
    }
    // Enter exit full screen on return key
    else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN)
    {
        if (mFullScreen)
        {
            SDL_SetWindowFullscreen(mWindow, SDL_FALSE);
            mFullScreen = false;
        }
        else
        {
            SDL_SetWindowFullscreen(mWindow, SDL_TRUE);
            mFullScreen = true;
            mMinimized = false;
        }
    }
}

void LWindow::free()
{
	if( mWindow != NULL )
	{
		SDL_DestroyWindow( mWindow );
	}

	mMouseFocus = false;
	mKeyboardFocus = false;
	mWidth = 0;
	mHeight = 0;
}

int LWindow::getWidth()
{
    return mWidth;
}

int LWindow::getHeight()
{
    return mHeight;
}

bool LWindow::hasMouseFocus()
{
    return mMouseFocus;
}

bool LWindow::hasKeyboardFocus()
{
    return mKeyboardFocus;
}

bool LWindow::isMinimized()
{
    return mMinimized;
}

Particle::Particle( int x, int y )
{
    //Set offsets
    mPosX = x - 5 + ( rand() % 25 );
    mPosY = y - 5 + ( rand() % 25 );

    //Initialize animation
    mFrame = rand() % 5;

    //Set type
    switch( rand() % 3 )
    {
        case 0: mTexture = &gRedTexture; break;
        case 1: mTexture = &gGreenTexture; break;
        case 2: mTexture = &gBlueTexture; break;
    }
}

void Particle::render()
{
    //Show image
    mTexture->render( mPosX, mPosY, -1 );

    //Show shimmer
    if( mFrame % 2 == 0 )
    {
        gShimmerTexture.render( mPosX, mPosY , -1);
    }

    //Animate
    mFrame++;
}

bool Particle::isDead()
{
    return mFrame > 10;
}