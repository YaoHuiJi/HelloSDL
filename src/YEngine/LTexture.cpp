#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "LTexture.hpp"

extern SDL_Renderer* gRenderer;
extern TTF_Font* gFont;

using namespace YEngine;

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

bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor ){
    free();

    SDL_Surface* textSurface  = TTF_RenderUTF8_Solid(gFont, textureText.data(), textColor);

    if(!textSurface){
        printf("渲染文字失败! SDL_tff Error:%s\n" , TTF_GetError());
    }else
    {
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
        if( mTexture == NULL )
        {
            printf( "渲染文字失败! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }

        SDL_FreeSurface( textSurface );
    }

    return mTexture != nullptr;
}

void LTexture::free(){
    if(mTexture != NULL){
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::setColor(Uint8 r ,Uint8 g, Uint8 b){
    SDL_SetTextureColorMod(mTexture,r,g,b);
}

void LTexture::setBlendMode( SDL_BlendMode blending ){
    SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha( Uint8 alpha ){
    SDL_SetTextureAlphaMod(mTexture, alpha);
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

void LTexture::render(int x, int y , SDL_Rect* clip, double scale , double angle, SDL_Point* center, SDL_RendererFlip flip ){
//Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };

    //Set clip rendering dimensions
    if( clip != NULL )
    {
        renderQuad.w = clip->w*scale;
        renderQuad.h = clip->h*scale;
    }

    //Render to screen
    //SDL_RenderCopy( gRenderer, mTexture, clip, &renderQuad );
    SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth(){
    return mWidth;
}

int LTexture::getHeight(){
    return mHeight;
}