#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <bits/stdc++.h>
//#include "texturelib.h"



class Obstacle
{
	public:
		int Obstacle_WIDTH = 30;
		int Obstacle_HEIGHT = 25;
		int mPosX, mPosY;
		int mVelX;
		int flag_of_obstacle;
		int hitten;
		//Maximum axis velocity of the Obstacle
		static const int Obstacle_VEL = -8;

		//Initializes the variables
		Obstacle();

		//Takes key presses and adjusts the Obstacle's velocity
		//Moves the Obstacle
		void move();
		void close();
		void init();
		//Shows the Obstacle on the screen
		void render();
		SDL_Rect Obstacle_rect; 

    private:
		//The X and Y offsets of the Obstacle
		//The velocity of the Obstacle
		int fuck;
};
void Obstacle::init()
{
	mVelX=Obstacle_VEL;
	flag_of_obstacle=0;
	mPosY=GROUND + 5*Obstacle_HEIGHT/3;
	mPosX=840 + rand()%15000;
	Obstacle_rect.w=Obstacle_WIDTH;
	Obstacle_rect.h=Obstacle_HEIGHT;
	hitten=0;
}
Obstacle::Obstacle()
{
	
	mVelX=Obstacle_VEL;
	flag_of_obstacle=0;
	mPosY=GROUND + 5*Obstacle_HEIGHT/3;
	mPosX=840 + rand()%15000;
	Obstacle_rect.w=Obstacle_WIDTH;
	Obstacle_rect.h=Obstacle_HEIGHT;
	hitten=0;
}	

void Obstacle::move()
{
	mPosX+=mVelX;
	Obstacle_rect.x=mPosX;
	Obstacle_rect.y=mPosY;
}





