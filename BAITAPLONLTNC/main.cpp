#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <chrono>
#include <SDL_ttf.h>
#include "SDL_utils.h"
#include "Game.h"
#include "Gallery.h"
#include <SDL_mixer.h>
typedef basic_stringstream<char> stringstream;
using namespace std;

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 720;
const char WINDOW_TITLE[] = "Spaceinvader Snake";

const int BOARD_WIDTH = 25;
const int BOARD_HEIGHT = 18;
const int CELL_SIZE = 40;
int mWidth;
int mHeight;
const SDL_Color BOARD_COLOR = {10, 10, 10};
const SDL_Color LINE_COLOR = {20, 20, 20};

const double STEP_DELAY = 0.195;
#define CLOCK_NOW chrono::system_clock::now
typedef chrono::duration<double> ElapsedTime;

void renderSplashScreen(SDL_Window*window);
void renderGamePlay(SDL_Renderer* renderer, const Game& game);
void renderGameOver(SDL_Renderer* renderer, const Game& game);
void interpretEvent(SDL_Event e, Game& game);
void updateRankingTable(const Game& game);
/////////
void free();
bool loadFromRenderedText( string textureText, SDL_Color textColor );
Gallery* gallery = nullptr; // global picture manager
TTF_Font *gGameOver;
TTF_Font *gPlayAgainWin;
TTF_Font *gPlayAgainLose;
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* mTexture;
int main(int argc, char* argv[])
{
    initSDL(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    gallery = new Gallery(renderer);
    Game game(BOARD_WIDTH, BOARD_HEIGHT);
    if(Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640)<0)cout<<"error: "<<Mix_GetError()<<endl;
    srand(time(0));
    SDL_Event e;
    ////////////
    Mix_Music* bmg=Mix_LoadMUS("tobu-puzzle.mp3");
    Mix_Chunk* effect=Mix_LoadWAV("click.wav");
    Mix_PlayMusic(bmg,1);
    renderSplashScreen(window);

    bool quit=false, restart=false;
    while(!quit){
            Game game(BOARD_WIDTH, BOARD_HEIGHT);
    auto start = CLOCK_NOW();
    renderGamePlay(renderer, game);
    while (game.isGameRunning()){
        while (SDL_PollEvent(&e)) {
            if(e.type==SDL_KEYDOWN){
                switch(e.key.keysym.sym){
                case SDLK_p:
                    if(!Mix_PlayingMusic())Mix_PlayMusic(bmg,1);
                    else{if(Mix_PausedMusic())Mix_ResumeMusic();
                    else{
                        Mix_PauseMusic();
                     }
                    }
                default:
                     Mix_PlayChannel(-1,effect,0);
                }
            }
            interpretEvent(e, game);
        }

        auto end = CLOCK_NOW();
        ElapsedTime elapsed = end-start;
        if (elapsed.count() > STEP_DELAY) {
            game.nextStep();
            renderGamePlay(renderer, game);
            start = end;
        }
        SDL_Delay(0);
      }
      game.playagain(quit);

    if(quit){
    renderGameOver(renderer, game);
    updateRankingTable(game);
    SDL_Delay(2000);
    delete gallery;
    }
    }

    quitSDL(window, renderer);
    return 0;
}
/*
void loadfont(){
	//Open the font

	gGameOver = TTF_OpenFont( "COMIC.TTF", 30 );
	if( gGameOver == NULL )
	{
	    cout << "Failed to load DTM-Sans font! SDL_ttf Error: " << TTF_GetError() << endl;
	}
	else
	{
		//Render text
		SDL_Color textColor = { 255, 255, 255 };
		if( !loadFromRenderedText( "trash lmao", textColor ) )
		{
			cout << "Failed to render text texture!\n";
		}
	}

	gPlayAgainWin = TTF_OpenFont( "COMIC.TTF", 40 );
	if( gPlayAgainWin == NULL )
	{
	    cout << "Failed to load DTM-Sans font! SDL_ttf Error: " << TTF_GetError() << endl;
	}
	else
    {
        //Render text
		SDL_Color playAgainWin = { 255, 255, 255 };
		if( !loadFromRenderedText( "s to try again?", playAgainWin ) )
		{
			cout << "Failed to render text texture!\n";
        }
    }

    gPlayAgainLose = TTF_OpenFont( "COMIC.TTF", 40 );
    if( gPlayAgainLose == NULL )
	{
	    cout << "Failed to load DTM-Sans font! SDL_ttf Error: " << TTF_GetError() << endl;;
	}
	else
    {
        //Render text
		SDL_Color playAgainLose = { 255, 255, 255 };
		if( !loadFromRenderedText( "s to try again", playAgainLose ) )
		{
			cout << "Failed to render text texture!\n";

		}
    }
}
bool loadFromRenderedText( string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
    free();
	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gGameOver, textureText.c_str(), textColor );
	if( textSurface == NULL )
	{
	    cout << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << endl;
	}
	else
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( renderer, textSurface );
		if( mTexture == NULL )
		{
		    cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << endl;
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

	//Return success
	return mTexture != NULL;
}
void free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}*/
float generateRandomNumber()
{
    return (float) rand() / RAND_MAX;
}

void renderSplashScreen(SDL_Window* window)
{   SDL_Surface* test=nullptr;
    SDL_Surface* loading=nullptr;
    SDL_Rect dest;
 dest.x = -170;
 dest.y = -50;
 dest.w = 110;
 dest.h = 110;
 test=SDL_GetWindowSurface(window);
    loading = SDL_LoadBMP( "loading1.bmp" );
    SDL_BlitSurface( loading, NULL,test ,&dest );
    SDL_UpdateWindowSurface( window );
    cout << "Press any key to start game" << endl;
    waitUntilKeyPressed();
    SDL_FreeSurface( test );
    SDL_FreeSurface( loading);
}

void drawCell(SDL_Renderer* renderer, int left, int top, Position pos, SDL_Texture* texture)
{
	SDL_Rect cell;
	cell.x = left + pos.x * CELL_SIZE + 5;
	cell.y = top + pos.y * CELL_SIZE + 5;
	cell.w = CELL_SIZE-10;
	cell.h = CELL_SIZE-10;
	SDL_RenderCopy(renderer, texture, NULL, &cell);
}

void drawMouse(SDL_Renderer* renderer, int left, int top, Position pos)
{
    drawCell(renderer, left, top, pos, gallery->getImage(PIC_MOUSE));
}

void drawSnake(SDL_Renderer* renderer, int left, int top, vector<Position> pos)
{
	// snake's head
	drawCell(renderer, left, top, pos[pos.size()-1], gallery->getImage(PIC_SNAKE_HEAD));

	// snake's body
    for (int i = pos.size() - 2; i >= 0; i--) {
        SDL_Texture* texture = gallery->getImage(
			pos[i].y == pos[i+1].y ? PIC_SNAKE_HORIZONTAL : PIC_SNAKE_VERTICAL);
        drawCell(renderer, left, top, pos[i], texture);
    }
}

void drawVerticalLine(SDL_Renderer* renderer, int left, int top, int cells)
{
    SDL_SetRenderDrawColor(renderer, LINE_COLOR.r, LINE_COLOR.g, LINE_COLOR.b, 0);
    SDL_RenderDrawLine(renderer, left, top, left, top + cells * CELL_SIZE);
}

void drawHorizontalLine(SDL_Renderer* renderer, int left, int top, int cells)
{
    SDL_SetRenderDrawColor(renderer, LINE_COLOR.r, LINE_COLOR.g, LINE_COLOR.b, 0);
    SDL_RenderDrawLine(renderer, left, top, left + cells * CELL_SIZE, top);
}

void renderGamePlay(SDL_Renderer* renderer, const Game& game)
{
    int top =0, left = 0;
    SDL_SetRenderDrawColor(renderer, BOARD_COLOR.r, BOARD_COLOR.g, BOARD_COLOR.b, 0);
    SDL_RenderClear(renderer);

    for (int x = 0; x <= BOARD_WIDTH; x++)
        drawVerticalLine(renderer, left + x*CELL_SIZE, top, BOARD_HEIGHT);
    for (int y = 0; y <= BOARD_HEIGHT; y++)
        drawHorizontalLine(renderer, left, top+y * CELL_SIZE, BOARD_WIDTH);

    drawMouse(renderer, left, top, game.getMousePosition());
    drawSnake(renderer, left, top, game.getSnakePositions());

    SDL_RenderPresent(renderer);
}

void renderGameOver(SDL_Renderer* renderer, const Game& game)
{
   int top =0, left = 0;
    SDL_SetRenderDrawColor(renderer, 100, 1000, 100,0 );
    SDL_RenderClear(renderer);

    for (int x = 0; x <= BOARD_WIDTH; x++)
        drawVerticalLine(renderer, left + x*CELL_SIZE, top, BOARD_HEIGHT);
    for (int y = 0; y <= BOARD_HEIGHT; y++)
        drawHorizontalLine(renderer, left, top+y * CELL_SIZE, BOARD_WIDTH);

    SDL_RenderPresent(renderer);
}

void interpretEvent(SDL_Event e, Game& game)
{
    if (e.type == SDL_KEYUP) {
        switch (e.key.keysym.sym) {
        	case SDLK_UP:
        	    game.processUserInput(UP);
        	break;
        	case SDLK_DOWN:
        	    game.processUserInput(DOWN);
        	break;
        	case SDLK_LEFT:
        	    game.processUserInput(LEFT);
            break;
        	case SDLK_RIGHT:
        	    game.processUserInput(RIGHT);
        	break;
        	case SDLK_w:
        	    game.processUserInput(UP);
        	break;
        	case SDLK_s:
        	    game.processUserInput(DOWN);
        	break;
        	case SDLK_a:
        	    game.processUserInput(LEFT);
        	break;
        	case SDLK_d:
        	    game.processUserInput(RIGHT);
        	break;
        }
    }
}

void updateRankingTable(const Game& game)
{
  cout<<Game::getScore;
}
///////////////////////////////////////////
void Game::playagain(bool &quit){
      SDL_Event e;
    while (SDL_PollEvent(&e)!=0) {
        if (e.key.keysym.sym==SDLK_ESCAPE) {
            quit=true;
        }
        else{
            if(e.key.keysym.sym==SDLK_q){
            quit=false;
            status=GAME_RUNNING;
            cout<<1;
        }
    }
    }
}
