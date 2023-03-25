#include <SDL.h>
#include <SDL_image.h>
#include "includes/LTexture.hpp"

extern SDL_Renderer* gRenderer;

LTexture::LTexture(){
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture(){
    free();
}

bool LTexture::loadFromFile(std::string path){
    free();

    SDL_Texture* newTexture = NULL;

    SDL_Surface* loadedSurface = IMG_Load(path.c_str());

    if(loadedSurface == NULL){
        printf("加载图片(%s)失败! SDL_Error:%s\n",path.c_str(),SDL_GetError());
    }else{
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format,255,255,255 ) );

        newTexture = SDL_CreateTextureFromSurface( gRenderer,loadedSurface);

        if(newTexture==NULL){
            printf( "无法从表面(%s)生成材质 ! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }else{
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        SDL_FreeSurface(loadedSurface);
    }

    mTexture = newTexture;

    return mTexture != NULL;
}

void LTexture::free(){
    if(mTexture != NULL){
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::render( int x, int y, int width, int height){

    SDL_Rect renderQuad = {x, y, width, height};

    if( width==-1 && height==-1 ){
        renderQuad = {x, y, mWidth, mHeight};
    }

    if(x == -1 || y == -1){
        SDL_RenderCopy( gRenderer, mTexture, NULL, NULL);
    }else{
        SDL_RenderCopy( gRenderer, mTexture, NULL, &renderQuad);
    }
}

int LTexture::getWidth(){
    return mWidth;
}

int LTexture::getHeight(){
    return mHeight;
}