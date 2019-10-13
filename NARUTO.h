#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
//#include "texturelib.h"





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
		double initial ;
		double velocity;
		double gravity;

    private:
		//The X and Y offsets of the Naruto
		

		//The velocity of the Naruto
		int mVelX, mVelY;
};

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
    initial = -13;
    velocity=initial;
 	gravity=0.5;

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
    if( ( mPosX < 0 ) || ( mPosX + Naruto_WIDTH > 840 ) )
    {
        //Move back
        mPosX -= mVelX;
    }

    //Move the Naruto up or down
    mPosY += mVelY;

    //If the Naruto went too far up or down
    if( ( mPosY < 0 ) || ( mPosY + Naruto_HEIGHT > 480 ) )
    {
        //Move back
        mPosY -= mVelY;
    }
}

