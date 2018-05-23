
#include "Genio.h"


struct object {
	G_Rect pos;
	G_Texture* texture;
	bool exist;
	bool broken;
};
enum GameState { STARTMENU, START, PLAY, WIN, LOSE,  SHOWLEVEL , PAUSE, EXITE};


const int level_count = 16;
int space = 60;
int box_speed = 3;
int HideBox = 0;
int broken_box_counter = 0;
int FPS = 100;
int startLoop, endLoop;
int delay = 1000 / FPS;

bool broken = false;
bool lose = true;
bool done;
bool all_broken = false;

object boxes[level_count];
object laser;
object pinR;
object pinL;
object play_btn;
object Txtlevel_btn;
object boxCounter_btn;
object back_btn;
object tryagain_btn;
object success_btn;
object pause_btn;
object levelmenu_btn;
object level1_btn;
object level2_btn;
object backtogame_btn;

SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;

G_Texture* box;
G_Texture* brokenBox;
G_Texture* Laser_pic;
G_Texture* redLeaser_pic;
G_Texture* level_font;
G_Texture* boxCounter_font;

G_Texture *level_Tex[level_count+1]; 

G_Font* font1;
G_Font* font2;

G_Rect windowPos;
G_Rect levelFont_pos;
G_Rect boxCounter_pos;
G_Rect boxCounter_pos2;

G_Sound* broken_sound;
G_Sound* laser_sound;
G_Sound* gameover_soun;
G_Sound* success_sound;

void initialize();
void start();
void play();
void draw();
void load();
bool colision(object obj1, object obj2);
bool check_Colision();
void loseGame();
void winGame();
void startMenu();
void pauseMenu();
void showlevel();
void startMenu();
void destroy();

GameState gameState = STARTMENU;

int main(int argc, char* args[])
{
	G_InitSDL();
	windowPos.x = SDL_WINDOWPOS_UNDEFINED;
	windowPos.y = SDL_WINDOWPOS_UNDEFINED;
	windowPos.w = 600;
	windowPos.h = 800;


	G_CreateWindow("Impossible Laser", windowPos, 255,255,255);
	load();
	initialize();

	while (gameState != EXITE)
	{
		startLoop = G_GetTicks();

		//Clear screen
		G_Update();
		
		switch (gameState)
		{
				
		case STARTMENU:    startMenu();        break;

		case START:			start();		break;

		case SHOWLEVEL:		showlevel();    break;

		case PLAY:		play();			break;
		
		case LOSE:     loseGame();         break;

		case WIN:     winGame();          break;

		case PAUSE:    pauseMenu();        break;
		
		case EXITE:							break;
		}
	}
	destroy();


	return 0;
}

void load()
{
	box = G_LoadImage("box.png");
	brokenBox = G_LoadImage("broken.png");
	Laser_pic = laser.texture = G_LoadImage("laser.png",255,255,255);
	redLeaser_pic = G_LoadImage("redlaser.png",255,255,255);
	pinL.texture = pinR.texture = G_LoadImage("pin.png",255,255,255);
	
	play_btn.texture = G_LoadImage("play_btn.png");
	Txtlevel_btn.texture = boxCounter_btn.texture = G_LoadImage("text_btn.png");
	back_btn.texture = G_LoadImage("back_btn.png");
	tryagain_btn.texture = G_LoadImage("tryagain_btn.png");
	success_btn.texture = G_LoadImage("success_btn.png");
	pause_btn.texture = G_LoadImage("pause_btn.png");
	levelmenu_btn.texture = G_LoadImage("levelmenu_btn.png");
	level1_btn.texture = G_LoadImage("level1_btn.png");
	level2_btn.texture = G_LoadImage("level2_btn.png");
	backtogame_btn.texture = G_LoadImage("backtogame_btn.png");

	font1 = G_OpenFont("arial.ttf", 25);
	font2 = G_OpenFont("arial.ttf", 10);

	level_font = G_LoadFont(font1, " LEVEL 1", 0, 0, 0);
	boxCounter_font = G_LoadFont(font1, " / 16", 0, 0, 0);

	level_Tex[0] = G_LoadFont(font2, "0", 0, 0, 0);
	level_Tex[1] = G_LoadFont(font2, "1", 0, 0, 0);
	level_Tex[2] = G_LoadFont(font2, "2", 0, 0, 0);
	level_Tex[3] = G_LoadFont(font2, "3", 0, 0, 0);
	level_Tex[4] = G_LoadFont(font2, "4", 0, 0, 0);
	level_Tex[5] = G_LoadFont(font2, "5", 0, 0, 0);
	level_Tex[6] = G_LoadFont(font2, "6", 0, 0, 0);
	level_Tex[7] = G_LoadFont(font2, "7", 0, 0, 0);
	level_Tex[8] = G_LoadFont(font2, "8", 0, 0, 0);
	level_Tex[9] = G_LoadFont(font2, "9", 0, 0, 0);
	level_Tex[10] = G_LoadFont(font2, "10", 0, 0, 0);
	level_Tex[11] = G_LoadFont(font2, "11", 0, 0, 0);
	level_Tex[12] = G_LoadFont(font2, "12", 0, 0, 0);
	level_Tex[13] = G_LoadFont(font2, "13", 0, 0, 0);
	level_Tex[14] = G_LoadFont(font2, "14", 0, 0, 0);
	level_Tex[15] = G_LoadFont(font2, "15", 0, 0, 0);
	level_Tex[16] = G_LoadFont(font2, "16", 0, 0, 0);
	
	broken_sound = G_LoadSound("broken.wav");
	laser_sound = G_LoadSound("laser.wav");
	gameover_soun = G_LoadSound("gameover.wav");
	success_sound = G_LoadSound("success.wav");

}

void startMenu()
{
	initialize();
	for (int i = 0; i < level_count; i++)
	{
		G_Draw(boxes[i].texture, &boxes[i].pos, false);
	}
	G_Draw(Txtlevel_btn.texture, &Txtlevel_btn.pos, false);
	G_Draw(boxCounter_btn.texture, &boxCounter_btn.pos, false);
	G_Draw(play_btn.texture, &play_btn.pos, false);
	G_Draw(levelmenu_btn.texture, &levelmenu_btn.pos, false);

	G_Draw(level_font, &levelFont_pos, false);
	G_Draw(boxCounter_font, &boxCounter_pos, false);

	switch (G_Event())
	{
	case G_QUIT:
		gameState = EXITE;
		break;
	case G_MOUSEBUTTONDOWN:
	{
		if (G_BUTTON_LEFT)
		{
			if (G_motion.x > play_btn.pos.x &&
				G_motion.x < play_btn.pos.x + play_btn.pos.w)
				if (G_motion.y > play_btn.pos.y &&
					G_motion.y < play_btn.pos.y + play_btn.pos.h)
					{
						gameState = START;
						//GM_LEFT.down = false;
					}
				else if (G_motion.x > levelmenu_btn.pos.x &&
					G_motion.x < levelmenu_btn.pos.x + levelmenu_btn.pos.w)
					if (G_motion.y > levelmenu_btn.pos.y &&
						G_motion.y < levelmenu_btn.pos.y + levelmenu_btn.pos.h)
					{
						gameState = SHOWLEVEL;
					}
		}break;
	}
		
		break;

	default:
		break;
	}

}


void initialize()
{
for (int i = 0; i < level_count; i++)
{
boxes[i].texture = box;
boxes[i].exist = true;
boxes[i].broken = false;
boxes[i].pos.w = boxes[i].pos.h = 50;
boxes[i].pos.x = windowPos.w / 2 - 25;
if (i == 0)			boxes[i].pos.y = 10;
else				boxes[i].pos.y = boxes[i - 1].pos.y + boxes[0].pos.h + space;

}


laser.exist = true;
laser.pos = { 0,windowPos.h / 2 + 15 ,windowPos.w ,8 };

pinR.exist = true;
pinR.pos = { windowPos.w - 40 ,windowPos.h / 2 ,40,40 };

pinL.exist = true;
pinL.pos = { 0 ,windowPos.h / 2 ,40,40 };

play_btn.exist = true;
play_btn.pos = { 100,windowPos.h - 300,400,100 };

Txtlevel_btn.exist = true;
Txtlevel_btn.pos = { 0,0,200,50 };

tryagain_btn.exist = true;
tryagain_btn.pos = { 200, windowPos.h - 300,200,100 };

back_btn.exist = true;
back_btn.pos = { 95,windowPos.h - 300,100,100 };

success_btn.exist = true;
success_btn.pos = { 100,windowPos.h - 300,400,100 };

levelmenu_btn.exist = true;
levelmenu_btn.pos = { 100,windowPos.h - 195,100,100 };

level1_btn.exist = true;
level1_btn.pos = { 20,20,100,100 };

level2_btn.exist = true;
level2_btn.pos = { 125,20,100,100 };

pause_btn.exist = true;
pause_btn.pos = { 5,windowPos.h - 25,20,20 };

backtogame_btn.exist = true;
backtogame_btn.pos = { 150,windowPos.h - 500,300,100 };

boxCounter_btn.exist = true;
boxCounter_btn.pos = { windowPos.w - 200,0,200,50 };

levelFont_pos = { 20,0,100,50 };

boxCounter_pos = { windowPos.w - 100,0,60,50 };

boxCounter_pos2 = { windowPos.w - 185,0,50,50 };

}



void start()
{
	initialize();
	done = false;
	lose = true;
	gameState = PLAY;

}


void showlevel()
{
	G_Draw(level1_btn.texture, &level1_btn.pos, false);
	G_Draw(level2_btn.texture, &level2_btn.pos, false);

	switch (G_Event())
	{
	case G_QUIT:
		gameState = EXITE;
		break;
	case G_MOUSEBUTTONDOWN:
	{
		if (G_BUTTON_LEFT)
		{
			if (G_motion.x > level1_btn.pos.x &&
				G_motion.x < level1_btn.pos.x + level1_btn.pos.w)
				if (G_motion.y > level1_btn.pos.y &&
					G_motion.y < level1_btn.pos.y + level1_btn.pos.h)
				{
					gameState = START;
				}
		}
	}
	}
}

void draw()
{
	
	for (int i = 0; i < level_count; i++)
	{
		boxes[i].pos.y -= box_speed;

		if (boxes[i].exist)
		{
			G_Draw(boxes[i].texture, &boxes[i].pos, false);
		}
		if (boxes[0].pos.y + boxes[0].pos.h < 0 && HideBox == 0)
		{
			
			if (boxes[level_count - 1].pos.y + boxes[level_count - 1].pos.h + space < windowPos.h && 
				boxes[level_count - 1].pos.y + boxes[level_count - 1].pos.h + space > 0)
			{
				
				if (windowPos.h - boxes[level_count - 1].pos.y + boxes[level_count - 1].pos.h < space )
				{
					boxes[0].pos.y = boxes[level_count - 1].pos.y + boxes[level_count - 1].pos.h + space;
				}
				else
				{
					boxes[0].pos.y = windowPos.h;
				}
				HideBox++;
			}
			else if (boxes[level_count - 1].pos.y + boxes[level_count - 1].pos.h + space <= 0 )
			{
				if (HideBox >1)
				{
					boxes[HideBox].pos.y = boxes[HideBox - 1].pos.y + boxes[HideBox - 1].pos.h + space;
					HideBox++;
				}
			}
		}
		else if (boxes[HideBox].pos.y + boxes[HideBox].pos.h < 0)
		{
			
			if (boxes[HideBox - 1].pos.y + boxes[HideBox - 1].pos.h <= windowPos.h)
			{
				boxes[HideBox].pos.y = boxes[HideBox - 1].pos.y + boxes[HideBox - 1].pos.h + space;
				HideBox++;
			}
		}

		if (HideBox == level_count)			HideBox = 0;
	}

	G_Draw(laser.texture, &laser.pos, false);
	G_Draw(pinL.texture, &pinL.pos, false);
	G_DrawEx(pinR.texture, &pinR.pos,flip, false);
	G_Draw(Txtlevel_btn.texture, &Txtlevel_btn.pos, false);
	G_Draw(boxCounter_btn.texture, &boxCounter_btn.pos, false);
	G_Draw(pause_btn.texture, &pause_btn.pos, false);

	G_Draw(level_font, &levelFont_pos, false);
	for (int  i = 0; i <= level_count; i++)
	{
		if ( !boxes[i].exist )
		{
			broken_box_counter++;
		}
	}
	G_Draw(level_Tex[broken_box_counter], &boxCounter_pos2, false);
	G_Draw(boxCounter_font, &boxCounter_pos, false);
	
}

bool colision(object obj1, object obj2)
{
	if (((obj1.pos.x >= obj2.pos.x) && (obj1.pos.x - obj2.pos.x)<obj2.pos.w) ||
		((obj2.pos.x >= obj1.pos.x) && ((obj2.pos.x - obj1.pos.x)<obj1.pos.w)))
	{
		if (((obj1.pos.y >= obj2.pos.y) && (obj1.pos.y - obj2.pos.y)<obj2.pos.h) ||
			((obj2.pos.y >= obj1.pos.y) && ((obj2.pos.y - obj1.pos.y)<obj1.pos.h)))
			return true;
	}
	return false;
}


void play()
{
	draw();
	broken_box_counter = 0;
	
	//cout << G_Event()<<endl;
	switch (G_Event())
	{
		case G_QUIT:
			gameState = EXITE;
			break;
		case G_KEYDOWN:
		{

			switch (G_Keyboard)
			{
				case GK_SPACE:
				{
					done = true;
					laser.texture = redLeaser_pic;
					G_Draw(laser.texture, &laser.pos, false);
					G_Delay(2);
					laser.texture = Laser_pic;

					for (int i = 0; i < level_count; i++)
					{
						if (boxes[i].exist)
						{
							if (colision(laser, boxes[i]))
							{
								boxes[i].exist = false;
								boxes[i].broken = true;
								G_PlaySound(broken_sound, 1);
							}
						}
					}
				}break;
			}
		}break;
		case G_KEYUP:
		{
			done = false;
			lose = true;
		}break;
		case G_MOUSEBUTTONDOWN:
		{
			if (G_BUTTON_LEFT)
			{
				if (G_motion.x > pause_btn.pos.x &&
					G_motion.x < pause_btn.pos.x + pause_btn.pos.w)
					if (G_motion.y > pause_btn.pos.y &&
						G_motion.y < pause_btn.pos.y + pause_btn.pos.h)
					{
						gameState = PAUSE;
						return;
					}
			}
		}break;
		
	}
	all_broken = true;
	//show broken picture
	for (int i = 0; i < level_count; i++)
	{
		if (boxes[i].broken)
		{
			lose = false;
			boxes[i].exist = false;
			boxes[i].texture = brokenBox;
			boxes[i].pos.y -= 5;
			G_Draw(boxes[i].texture, &boxes[i].pos, false);
			boxes[i].broken = false;
		}
		else if (boxes[i].exist)			all_broken = false; // chek if there is a unbroken box the counter change to false
		
	}
	if (lose && done)
	{
		gameState = LOSE;
	
	}
	if (all_broken)
	{
		gameState = WIN;
		G_Delay(500);
	
	}

	endLoop = G_GetTicks() - startLoop;
	if (endLoop<delay)
	{
		G_Delay(delay - endLoop);
	}

}


void winGame()
{
	G_Draw(success_btn.texture, &success_btn.pos, false);
	G_Draw(levelmenu_btn.texture, &levelmenu_btn.pos, false);

	switch (G_Event())
	{
	case G_QUIT:
		gameState = EXITE;
		break;
	case G_MOUSEBUTTONDOWN:
	{
		if (G_BUTTON_LEFT)
		{
			if (G_motion.x > success_btn.pos.x &&
				G_motion.x < success_btn.pos.x + success_btn.pos.w)
				if (G_motion.y > success_btn.pos.y &&
					G_motion.y < success_btn.pos.y + success_btn.pos.h)
				{
					gameState = STARTMENU;
					//GM_LEFT.down = false;
				}
				else if (G_motion.x > levelmenu_btn.pos.x &&
					G_motion.x < levelmenu_btn.pos.x + levelmenu_btn.pos.w)
					if (G_motion.y > levelmenu_btn.pos.y &&
						G_motion.y < levelmenu_btn.pos.y + levelmenu_btn.pos.h)
					{
						gameState = SHOWLEVEL;
						//GM_LEFT.down = false;
					}
		}
	}
	}

}

void loseGame()
{
	for (int i = 0; i < level_count; i++)
		if (boxes[i].exist)			
			G_Draw(boxes[i].texture, &boxes[i].pos, false);
		
	G_Draw(back_btn.texture, &back_btn.pos, false);
	G_Draw(tryagain_btn.texture, &tryagain_btn.pos, false);
	switch (G_Event())
	{
	case G_QUIT:
		gameState = EXITE;
		break;
	case G_MOUSEBUTTONDOWN:
	{
		if (G_BUTTON_LEFT)
		{
			if (G_motion.x > back_btn.pos.x &&
				G_motion.x < back_btn.pos.x + back_btn.pos.w)
				if (G_motion.y > back_btn.pos.y &&
					G_motion.y < back_btn.pos.y + back_btn.pos.h)
				{
					gameState = STARTMENU;
					//GM_LEFT.down = false;

				}

			if (G_motion.x > tryagain_btn.pos.x &&
				G_motion.x < tryagain_btn.pos.x + tryagain_btn.pos.w)
				if (G_motion.y > tryagain_btn.pos.y &&
					G_motion.y < tryagain_btn.pos.y + tryagain_btn.pos.h)
				{
					gameState = START;
					//GM_LEFT.down = false;
				}
		}
	}
	}
	
}

void pauseMenu()
{
	for (int i = 0; i < level_count; i++)
		if (boxes[i].exist)				G_Draw(boxes[i].texture, &boxes[i].pos, false);

	G_Draw(backtogame_btn.texture, &backtogame_btn.pos, false);
	switch (G_Event())
	{
	case G_QUIT:
		gameState = EXITE;
		break;
	case G_MOUSEBUTTONDOWN:
	{
		if (G_BUTTON_LEFT)
		{
			if (G_motion.x > backtogame_btn.pos.x &&
				G_motion.x < backtogame_btn.pos.x + backtogame_btn.pos.w)
				if (G_motion.y > backtogame_btn.pos.y &&
					G_motion.y < backtogame_btn.pos.y + backtogame_btn.pos.h)
				{
					gameState = PLAY;
					//GM_LEFT.down = false;
				}
		}
	}break;
	}
	
}


void destroy()
{
G_DestroyTexture(box);
G_QuitSDL();
//G_FreeMusic();
//G_FreeSound();

}

