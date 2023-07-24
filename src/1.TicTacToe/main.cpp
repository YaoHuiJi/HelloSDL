#include <SDL.h>
#include <SDL_image.h>
#include <cstdio>

SDL_Window* window;
SDL_Renderer* renderer;

SDL_Texture* sprites;

SDL_Rect xSpriteClip{0,0,32,32};
SDL_Rect oSpriteClip{32,0,32,32};

int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;

int main(int argc, const char** argv) {

    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)<0)
    {
        printf("初始化SDL失败%s\n", SDL_GetError());
    }else{
        window = SDL_CreateWindow("Tic Tac Toe",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);

        if(!window){
            printf("初始化窗口失败%s\n", SDL_GetError());
        }else{
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);

            if(!renderer){
                printf("初始化渲染器失败%s\n", SDL_GetError());
            }else{
                int flags = IMG_INIT_PNG;
                if((IMG_Init(flags)&flags) == 0 ){
                    printf("初始化SLD_image失败%s\n", IMG_GetError());
                }else{
                    SDL_Surface* spriteSheetSurface = IMG_Load("resources/images/tictactoe.png");

                    if(!spriteSheetSurface){
                        printf("加载精灵表失败%s\n", IMG_GetError());
                    }else{
                        sprites = SDL_CreateTextureFromSurface(renderer,spriteSheetSurface);

                        if(!sprites){
                            printf("创建精灵表纹理失败%s\n", SDL_GetError());
                        }else
                        {
                            printf("哈哈，一切都好!开始渲染");        

                            SDL_RenderSetLogicalSize(renderer,SCREEN_WIDTH,SCREEN_HEIGHT);

                            SDL_Event e;

                            bool quit = false;

                            while(!quit){
                                while(SDL_PollEvent(&e)){
                                    if(e.type == SDL_QUIT){
                                        quit = true;
                                    }
                                }

                                SDL_SetRenderDrawColor(renderer, 0xCC, 0xCC, 0xCC, 0xFF);
                                SDL_RenderClear(renderer);

                                int pieceSize = SCREEN_HEIGHT/3;

                                int widthOffset = (SCREEN_WIDTH - SCREEN_HEIGHT) /2;

                                //渲染0，0
                                SDL_Rect pos_0_0{widthOffset,0, pieceSize,pieceSize};
                                SDL_RenderCopyEx(renderer,sprites,&xSpriteClip,&pos_0_0,0,nullptr,SDL_FLIP_NONE);

                                //渲染0，1
                                SDL_Rect pos_0_1{pieceSize*1+widthOffset,0, pieceSize,pieceSize};
                                SDL_RenderCopyEx(renderer,sprites,&oSpriteClip,&pos_0_1,0,nullptr,SDL_FLIP_NONE);

                                //渲染0，2
                                SDL_Rect pos_0_2{pieceSize*2+widthOffset,0, pieceSize,pieceSize};
                                SDL_RenderCopyEx(renderer,sprites,&xSpriteClip,&pos_0_2,0,nullptr,SDL_FLIP_NONE);

                                //渲染1，0
                                SDL_Rect pos_1_0{widthOffset,pieceSize*1, pieceSize,pieceSize};
                                SDL_RenderCopyEx(renderer,sprites,&xSpriteClip,&pos_1_0,0,nullptr,SDL_FLIP_NONE);

                                //渲染1，1
                                SDL_Rect pos_1_1{pieceSize*1+widthOffset,pieceSize*1, pieceSize,pieceSize};
                                SDL_RenderCopyEx(renderer,sprites,&oSpriteClip,&pos_1_1,0,nullptr,SDL_FLIP_NONE);

                                //渲染1，2
                                SDL_Rect pos_1_2{pieceSize*2+widthOffset,pieceSize*1, pieceSize,pieceSize};
                                SDL_RenderCopyEx(renderer,sprites,&xSpriteClip,&pos_1_2,0,nullptr,SDL_FLIP_NONE);

                                //渲染2，0
                                SDL_Rect pos_2_0{widthOffset,pieceSize*2, pieceSize,pieceSize};
                                SDL_RenderCopyEx(renderer,sprites,&xSpriteClip,&pos_2_0,0,nullptr,SDL_FLIP_NONE);

                                //渲染2，1
                                SDL_Rect pos_2_1{pieceSize*1+widthOffset,pieceSize*2, pieceSize,pieceSize};
                                SDL_RenderCopyEx(renderer,sprites,&oSpriteClip,&pos_2_1,0,nullptr,SDL_FLIP_NONE);

                                //渲染2，2
                                SDL_Rect pos_2_2{pieceSize*2+widthOffset,pieceSize*2, pieceSize,pieceSize};
                                SDL_RenderCopyEx(renderer,sprites,&xSpriteClip,&pos_2_2,0,nullptr,SDL_FLIP_NONE);

                                SDL_RenderPresent(renderer);
                            }
                        }
                    }

                    SDL_FreeSurface(spriteSheetSurface);
                }
            }
        }

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_DestroyTexture(sprites);

        renderer = nullptr;
        window = nullptr;
        sprites = nullptr;

        IMG_Quit();
        SDL_Quit();
    }
    

    return 0;
}