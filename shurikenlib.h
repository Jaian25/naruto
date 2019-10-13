#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
//#include "texturelib.h"



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





