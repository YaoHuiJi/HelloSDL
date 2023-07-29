/**
 * @file main.cpp
 * @author Yao
 * @brief 
 * @version 0.1
 * @date 2023-07-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <SDL.h>
#include <SDL_image.h>
#include <cstdio>

SDL_Window* window;
SDL_Renderer* renderer;

SDL_Texture* sprites;

SDL_Rect xSpriteClip{0, 0, 32, 32};
SDL_Rect oSpriteClip{32, 0, 32, 32};

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int Piece_Size = SCREEN_HEIGHT/3;
const int Width_Offset = (SCREEN_WIDTH - SCREEN_HEIGHT) /2;

enum class PieceType {
    None,
    X,
    O,
};

class Piece {
 public:
    Piece() : m_posX{0}, m_posY{0}, m_pieceType{PieceType::None} {}
    Piece(int posX, int posY, PieceType pieceType = PieceType::None) : m_posX{posX}, m_posY{posY}, m_pieceType{pieceType} {}

    void draw() const {
        SDL_Rect pos{Piece_Size * m_posX + Width_Offset, Piece_Size * m_posY,  Piece_Size, Piece_Size};

        switch (m_pieceType) {
        case PieceType::X:
            SDL_RenderCopyEx(renderer, sprites, &xSpriteClip, &pos, 0, nullptr, SDL_FLIP_NONE);
            break;
        case PieceType::O:
            SDL_RenderCopyEx(renderer, sprites, &oSpriteClip, &pos, 0, nullptr, SDL_FLIP_NONE);
            break;
        default:
            break;
        }
    }

 private:
    int m_posX;
    int m_posY;

    PieceType m_pieceType;
};

Piece pieces[9] = {
    Piece(0, 0, PieceType::X),
    Piece(1, 0, PieceType::O),
    Piece(2, 0, PieceType::X),
    Piece(0, 1, PieceType::O),
    Piece(1, 1, PieceType::X),
    Piece(2, 1, PieceType::O),
    Piece(0, 2, PieceType::X),
    Piece(1, 2, PieceType::O),
    Piece(2, 2, PieceType::X)
};


int main(int argc, const char** argv) {
    if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0 ) {
        printf("初始化SDL失败%s\n", SDL_GetError());
    } else {
        window = SDL_CreateWindow("Tic Tac Toe", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);

        if ( !window ) {
            printf("初始化窗口失败%s\n", SDL_GetError());
        } else {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);

            if ( !renderer ) {
                printf("初始化渲染器失败%s\n", SDL_GetError());
            } else {
                int flags = IMG_INIT_PNG;
                if ( (IMG_Init(flags) & flags) == 0 ) {
                    printf("初始化SLD_image失败%s\n", IMG_GetError());
                } else {
                    SDL_Surface* spriteSheetSurface = IMG_Load("resources/images/tictactoe.png");

                    if ( !spriteSheetSurface ) {
                        printf("加载精灵表失败%s\n", IMG_GetError());
                    } else {
                        sprites = SDL_CreateTextureFromSurface(renderer, spriteSheetSurface);

                        if ( !sprites ) {
                            printf("创建精灵表纹理失败%s\n", SDL_GetError());
                        } else {
                            printf("哈哈，一切都好!开始渲染");

                            SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

                            SDL_Event e;

                            bool quit = false;

                            while (!quit) {
                                while (SDL_PollEvent(&e)) {
                                    if (e.type == SDL_QUIT) {
                                        quit = true;
                                    }
                                }

                                SDL_SetRenderDrawColor(renderer, 0xCC, 0xCC, 0xCC, 0xFF);
                                SDL_RenderClear(renderer);

                                for (const Piece& p : pieces) {
                                    p.draw();
                                }

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
