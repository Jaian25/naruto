#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <bits/stdc++.h>
//#include "texturelib.h"



class Obstacle
{
	public:
		static const int Obstacle_WIDTH = 50;
		static const int Obstacle_HEIGHT = 70;
		int mPosX, mPosY;
		int mVelX;
		int flag_of_obstacle;
		//Maximum axis velocity of the Obstacle
		static const int Obstacle_VEL = -5;

		//Initializes the variables
		Obstacle();

		//Takes key presses and adjusts the Obstacle's velocity
		//Moves the Obstacle
		void move();
		void close();
		//Shows the Obstacle on the screen
		void render();

    private:
		//The X and Y offsets of the Obstacle
		//The velocity of the Obstacle
		int fuck;
};

Obstacle::Obstacle()
{
	
	mVelX=Obstacle_VEL;
	flag_of_obstacle=0;
	mPosY=440 ;
	mPosX=840 + rand()%15000;


}	

void Obstacle::move()
{
	mPosX+=mVelX;
}





