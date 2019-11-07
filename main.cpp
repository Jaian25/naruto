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



char getScore[100], leaderboardScore[200];

bool scoreLoaded;
struct scoreData{
    char handle[100];
    int score;
    bool operator < (const scoreData &p) const {
        if(score == p.score) return strcmp(handle, p.handle) < 0;
        return score > p.score;
    }
};
std::set< scoreData > LeaderboardData;

struct saveData{
	int score;
	string handle, healthP, powerP;
	bool saved;
	saveData(int score = 0, string handle = "SYSTEM", string healthP = "health/1h.png", string powerP = "power/0.png", bool saved = false) :
		score(score), handle(handle), healthP(healthP), powerP(powerP), saved(saved) {};
}SLOT[6];

void readSaveData() {
	ifstream readF("savegames");
	if(!readF.is_open()){
		cout << "File Nai" << endl;
		return;
	} 
	for(int i = 0; i < 6;i++){
		readF >> SLOT[i].saved >> SLOT[i].score >> SLOT[i].handle >> SLOT[i].healthP >> SLOT[i].powerP;
	}
	readF.close();
}

void writeSaveData() {
	FILE *writeF = fopen("savegames", "w");
	for(int i = 0; i < 6;i++){
		fprintf(writeF, "%d %d %s %s %s\n", SLOT[i].saved, SLOT[i].score, SLOT[i].handle.c_str(), SLOT[i].healthP.c_str(), SLOT[i].powerP.c_str());
	}
	fclose(writeF);
}
void LoadSaveData(int slot, int &score, string &PlayerHandle,string &health_path,string &power_path, int &Nhealth, int &NPower){
	readSaveData();
	if(SLOT[slot].saved == false){
		cout << "Save Nai" << endl;
		return;
	}
	where = GAME;
	mainFont = sFont;
	score = SLOT[slot].score;
	PlayerHandle = SLOT[slot].handle;
	health_path = SLOT[slot].healthP;
	power_path = SLOT[slot].powerP;

	Nhealth = 100 - (health_path[7] - '0' - 1)*25;
	NPower = power_path[6] - '0';

	gHealthTexture.loadFromFile(health_path);
	gPowerTexture.loadFromFile(power_path);

}

void loadScoreFromFile() {

	scoreLoaded = true;

	FILE *readF = fopen("leaderboard", "r");
    if(readF == NULL) {
        cout << "No File Found" << endl;
    }
    else {
        scoreData temp;
        int rank;
        while(fscanf(readF, "%d %s %d", &rank, temp.handle, &temp.score) != EOF) LeaderboardData.insert( temp );
    }
    fclose(readF);
}

void updateScores() {
	loadScoreFromFile();
	FILE *writeF = fopen("leaderboard", "w");
	bool scoreLoaded = false;
    int rank = 0;
    for(auto user : LeaderboardData) {
        fprintf(writeF, "%d %s %d\n",++rank, user.handle, user.score);
        if(rank == 10) break;
    }
    fclose(writeF);
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
	mainFont = gFont;
	if( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else
	{	
		//Render the prompt
		TTF_SetFontStyle(mainFont, TTF_STYLE_BOLD);
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

	if( !gLeaderBoard1.loadFromFile( "LeaderBoard1.png" ) )
	{
		printf( "Failed to load MENU texture!\n" );
		success = false;
	}
	if( !gLeaderBoard2.loadFromFile( "LeaderBoard2.png" ) )
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
        if(whereInMenu == EXIT){
            where = EXIT;
            return;
        }
        if(whereInMenu == LEADERBOARD){
        	where = LEADERBOARD;
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

void handleMenuEvent( SDL_Event& e ){
	
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



int whereInLeaderBoard;

void changeFromLeaderBoard(int x, int y) {

	if(whereInLeaderBoard == BACK) {
		where = MENU;
		whereInLeaderBoard = 0;
		return;
	}
}

void changeLeaderBoardAnimation(int x, int y){

	if(x >= 14 && x <= 179 && y >= 375 and y <= 440) {
		whereInLeaderBoard = BACK;
		return; 
	}
	whereInLeaderBoard = 0;
}

void handleLeaderBoardEvents(SDL_Event& e) {

	int mx, my, wx, wy;

	if(e.type == SDL_MOUSEMOTION){

        SDL_GetGlobalMouseState(&mx, &my);
        SDL_GetWindowPosition(gWindow, &wx, &wy);
        // cout << mx - wx << ' ' << my  - wy << endl;
        changeLeaderBoardAnimation(mx - wx, my - wy);
    }

	if(e.type == SDL_MOUSEBUTTONUP) {

		SDL_GetGlobalMouseState(&mx, &my);
		SDL_GetWindowPosition(gWindow, &wx, &wy);
		// cout << mx - wx << ' ' << my  - wy << endl;
		changeFromLeaderBoard(mx - wx, my - wy);
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
            bool recall=0;
			//The background scrolling offset
			int scrollingOffset = 0;

			//While application is running
			
			// readSaveData();
			// writeSaveData();

			string s= "n1.png";
			int countt=0;
			int waiting_for_shuriken=0;
			Shuriken sos[5];
			Obstacle rocks[10];
			Moving_Obstacle ninja[10],bird[10];
			for(int i=0;i<10;i++)
			{
				ninja[i].path_of_Moving_Obstacle='1';
				bird[i].mPosY=400;
				bird[i].Moving_Obstacle_WIDTH=85;
				bird[i].Moving_Obstacle_HEIGHT=85;
				bird[i].path_of_Moving_Obstacle='1';
				bird[i].init();
			}
			int shot_shuriken=0;
			int score=0,counter=0;
			char health_char='1';
			string health_path="health/1h.png";
			gHealthTexture.loadFromFile(health_path);
			string power_path="power/0.png";
			gPowerTexture.loadFromFile(power_path);
			Mix_PlayMusic( gMusic, -1 );
			//int solid=0;

				//Main loop flag

			bool renderText = false;
			SDL_Color textColor = { 255, 255, 255, 0xFF };

			//The current input text.
			std::string PlayerHandle = " ";
			// gPlayerHandleTexture.loadFromRenderedText( PlayerHandle.c_str(), textColor );

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
                        if(recall==1)
                        {
                            // cout << "Dhuklam" << endl;

                            scrollingOffset = 0;

            //While application is running
            
                            readSaveData();
                            writeSaveData();

                            s= "n1.png";
                            countt=0;
                            waiting_for_shuriken=0;
                            
                            for(int i=0;i<10;i++)
							{
								ninja[i].path_of_Moving_Obstacle='1';
								bird[i].mPosY=400;
								bird[i].path_of_Moving_Obstacle='1';
							}
                            for(int i=0;i < 10 ; i++ )
                            {
                                rocks[i].init();
                                ninja[i].init();

                                bird[i].init();
                            }
                            shot_shuriken=0;
                            score=0;
                            counter=0;
                            health_char='1';
                            health_path="health/1h.png";
							gHealthTexture.loadFromFile(health_path);
							power_path="power/0.png";
							gPowerTexture.loadFromFile(power_path);
                            Naruto.Life = 100;
                            Naruto.Power=0;
                            //int solid=0;

                                //Main loop flag

                            renderText = false;
                            SDL_Color textColor = { 255, 255, 255, 0xFF };

                            //The current input text.
                            PlayerHandle = " ";
                            // gPlayerHandleTexture.loadFromRenderedText( PlayerHandle.c_str(), textColor );

                            //Enable text input
                            Naruto.init();
                            
                            SDL_StartTextInput();



                            recall=0;
                        }

						if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F1){
							LoadSaveData(0, score, PlayerHandle, health_path, power_path, Naruto.Life, Naruto.Power);
							// cout << score << ' ' << PlayerHandle << health_path << ' ' << power_path << endl;
						}
						else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F2){
							LoadSaveData(1, score, PlayerHandle, health_path, power_path, Naruto.Life, Naruto.Power);
						}
						else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F3){
							LoadSaveData(2, score, PlayerHandle, health_path, power_path, Naruto.Life, Naruto.Power);
						}
						else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F4){
							LoadSaveData(3, score, PlayerHandle, health_path, power_path, Naruto.Life, Naruto.Power);
						}
						else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F5){
							LoadSaveData(4, score, PlayerHandle, health_path, power_path, Naruto.Life, Naruto.Power);
						}
						else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F6){
							LoadSaveData(5, score, PlayerHandle, health_path, power_path, Naruto.Life, Naruto.Power);
						}
						handleMenuEvent( e );
					}
					else if(where == USERNAME) {
						if( e.type == SDL_QUIT)
						{
							quit = true;
						}
						//Special key input
						else if( e.type == SDL_KEYDOWN )
						{
							//Handle backspace

							if(e.key.keysym.sym == SDLK_RETURN && PlayerHandle.size()){
								//ScreenQuit = true;
								where = GAME;
								mainFont = sFont;
								SDL_StopTextInput();
								SDL_Delay(250);
                                recall=1;
							}

							if( e.key.keysym.sym == SDLK_BACKSPACE && PlayerHandle.length() > 1 )
							{
								//lop off character
								PlayerHandle.pop_back();
								renderText = true;
							}
							// //Handle copy
							// else if( e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )
							// {
							// 	SDL_SetClipboardText( PlayerHandle.c_str() );
							// }
							// //Handle paste
							// else if( e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
							// {
							// 	PlayerHandle = SDL_GetClipboardText();
							// 	renderText = true;
							// }
						}
						//Special text input event
						else if( e.type == SDL_TEXTINPUT && PlayerHandle.size() + 1 <= 20)
						{
							//Not copy or pasting
							if( !( SDL_GetModState() & KMOD_CTRL && ( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C' || e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V' ) ) )
							{
								//Append character
								if(e.text.text[0] == ' ') continue;
								PlayerHandle += e.text.text;
								renderText = true;
							}
						}
					}
					else if(where == GAME){


                        // cout << Naruto.Life << endl;

						if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F1){
							SLOT[0] = saveData(score, PlayerHandle, health_path, power_path, true);
						}
						else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F2){
							SLOT[1] = saveData(score, PlayerHandle, health_path, power_path, true);
						}
						else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F3){
							SLOT[2] = saveData(score, PlayerHandle, health_path, power_path, true);
						}
						else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F4){
							SLOT[3] = saveData(score, PlayerHandle, health_path, power_path, true);
						}
						else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F5){
							SLOT[4] = saveData(score, PlayerHandle, health_path, power_path, true);
						}
						else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F6){
							SLOT[5] = saveData(score, PlayerHandle, health_path, power_path, true);
						}
                        
                            
                            Naruto.handleEvent( e );
                            // Naruto.move();
                            // cout << e.key.keysym.sym << endl;
                            // cout << Naruto.mPosX << ' ' << Naruto.mPosY << endl;
                            // cout << Naruto.mVelX << ' ' << Naruto.mVelY << endl; 
                        
					}

					if(where == LEADERBOARD) {

						handleLeaderBoardEvents(e);
					}
				}

				if(where == USERNAME) {

					mainFont = gFont;

					//Rerender text if needed
					// cout << PlayerHandle << endl;
					if( renderText )
					{
						//Text is not empty
						if( PlayerHandle != "" )
						{
							//Render new text
							gPlayerHandleTexture.loadFromRenderedText( PlayerHandle.c_str(), textColor );
						}
						//Text is empty
						// else
						// {
						// 	//Render space texture
						// 	gPlayerHandleTexture.loadFromRenderedText( " ", textColor );
						// }
					}
					//SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255 );
						//Render background
					U_BG.render(0 , 0 );
					U_BG.render(U_BG.getWidth(), 0 );

					//Clear screen
					//SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
					//Render text textures
					gPromptTextTexture.render( ( SCREEN_WIDTH - gPromptTextTexture.getWidth() - gPlayerHandleTexture.getWidth()) / 2, SCREEN_HEIGHT / 2 - 5);
					gPlayerHandleTexture.render( ( SCREEN_WIDTH + gPromptTextTexture.getWidth() - gPlayerHandleTexture.getWidth()) / 2, SCREEN_HEIGHT / 2 - 5);

					//Update screen
					SDL_RenderPresent( gRenderer );
					
					//Disable text input
				}

				else if(where == MENU) {

                    if(whereInMenu == GAME){
                        SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255 );
                        SDL_RenderClear( gRenderer );

                        //Render background
                        gMenuPlayTexture.render( 0, 0 );
                        gMenuPlayTexture.render( 0 + gMenuPlayTexture.getWidth(), 0 );
                        SDL_RenderPresent( gRenderer );
                    }
                    if(whereInMenu == INSTRUCTION){
                        SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255 );
                        SDL_RenderClear( gRenderer );

                        //Render background
                        gMenuInsTexture.render( 0, 0 );
                        gMenuInsTexture.render( 0 + gMenuInsTexture.getWidth(), 0 );
                        SDL_RenderPresent( gRenderer );
                    }
                    if(whereInMenu == LEADERBOARD){
                        SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255 );
                        SDL_RenderClear( gRenderer );

                        //Render background
                        gMenuLeadTexture.render( 0, 0 );
                        gMenuLeadTexture.render( 0 + gMenuLeadTexture.getWidth(), 0 );
                        SDL_RenderPresent( gRenderer );
                    }
                    if(whereInMenu == EXIT){
                        SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255 );
                        SDL_RenderClear( gRenderer );

                        //Render background
                        gMenuExitTexture.render( 0, 0 );
                        gMenuExitTexture.render( 0 + gMenuExitTexture.getWidth(), 0 );
                        SDL_RenderPresent( gRenderer );
                    }

					if(whereInMenu == DEFAULT){
                        SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255 );
                        SDL_RenderClear( gRenderer );

                        //Render background
                        gMenuTexture.render( 0, 0 );
                        gMenuTexture.render(gMenuTexture.getWidth(), 0 );
                        SDL_RenderPresent( gRenderer );
                    }
				}

				else if(where == LEADERBOARD) {

					if(scoreLoaded == false) loadScoreFromFile();

					int Y = 260, rank = 0;

					mainFont = sFont;
					SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255 );
                    SDL_RenderClear( gRenderer );
                    
                    if(!whereInLeaderBoard){
                    	gLeaderBoard1.render( 0, 0 );
                    	gLeaderBoard1.render( 0 + gLeaderBoard1.getWidth(), 0 );
                    }
                    else {

                    	gLeaderBoard2.render( 0, 0 );
                    	gLeaderBoard2.render( 0 + gLeaderBoard2.getWidth(), 0 );	
                    }

					for(auto user : LeaderboardData) {
						const int total_width = 40;
			            const int s_width = strlen(user.handle);
			            const int field_width = (total_width - s_width) / 2 + s_width;
			            sprintf(leaderboardScore, "%02d%*s%*s%4d",++rank,field_width + (s_width % 2), user.handle, field_width - s_width - 1 ,"", user.score);
						
						// cout << leaderboardScore << endl;
						//sprintf(leaderboardScore,"  %2d\t\t%20"

						gLeaderBoardScores[rank - 1].loadFromRenderedText( leaderboardScore, textColor );
						gLeaderBoardScores[rank - 1].render( 374 , Y);

						Y += 37;
					}
					SDL_RenderPresent( gRenderer );
				}

                else if(where == EXIT) {
                    quit = true;
                }

				else if(where == GAME) {


					if(PauseOn){

						SDL_RenderClear(gRenderer);
						gBGTexture.render( scrollingOffset, BORDER );
						gBGTexture.render( scrollingOffset + gBGTexture.getWidth(), BORDER );
						Naruto.render();
						SDL_RenderPresent(gRenderer);
					}
					//Move the Naruto
					else {
						Naruto.hitten--;
						Naruto.hitten=max(0,Naruto.hitten);

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
							if(!bird[i].flag_of_obstacle)
							{
								if(rand()%1007==0)
								{
									bird[i].flag_of_obstacle=1;
									break;

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
						if(Naruto.hitten%5==0)
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
									// cout << "fuck noh!!" << endl;
									rocks[i].hitten=1;
									Naruto.Life-=25;
									health_path[7]++;
									gHealthTexture.loadFromFile(health_path);
									Naruto.hitten=77;
									cout << "Life :" << Naruto.Life << endl;
								}
								if(rocks[i].mPosX<=-10)
								{
									rocks[i].mPosX=SCREEN_WIDTH+rand()%15000;
									rocks[i].flag_of_obstacle=0;
									rocks[i].hitten=0;
									score+=10;
									// cout << i << endl;
								}
								if(rocks[i].mPosX < SCREEN_WIDTH)
								{


									for(int j=0;j<5;j++)
									{
										if(checkCollision(sos[j].Shuriken_rect, rocks[i].Obstacle_rect ) && sos[j].flag_of_shuriken )
										{
											// cout << "fucl yea" << endl;
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
									// cout << "fuck noh injaaa!!" << endl;
									ninja[i].hitten=1;
									Naruto.Life-=25;
									health_path[7]++;
									gHealthTexture.loadFromFile(health_path);
									Naruto.hitten=77;
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
											// cout << "fucl yea inja" << endl;
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
						for(int i=0;i < 10 ; i++ )
						{
							if(bird[i].flag_of_obstacle)
							{
								bird[i].move();
								
								if(checkCollision(Naruto.Naruto_Rect,bird[i].Moving_Obstacle_rect) && !bird[i].hitten){
									// cout << "fuck noh injaaa!!" << endl;
									bird[i].hitten=1;
									Naruto.Life-=25;
									health_path[7]++;
									gHealthTexture.loadFromFile(health_path);
									Naruto.hitten=77;
									cout << "Life :" << Naruto.Life << endl;
								}
								if(bird[i].mPosX<=-10)
								{
									bird[i].mPosX=SCREEN_WIDTH+rand()%15000;
									bird[i].flag_of_obstacle=0;
									bird[i].hitten=0;
									score+=10;
									
								}
								if(bird[i].mPosX < SCREEN_WIDTH)
								{


									for(int j=0;j<5;j++)
									{
										if(checkCollision(sos[j].Shuriken_rect, bird[i].Moving_Obstacle_rect ) && sos[j].flag_of_shuriken )
										{
											// cout << "fucl yea inja" << endl;
											score+=50;
											bird[i].mPosX=SCREEN_WIDTH+ (rand()%10)* (rand()%15000);
											bird[i].flag_of_obstacle=0;
											bird[i].hitten=0;
											sos[j].flag_of_shuriken=0;

										}
									}
								}
								bird[i].counter++;
								if(bird[i].counter==3){

									bird[i].path_of_Moving_Obstacle++;

									if(bird[i].path_of_Moving_Obstacle=='7')
										bird[i].path_of_Moving_Obstacle='1';
									bird[i].counter=0;
								}

								
								string temp2="tobi/";
								temp2+= bird[i].path_of_Moving_Obstacle;
								temp2+=".png";
								gMovingObstacle.loadFromFile(temp2);
								gMovingObstacle.render(bird[i].mPosX+24,bird[i].mPosY-33);
								string temp="bird/";
								temp+= bird[i].path_of_Moving_Obstacle;
								temp+=".png";
								gMovingObstacle.loadFromFile(temp);
								gMovingObstacle.render(bird[i].mPosX,bird[i].mPosY);
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
							// cout << "You are fucked!!" << endl;
							cout << "Your Score : " << score << endl;
							
							SDL_Delay(1000);
                            // SDL_RenderClear(gRenderer);
                             where = MENU;
                             whereInMenu = DEFAULT;
                             recall=1;
                             cout << "recalled";
							//quit=true;
							scoreData now;
							strcpy(now.handle, PlayerHandle.c_str());
							now.score = score;
							LeaderboardData.insert(now);
							updateScores();
							writeSaveData();
						}
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