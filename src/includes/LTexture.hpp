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

        void render(int x, int y,int width=-1, int height=-1);

        int getWidth();

        int getHeight();
    
    private:
        SDL_Texture* mTexture;

        int mWidth;
        int mHeight;
};

#endif /* LTEXTURE */
