/*This source code copyrighted by Lazy Foo' Productions (2004-2019)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, vectors, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include "texturelib.h"
#include <SDL2/SDL_mixer.h>
#include <bits/stdc++.h>
using namespace std;
//Screen dimension constants
const int SCREEN_WIDTH = 840;
const int SCREEN_HEIGHT = 480;

//The Naruto that will move around on the screen
//Scene textures
Mix_Music *gMusic = NULL;
//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to








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
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
				{
					printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
					success = false;
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
	if( !gObstacleTexture.loadFromFile( "rock.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !gHealthTexture.loadFromFile( "health/1h.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	gMusic = Mix_LoadMUS( "mu2.ogg" );
	if( gMusic == NULL )
	{
		printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	return success;
}

void close()
{
	//Free loaded images
	gNarutoTexture.free();
	gBGTexture.free();
	gShurikenTexture.free();
	gObstacleTexture.free();
	Mix_HaltMusic();
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
	srand(time(0));
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
			int waiting_for_shuriken=0;
			Shuriken sos[3];
			Obstacle rocks[10];
			int shot_shuriken=0;
			int score=0,counter=0;
			char health_char='1';
			string health_path="health/1h.png";
			Mix_PlayMusic( gMusic, -1 );
			//int solid=0;
			while( !quit )
			{
				//solid++;
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//solid++;
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}

					//Handle input for the Naruto
					Naruto.handleEvent( e );
				}

				//Move the Naruto

				for(int i=0;i < 10 ; i++ )
				{
					if(!rocks[i].flag_of_obstacle)
					{
						if(rand()%2==0)
						{
							rocks[i].flag_of_obstacle=1;
						}
					}
				}

				Naruto.move();
				waiting_for_shuriken++;
				if(waiting_for_shuriken%220==0)
					shot_shuriken=0;
				if(Naruto.shuriken_throwed)
				{


					for(int i=0 ; i < 2; i++)
					{
						if(waiting_for_shuriken<=220)
							continue;
						if(!sos[i].flag_of_shuriken)
						{
							cout << i << endl;
							shot_shuriken++;
							if(shot_shuriken==2)
							{
								shot_shuriken=0;
								waiting_for_shuriken=0;
							}
							sos[i].flag_of_shuriken=1;
							sos[i].mPosX=Naruto.mPosX+Naruto.Naruto_WIDTH;
							sos[i].mPosY=Naruto.mPosY + Naruto.Naruto_HEIGHT/2 -5;
							break;
						}
					}	
					Naruto.shuriken_throwed=0;
				}
				

				//Scroll background
				scrollingOffset-=8;
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
				if(countt==5){
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
				for(int i=0;i < 10 ; i++ )
				{
					if(rocks[i].flag_of_obstacle)
					{
						rocks[i].move();
						
						if(checkCollision(Naruto.Naruto_Rect,rocks[i].Obstacle_rect) && !rocks[i].hitten){
							cout << "fuck noh!!" << endl;
							rocks[i].hitten=1;
							Naruto.Life-=25;
							health_path[7]++;
							gHealthTexture.loadFromFile(health_path);
							
							cout << "Life :" << Naruto.Life << endl;
						}
						if(rocks[i].mPosX<=-10)
						{
							rocks[i].mPosX=SCREEN_WIDTH+rand()%15000;
							rocks[i].flag_of_obstacle=0;
							rocks[i].hitten=0;
							score+=10;
							cout << i << endl;
						}
						if(rocks[i].mPosX < SCREEN_WIDTH)
						{


							for(int j=0;j<3;j++)
							{
								if(checkCollision(sos[j].Shuriken_rect, rocks[i].Obstacle_rect ) && sos[j].flag_of_shuriken )
								{
									cout << "fucl yea" << endl;
									score+=10;
									rocks[i].mPosX=SCREEN_WIDTH+rand()%15000;
									rocks[i].flag_of_obstacle=0;
									rocks[i].hitten=0;
									sos[j].flag_of_shuriken=0;

								}
							}
						}
						rocks[i].render();
					}


				}
				counter++;
				if(counter==10)
				{
					counter=0;
					score++;
				}
				gHealthTexture.render(10,10);
				SDL_RenderPresent( gRenderer );
				if(Naruto.Life<=0)
				{
					cout << "You are fucked!!" << endl;
					cout << "Your Score : " << score << endl;
					
					SDL_Delay(1000);
					quit=true;
				}
			//Update screen
				
	
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}