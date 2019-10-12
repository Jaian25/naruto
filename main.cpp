/*This source code copyrighted by Lazy Foo' Productions (2004-2019)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, vectors, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <bits/stdc++.h>
using namespace std;
//Screen dimension constants
const int SCREEN_WIDTH = 840;
const int SCREEN_HEIGHT = 480;
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;


double initial = -13;
double velocity=initial;
double gravity=0.5;
//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );
		
		#if defined(_SDL_TTF_H) || defined(SDL_TTF_H)
		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
		#endif

		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );
		
		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
};
LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#if defined(_SDL_TTF_H) || defined(SDL_TTF_H)
bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface != NULL )
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	else
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}

	
	//Return success
	return mTexture != NULL;
}
#endif

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}
		
void LTexture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}
LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

//The Naruto that will move around on the screen
//Scene textures
LTexture gNarutoTexture;
LTexture gBGTexture;
LTexture gShurikenTexture;

class Shuriken
{
	public:
		static const int Shuriken_WIDTH = 50;
		static const int Shuriken_HEIGHT = 70;
		int mPosX, mPosY;
		int mVelX;
		int flag_of_shuriken;
		//Maximum axis velocity of the Naruto
		static const int Shuriken_VEL = 10;

		//Initializes the variables
		Shuriken();

		//Takes key presses and adjusts the Naruto's velocity
		//Moves the Naruto
		void move();
		void close();
		//Shows the Naruto on the screen
		void render();

    private:
		//The X and Y offsets of the Naruto
		//The velocity of the Naruto
		int fuck;
};

Shuriken::Shuriken()
{
	
	mVelX=Shuriken_VEL;
	flag_of_shuriken=0;
}	

void Shuriken::move()
{
	mPosX+=mVelX;
}
void Shuriken::render()
{
	gShurikenTexture.render(mPosX,mPosY);
}

void Shuriken::close()
{
	//Free loaded images
	gShurikenTexture.free();

}



















class Naruto
{
    public:
		//The dimensions of the Naruto
		static const int Naruto_WIDTH = 50;
		static const int Naruto_HEIGHT = 70;
		int mPosX, mPosY;
		//Maximum axis velocity of the Naruto
		static const int Naruto_VEL = 10;
		int jumped;
		int shuriken_throwed=0;
		//Initializes the variables
		Naruto();

		//Takes key presses and adjusts the Naruto's velocity
		void handleEvent( SDL_Event& e );

		//Moves the Naruto
		void move();
		void jump();

		//Shows the Naruto on the screen
		void render();

    private:
		//The X and Y offsets of the Naruto
		

		//The velocity of the Naruto
		int mVelX, mVelY;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to





Naruto::Naruto()
{
    //Initialize the offsets
    mPosX = 20;
    mPosY = 390;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;
    jumped=0;
    shuriken_throwed=0;
}

void Naruto::jump()
{
	if(velocity >= abs(initial)+1)
	{
		velocity=initial;
		jumped=0;
	}
	else
	{
		mPosY+=velocity;
		velocity+=gravity;

	}
}

void Naruto::handleEvent( SDL_Event& e )
{
    //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY -= Naruto_VEL; break;
            case SDLK_DOWN: mVelY += Naruto_VEL; break;
            case 'a': mVelX -= Naruto_VEL; break;
            case 'd': mVelX += Naruto_VEL; break;
            case 'w': jumped=1;break;
            case 'q': shuriken_throwed=1;break;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY += Naruto_VEL; break;
            case SDLK_DOWN: mVelY -= Naruto_VEL; break;
            case 'a': mVelX += Naruto_VEL; break;
            case 'd': mVelX -= Naruto_VEL; break;

        }
    }
}

void Naruto::move()
{
    //Move the Naruto left or right
    if(jumped)
	{
		jump();
	}
    mPosX += mVelX;

    //If the Naruto went too far to the left or right
    if( ( mPosX < 0 ) || ( mPosX + Naruto_WIDTH > SCREEN_WIDTH ) )
    {
        //Move back
        mPosX -= mVelX;
    }

    //Move the Naruto up or down
    mPosY += mVelY;

    //If the Naruto went too far up or down
    if( ( mPosY < 0 ) || ( mPosY + Naruto_HEIGHT > SCREEN_HEIGHT ) )
    {
        //Move back
        mPosY -= mVelY;
    }
}

void Naruto::render()
{
    //Show the Naruto
	gNarutoTexture.render( mPosX, mPosY );
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "Naruto Uzumaki", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load Naruto texture
	if( !gNarutoTexture.loadFromFile( "Naruto.png" ) )
	{
		printf( "Failed to load Naruto texture!\n" );
		success = false;
	}

	//Load background texture
	if( !gBGTexture.loadFromFile( "bg.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !gShurikenTexture.loadFromFile( "dot.bmp" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}

	return success;
}

void close()
{
	//Free loaded images
	gNarutoTexture.free();
	gBGTexture.free();

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//The Naruto that will be moving around on the screen
			Naruto Naruto;

			//The background scrolling offset
			int scrollingOffset = 0;

			//While application is running
			
			string s= "n1.png";
			int countt=0;
			Shuriken sos[3];
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}

					//Handle input for the Naruto
					Naruto.handleEvent( e );
				}

				//Move the Naruto
				Naruto.move();
				if(Naruto.shuriken_throwed)
				{

					for(int i=0 ; i < 3; i++)
					{
						if(!sos[i].flag_of_shuriken)
						{
							//cout << i << endl;
							sos[i].flag_of_shuriken=1;
							sos[i].mPosX=Naruto.mPosX+Naruto.Naruto_WIDTH;
							sos[i].mPosY=Naruto.mPosY + Naruto.Naruto_HEIGHT/3;
							break;
						}
					}	
					Naruto.shuriken_throwed=0;
				}
				



				//Scroll background
				scrollingOffset-=4;
				if( scrollingOffset < -gBGTexture.getWidth() )
				{
					scrollingOffset = 0;
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render background
				gBGTexture.render( scrollingOffset, -110 );
				gBGTexture.render( scrollingOffset + gBGTexture.getWidth(), -110 );

				//Render objects
			//	if(fl){
					Naruto.render();
				//	fl^=1;
					countt++;
					if(countt==6){
						s[1]++;
						if(s[1]=='7')
							s[1]='1';
						countt=0;
					}
					
				//	cout << s << endl;
					gNarutoTexture.loadFromFile(s); 
					for(int i=0;i<3;i++)
				{
					if(sos[i].flag_of_shuriken)
					{
						sos[i].move();
						sos[i].render();
						if(sos[i].mPosX>SCREEN_WIDTH)
							sos[i].flag_of_shuriken=0;
					}
				}
				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
