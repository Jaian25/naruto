#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include "constants.h"
//#include "texturelib.h"
#include <bits/stdc++.h>
using namespace std;

class Naruto
{
    public:
		//The dimensions of the Naruto
		static const int Naruto_WIDTH = 30;
		static const int Naruto_HEIGHT = 67;
		int mPosX, mPosY;
		//Maximum axis velocity of the Naruto
		static const int Naruto_VEL = 10;
		int jumped;
		int shuriken_throwed;
		int Life;
		int Power;
		int hitten;
		//Initializes the variables
		Naruto();

		//Takes key presses and adjusts the Naruto's velocity
		void handleEvent( SDL_Event& e );

		//Moves the Naruto
		void move();
		void jump();
		void init();
		//Shows the Naruto on the screen
		void render();
		double initial ;
		double velocity;
		double gravity;
		SDL_Rect Naruto_Rect;
    private:
		//The X and Y offsets of the Naruto
		

		//The velocity of the Naruto
		int mVelX, mVelY;
};
void Naruto::init()
{
	mPosX = 40;
    mPosY = GROUND;

    //Initialize the velocity
    hitten=0;
    mVelX = 0;
    mVelY = 0;
    jumped=0;
    shuriken_throwed=0;
    initial = -13;
    velocity=initial;
 	gravity=0.5;
 	Naruto_Rect.w=Naruto_WIDTH;
 	Naruto_Rect.h=Naruto_HEIGHT;
 	Life=100;
 	Power=0;
}
Naruto::Naruto()
{
    //Initialize the offsets
    mPosX = 40;
    mPosY = GROUND;

    //Initialize the velocity
    hitten=0;
    mVelX = 0;
    mVelY = 0;
    jumped=0;
    shuriken_throwed=0;
    initial = -13;
    velocity=initial;
 	gravity=0.5;
 	Naruto_Rect.w=Naruto_WIDTH;
 	Naruto_Rect.h=Naruto_HEIGHT;
 	Life=100;
 	Power=0;
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
    // cout << mVelX << ' ';

    //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        if(!PauseOn)
        switch( e.key.keysym.sym )
        {
        	case 'p': PauseOn=1;break;
            case SDLK_UP: mVelY -= Naruto_VEL; break;
            case SDLK_DOWN: mVelY += Naruto_VEL; break;
            case 'a': mVelX -= Naruto_VEL; break;
            case 'd': mVelX += Naruto_VEL; break;
            case 'w': jumped=1;break;
            case 'q': shuriken_throwed=1;break;
            
        }
        else
        {
        	if(e.key.keysym.sym=='p')
        	{
        		PauseOn=0;
        	}
        	mVelX=0;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        if(!PauseOn)
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY += Naruto_VEL; break;
            case SDLK_DOWN: mVelY -= Naruto_VEL; break;
            case 'a': mVelX += Naruto_VEL; break;
            case 'd': mVelX -= Naruto_VEL; break;
            case 'p': mVelX =0;break;

        }
        else
        	mVelX=0;
    }
   // cout << "Vel changed to " << mVelX <<endl;
   // cout << PauseOn << endl;
    if(PauseOn)
    {
    	mVelX=0;
    }
    // mVelX = min(0, mVelX);
    // cout << mVelX << endl;
}

void Naruto::move()
{   
    //Move the Naruto left or right
    if(jumped)
	{
		jump();
	}

    mPosX += mVelX;
    mPosX=max(mPosX, 40 );
    mPosX=min(mPosX, SCREEN_WIDTH/2 );
    //If the Naruto went too far to the left or right
    

    //Move the Naruto up or down
    mPosY += mVelY;

    //If the Naruto went too far up or down
    if( ( mPosY < 0 ) || ( mPosY + Naruto_HEIGHT > SCREEN_HEIGHT ) )
    {
        //Move back
        mPosY -= mVelY;
    }
    cout << "Pos " << mPosX << ' ' << "Vel " << mVelX << endl;
    Naruto_Rect.x=mPosX;
    Naruto_Rect.y=mPosY;
}

