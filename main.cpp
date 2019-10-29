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
#include <SDL2/SDL_ttf.h>
using namespace std;
//Screen dimension constants

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

int where = MENU, whereInMenu;


char getScore[100];


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
		gWindow = SDL_CreateWindow( "Naruto Uzumaki", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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
	if( TTF_Init() == -1 )
	{
		printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}

	return success;
}

bool loadMedia()
{	
	//Loading success flag
	bool success = true;

	//Load Fonts
	if( !U_BG.loadFromFile( "test.png" ) )
	{
		printf( "Failed to load MENU texture!\n" );
		success = false;
	}
	//Open the font
	gFont = TTF_OpenFont( "lazy.ttf", 28 );
	sFont = TTF_OpenFont( "score.ttf", 28 );
	if( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else
	{	
		//Render the prompt
		TTF_SetFontStyle(gFont, TTF_STYLE_BOLD);
		//TTF_SetFontStyle(sFont, TTF_STYLE_BOLD);
		SDL_Color textColor = { 255, 255, 255, 0xFF };
		string T = "Enter Handle : ";
		if( !gPromptTextTexture.loadFromRenderedText( T.c_str(), textColor ) )
		{
			printf( "Failed to render prompt text!\n" );
			success = false;
		}
	}
	//Load Menu
	
    if( !gMenuTexture.loadFromFile( "menu.png" ) )
	{
		printf( "Failed to load MENU texture!\n" );
		success = false;
	}

    if( !gMenuPlayTexture.loadFromFile( "Play.png" ) )
    {
        printf( "Failed to load MENU texture!\n" );
        success = false;
    }
    if( !gMenuInsTexture.loadFromFile( "Ins.png" ) )
    {
        printf( "Failed to load MENU texture!\n" );
        success = false;
    }
    if( !gMenuLeadTexture.loadFromFile( "Lead.png" ) )
    {
        printf( "Failed to load MENU texture!\n" );
        success = false;
    }
    if( !gMenuExitTexture.loadFromFile( "Exit.png" ) )
    {
        printf( "Failed to load MENU texture!\n" );
        success = false;
    }

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
	if( !gPowerTexture.loadFromFile( "power/0.png" ) )
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
	gHealthTexture.free();
	gPowerTexture.free();
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

void changeFromMenu(int x, int y) {

	if(x >= 125 && x <= 445){
		if(y >= 255 && y <= 325){
			where = USERNAME;
			return;
		}
		// if(y >= 330 && y <= 400){
		// 	where = INSTRUCTION;
		// 	return;
		// }
	}
	return;
}

void changeMenuAnimation(int x, int y){

    if(x >= 125 && x <= 445){
        if(y >= 255 && y <= 325){
            whereInMenu = GAME;
            return;
        }
        if(y >= 325 && y <= 405){
            whereInMenu = INSTRUCTION;
            return;
        }
        if(y >= 405 && y <= 480){
            whereInMenu = LEADERBOARD;
            return;
        }
        if(y >= 480 && y <= 555){
            whereInMenu = EXIT;
            return;
        }
    }
    whereInMenu = DEFAULT;
}

void handleMenuEvent( SDL_Event& e )
{
	//If a key was pressed

	int mx, my, wx, wy;

    if(e.type == SDL_MOUSEMOTION){

        SDL_GetGlobalMouseState(&mx, &my);
        SDL_GetWindowPosition(gWindow, &wx, &wy);
        //cout << mx - wx << ' ' << my  - wy << endl;
        changeMenuAnimation(mx - wx, my - wy);
    }

	if(e.type == SDL_MOUSEBUTTONUP) {

		SDL_GetGlobalMouseState(&mx, &my);
		SDL_GetWindowPosition(gWindow, &wx, &wy);
		cout << mx - wx << ' ' << my  - wy << endl;
		changeFromMenu(mx - wx, my - wy);
	}
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
			Shuriken sos[5];
			Obstacle rocks[10];
			Moving_Obstacle ninja[10];
			for(int i=0;i<10;i++)
			{
				ninja[i].path_of_Moving_Obstacle='1';
			}
			int shot_shuriken=0;
			int score=0,counter=0, prevscore = 0;
			char health_char='1';
			string health_path="health/1h.png";
			string power_path="power/0.png";
			Mix_PlayMusic( gMusic, -1 );
			//int solid=0;

				//Main loop flag

			bool renderText = false;
			SDL_Color textColor = { 255, 255, 255, 0xFF };

			//The current input text.
			std::string inputText = "";
			gInputTextTexture.loadFromRenderedText( inputText.c_str(), textColor );

			//Enable text input
			SDL_StartTextInput();

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
					if(where == MENU) {
						handleMenuEvent( e );
					}
					if(where == USERNAME) {
						if( e.type == SDL_QUIT)
						{
							quit = true;
						}
						//Special key input
						else if( e.type == SDL_KEYDOWN )
						{
							//Handle backspace

							if(e.key.keysym.sym == SDLK_RETURN && inputText.size()){
								//ScreenQuit = true;
								where = GAME;
								gFont = sFont;
								SDL_StopTextInput();
								SDL_Delay(250);
							}

							if( e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0 )
							{
								//lop off character
								inputText.pop_back();
								renderText = true;
							}
							//Handle copy
							else if( e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )
							{
								SDL_SetClipboardText( inputText.c_str() );
							}
							//Handle paste
							else if( e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
							{
								inputText = SDL_GetClipboardText();
								renderText = true;
							}
						}
						//Special text input event
						else if( e.type == SDL_TEXTINPUT )
						{
							//Not copy or pasting
							if( !( SDL_GetModState() & KMOD_CTRL && ( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C' || e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V' ) ) )
							{
								//Append character
								inputText += e.text.text;
								renderText = true;
							}
						}
					}
					if(where == GAME)Naruto.handleEvent( e );
				}

				if(where == USERNAME) {

					//Rerender text if needed
					cout << inputText << endl;
					if( renderText )
					{
						//Text is not empty
						if( inputText != "" )
						{
							//Render new text
							gInputTextTexture.loadFromRenderedText( inputText.c_str(), textColor );
						}
						//Text is empty
						else
						{
							//Render space texture
							gInputTextTexture.loadFromRenderedText( " ", textColor );
						}
					}
					//SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255 );
						//Render background
					U_BG.render(0 , 0 );
					U_BG.render(U_BG.getWidth(), 0 );

					//Clear screen
					//SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
					//Render text textures
					gPromptTextTexture.render( ( SCREEN_WIDTH - gPromptTextTexture.getWidth() - gInputTextTexture.getWidth()) / 2, SCREEN_HEIGHT / 2 - 5);
					gInputTextTexture.render( ( SCREEN_WIDTH + gPromptTextTexture.getWidth() - gInputTextTexture.getWidth()) / 2, SCREEN_HEIGHT / 2 - 5);

					//Update screen
					SDL_RenderPresent( gRenderer );
					
					//Disable text input
				}

				if(where == MENU) {

                    if(whereInMenu == GAME){
                        SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255 );
                        SDL_RenderClear( gRenderer );

                        //Render background
                        gMenuPlayTexture.render( scrollingOffset, 0 );
                        gMenuPlayTexture.render( scrollingOffset + gMenuPlayTexture.getWidth(), 0 );
                        SDL_RenderPresent( gRenderer );
                    }
                    if(whereInMenu == INSTRUCTION){
                        SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255 );
                        SDL_RenderClear( gRenderer );

                        //Render background
                        gMenuInsTexture.render( scrollingOffset, 0 );
                        gMenuInsTexture.render( scrollingOffset + gMenuInsTexture.getWidth(), 0 );
                        SDL_RenderPresent( gRenderer );
                    }
                    if(whereInMenu == LEADERBOARD){
                        SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255 );
                        SDL_RenderClear( gRenderer );

                        //Render background
                        gMenuLeadTexture.render( scrollingOffset, 0 );
                        gMenuLeadTexture.render( scrollingOffset + gMenuLeadTexture.getWidth(), 0 );
                        SDL_RenderPresent( gRenderer );
                    }
                    if(whereInMenu == EXIT){
                        SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255 );
                        SDL_RenderClear( gRenderer );

                        //Render background
                        gMenuExitTexture.render( scrollingOffset, 0 );
                        gMenuExitTexture.render( scrollingOffset + gMenuExitTexture.getWidth(), 0 );
                        SDL_RenderPresent( gRenderer );
                    }

					if(whereInMenu == DEFAULT){
                        SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255 );
                        SDL_RenderClear( gRenderer );

                        //Render background
                        gMenuTexture.render( scrollingOffset, 0 );
                        gMenuTexture.render( scrollingOffset + gMenuTexture.getWidth(), 0 );
                        SDL_RenderPresent( gRenderer );
                    }
				}

				if(where == GAME){

					renderText = true;

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
						if(!ninja[i].flag_of_obstacle)
						{
							if(rand()%107==0)
							{
								ninja[i].flag_of_obstacle=1;
							}
						}
					}

					Naruto.move();
					waiting_for_shuriken++;
					if(waiting_for_shuriken%220==0){
						if(Naruto.Power < 4)
						{
							Naruto.Power++;
							power_path[6]++;
							gPowerTexture.loadFromFile(power_path);
						}
						waiting_for_shuriken=0;
					}
					if(Naruto.shuriken_throwed)
					{

						if(Naruto.Power>0)
						for(int i=0 ; i < 5; i++)
						{
							
							if(!sos[i].flag_of_shuriken)
							{
								Naruto.Power--;
								power_path[6]--;
								gPowerTexture.loadFromFile(power_path);
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
					SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 0 );
					SDL_RenderClear( gRenderer );

					//Render background
					gBGTexture.render( scrollingOffset, BORDER );
					gBGTexture.render( scrollingOffset + gBGTexture.getWidth(), BORDER );

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
					for(int i=0;i<5;i++)
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


								for(int j=0;j<5;j++)
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

					for(int i=0;i < 10 ; i++ )
					{
						if(ninja[i].flag_of_obstacle)
						{
							ninja[i].move();
							
							if(checkCollision(Naruto.Naruto_Rect,ninja[i].Moving_Obstacle_rect) && !ninja[i].hitten){
								cout << "fuck noh injaaa!!" << endl;
								ninja[i].hitten=1;
								Naruto.Life-=25;
								health_path[7]++;
								gHealthTexture.loadFromFile(health_path);
								
								cout << "Life :" << Naruto.Life << endl;
							}
							if(ninja[i].mPosX<=-10)
							{
								ninja[i].mPosX=SCREEN_WIDTH+rand()%15000;
								ninja[i].flag_of_obstacle=0;
								ninja[i].hitten=0;
								score+=10;
								
							}
							if(ninja[i].mPosX < SCREEN_WIDTH)
							{


								for(int j=0;j<5;j++)
								{
									if(checkCollision(sos[j].Shuriken_rect, ninja[i].Moving_Obstacle_rect ) && sos[j].flag_of_shuriken )
									{
										cout << "fucl yea inja" << endl;
										score+=50;
										ninja[i].mPosX=SCREEN_WIDTH+rand()%15000;
										ninja[i].flag_of_obstacle=0;
										ninja[i].hitten=0;
										sos[j].flag_of_shuriken=0;

									}
								}
							}
							ninja[i].counter++;
							if(ninja[i].counter==5){

								ninja[i].path_of_Moving_Obstacle++;

								if(ninja[i].path_of_Moving_Obstacle=='4')
									ninja[i].path_of_Moving_Obstacle='1';
								ninja[i].counter=0;
							}

							string temp="ninja/";
							temp+= ninja[i].path_of_Moving_Obstacle;
							temp+=".png";
							gMovingObstacle.loadFromFile(temp);

							gMovingObstacle.render(ninja[i].mPosX,ninja[i].mPosY);
							//cout << ninja[i].mPosX << " ";
						}


					}
					counter++;
					if(counter==10)
					{
						counter=0;
						score++;
					}

					sprintf(getScore, "Score : %04d", score);
					gScore.loadFromRenderedText( getScore, textColor );
					gScore.render( ( SCREEN_WIDTH - gScore.getWidth() - gScore.getWidth()) + 50, 35);
					
					gHealthTexture.render(50,10);
					gPowerTexture.render(50,50);
					
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
	}

	//Free resources and close SDL
	close();

	return 0;
}