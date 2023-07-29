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
#include <span>
#include <cstdio>
#include <iostream>

class Board;

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

    void onClick() const {
        // TODO(Yao) Piece被点击后的处理器
    }

    Piece& operator=(const PieceType& pieceType) {
        m_pieceType = pieceType;
        return *this;
    }

 private:
    int m_posX;
    int m_posY;

    PieceType m_pieceType;
};


class Board {
 private:
    Piece m_pieces[9];

 public:
    Board() : m_pieces {
        Piece(0, 0, PieceType::None),
        Piece(0, 1, PieceType::None),
        Piece(0, 2, PieceType::None),

        Piece(1, 0, PieceType::None),
        Piece(1, 1, PieceType::None),
        Piece(1, 2, PieceType::None),

        Piece(2, 0, PieceType::None),
        Piece(2, 1, PieceType::None),
        Piece(2, 2, PieceType::None)
    } {}

    void setPiece(int posX, int posY, PieceType pieceType) {
        m_pieces[posY*3 + posX] = pieceType;
    }

    Piece& getPiece(int posX, int posY) {
        return m_pieces[posY*3 + posX];
    }

    const Piece* getPieces() {
        return m_pieces;
    }

    void handleEvent(const SDL_Event& e) {
        // TODO(Yao) 接收鼠标点击事件

        // TODO(Yao) 判断哪个Piece位置被点击

        // TODO(Yao) 调用Piece的onClick事件
    }

    void draw() {
        SDL_SetRenderDrawColor(renderer, 255, 128, 0, 255);

        SDL_Rect outBorder {Width_Offset, 0, Piece_Size*3, Piece_Size*3};
        SDL_RenderDrawRect(renderer, &outBorder);

        SDL_RenderDrawLine(renderer, Width_Offset, Piece_Size, Width_Offset + Piece_Size * 3, Piece_Size);
        SDL_RenderDrawLine(renderer, Width_Offset, Piece_Size * 2, Width_Offset + Piece_Size * 3, Piece_Size * 2);

        SDL_RenderDrawLine(renderer, Width_Offset + Piece_Size, 0, Width_Offset + Piece_Size, Piece_Size * 3);
        SDL_RenderDrawLine(renderer, Width_Offset + Piece_Size * 2, 0, Width_Offset + Piece_Size * 2, Piece_Size * 3);
    }
};

int main(int argc, const char** argv) {
    Board board;

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

                                    board.handleEvent(e);
                                }

                                SDL_SetRenderDrawColor(renderer, 0xCC, 0xCC, 0xCC, 0xFF);
                                SDL_RenderClear(renderer);

                                board.draw();

                                const Piece* pieces = board.getPieces();

                                for (int i = 0; i < 9; i++) {
                                    pieces[i].draw();
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
