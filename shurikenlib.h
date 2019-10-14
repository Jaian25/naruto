#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
//#include "texturelib.h"



class Shuriken
{
	public:
		static const int Shuriken_WIDTH = 20;
		static const int Shuriken_HEIGHT = 20;
		int mPosX, mPosY;
		int mVelX;
		int flag_of_shuriken;
		//Maximum axis velocity of the Naruto
		static const int Shuriken_VEL = 12;

		//Initializes the variables
		Shuriken();

		//Takes key presses and adjusts the Naruto's velocity
		//Moves the Naruto
		void move();
		void close();
		//Shows the Naruto on the screen
		void render();
		SDL_Rect Shuriken_rect;

    private:
		//The X and Y offsets of the Naruto
		//The velocity of the Naruto
		int fuck;
};

Shuriken::Shuriken()
{
	
	mVelX=Shuriken_VEL;
	flag_of_shuriken=0;
	Shuriken_rect.w=Shuriken_WIDTH;
	Shuriken_rect.h=Shuriken_HEIGHT;
}	

void Shuriken::move()
{
	mPosX+=mVelX;
	Shuriken_rect.x=mPosX;
	Shuriken_rect.y=mPosY;
}





