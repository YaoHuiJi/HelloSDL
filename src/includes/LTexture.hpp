#ifndef LTEXTURE
#define LTEXTURE

#include <string>
#include <SDL.h>


class LTexture
{
    public:
        LTexture();
        
        ~LTexture();

        bool loadFromFile(std::string path);

        void free();

        void setColor(Uint8 r ,Uint8 g, Uint8 b);

        void render(int x=-1, int y=-1,int width=-1, int height=-1);

        int getWidth();

        int getHeight();
    
    private:
        SDL_Texture* mTexture;

        int mWidth;
        int mHeight;
};

#endif /* LTEXTURE */
