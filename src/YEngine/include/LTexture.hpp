#ifndef LTEXTURE
#define LTEXTURE

#include <string>
#include <SDL.h>

namespace YEngine{

class LTexture
{
    public:
        LTexture();
        
        ~LTexture();

        bool loadFromFile(std::string path, Uint8 colorKey_r=255, Uint8 colorKey_g=255, Uint8 colorKey_b=255);

        #if defined(SDL_TTF_MAJOR_VERSION)
        bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
        #endif

        void free();

        //Set color modulation
        void setColor(Uint8 r ,Uint8 g, Uint8 b);

        //Set blending
        void setBlendMode( SDL_BlendMode blending );

        //Set alpha modulation
        void setAlpha( Uint8 alpha );

        void render(int x = -1, int y = -1,int width = -1, int height = -1);

        void render( int x, int y, SDL_Rect* clip = NULL, double scale = 1 , double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );


        int getWidth();

        int getHeight();

        SDL_Texture* getTexture() const {
            return mTexture;
        }
    
    private:
        SDL_Texture* mTexture;

        int mWidth;
        int mHeight;
};

}

#endif /* LTEXTURE */
