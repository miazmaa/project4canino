#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "SpriteSheet.h"
#include "mappy_A5.h"
#include <iostream>
using namespace std;

int collided(int x, int y);  //Tile Collision
bool endValue(int x, int y); //End Block with the User Value = 8
int main(void)
{
	const int WIDTH = 900;
	const int HEIGHT = 480;
	bool keys[] = { false, false, false, false };
	enum KEYS { UP, DOWN, LEFT, RIGHT };
	//variables
	bool done = false;
	bool render = false;
	int dir = -1;
	//Player Variable
	Sprite player;
	bool paused = false;
	bool gameOver = false;
	int currentLevel = 1;
	double pauseEndTime = 0;
	ALLEGRO_FONT* rubik = nullptr;
	double levelStartTime = 0;
	const double LEVEL_TIME_LIMIT = 60.0; 
	double timeLeft = LEVEL_TIME_LIMIT;
	//allegro variable
	ALLEGRO_DISPLAY* display = NULL;
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;
	ALLEGRO_TIMER* timer;

	//program init
	if (!al_init())										//initialize Allegro
		return -1;

	display = al_create_display(WIDTH, HEIGHT);			//create our display object

	if (!display)										//test display object
		return -1;

	//addon init
	al_install_keyboard();
	al_init_image_addon();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	rubik = al_load_ttf_font("rubik.ttf", 24, 0);
	if (!rubik) {
		cout << "Failed to load font!\n";
		return -1;
	}

	player.InitSprites(WIDTH, HEIGHT);

	int xOff = 0;
	int yOff = 0;
	if (currentLevel == 1)
		MapLoad(const_cast<char*>("level1.fmp"), 1);
	else if (currentLevel == 2)
		MapLoad(const_cast<char*>("level2.fmp"), 1);
	else
		MapLoad(const_cast<char*>("level3.fmp"), 1);
	levelStartTime = al_get_time();
	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / 60);

	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_start_timer(timer);
	//draw the background tiles
	MapDrawBG(xOff, yOff, 0, 0, WIDTH - 1, HEIGHT - 1);

	//draw foreground tiles
	MapDrawFG(xOff, yOff, 0, 0, WIDTH - 1, HEIGHT - 1, 0);
	player.DrawSprites(0, 0);
	al_flip_display();
	al_clear_to_color(al_map_rgb(0, 0, 0));
	while (!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			render = true;
			if (!paused)
			{
				double elapsed = al_get_time() - levelStartTime;
				timeLeft = LEVEL_TIME_LIMIT - elapsed;

				if (timeLeft <= 0)
				{
					gameOver = true;
					done = true; // window closes
				}
			}
			if (!paused)
			{
				if (keys[UP])
					player.UpdateSprites(WIDTH, HEIGHT, 2);
				else if (keys[DOWN])
					player.UpdateSprites(WIDTH, HEIGHT, 3);
				else if (keys[LEFT])
					player.UpdateSprites(WIDTH, HEIGHT, 0);
				else if (keys[RIGHT])
					player.UpdateSprites(WIDTH, HEIGHT, 1);
				else
					player.UpdateSprites(WIDTH, HEIGHT, -1);
			}
			if (player.CollisionEndBlock() && !paused)
			{
				paused = true;
				pauseEndTime = al_get_time() + 5.0; 
			}
			if (paused)                                      
			{
				if (al_get_time() >= pauseEndTime)             
				{
					paused = false;                           
					currentLevel++; 

					MapFreeMem();

					if (currentLevel == 1)
						MapLoad(const_cast<char*>("level1.fmp"), 1);
					else if (currentLevel == 2)
						MapLoad(const_cast<char*>("level2.fmp"), 1);
					else if (currentLevel == 3)
						MapLoad(const_cast<char*>("level3.fmp"), 1);
					else
						gameOver = true;  
					levelStartTime = al_get_time();
				}
			}
			render = true;

		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				if (!paused) done = true;
				break;
			case ALLEGRO_KEY_UP:
				if (!paused) keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				if (!paused) keys[DOWN] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				if (!paused) keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				if (!paused) keys[RIGHT] = true;
				break;

			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_UP: 
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			}
		}
		if (render && al_is_event_queue_empty(event_queue))
		{
			render = false;

			//update the map scroll position
			xOff = player.getX() + player.getWidth() - WIDTH / 2;
			yOff = player.getY() + player.getHeight() - HEIGHT / 2;

			//avoid moving beyond the map edge
			if (xOff < 0) xOff = 0;

			if (xOff > (mapwidth * mapblockwidth - WIDTH))
				xOff = mapwidth * mapblockwidth - WIDTH;
			if (yOff < 0)
				yOff = 0;
			if (yOff > (mapheight * mapblockheight - HEIGHT))
				yOff = mapheight * mapblockheight - HEIGHT;

			//draw the background tiles
			MapDrawBG(xOff, yOff, 0, 0, WIDTH, HEIGHT);

			//draw foreground tiles
			MapDrawFG(xOff, yOff, 0, 0, WIDTH, HEIGHT, 0);
			player.DrawSprites(xOff, yOff);
			al_draw_filled_rectangle(10, 10, 180, 45, al_map_rgb(0, 0, 0));
			al_draw_textf(rubik, al_map_rgb(255, 255, 255), 20, 15, 0, "Time: %d", (int)timeLeft);
			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
			if (!gameOver)
			{
				double elapsed = al_get_time() - levelStartTime;
				timeLeft = LEVEL_TIME_LIMIT - elapsed;
				if (timeLeft < 0) timeLeft = 0;
			}
		}
		if (gameOver)
		{
			al_rest(2.0);
			done = true; 
		}
	}
	MapFreeMem();
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);						//destroy our display object

	return 0;
}



int collided(int x, int y)
{
	BLKSTR* blockdata;
	blockdata = MapGetBlock(x / mapblockwidth, y / mapblockheight);
	return blockdata->tl;
}

bool endValue(int x, int y)
{

	BLKSTR* data;
	data = MapGetBlock(x / mapblockwidth, y / mapblockheight);

	if (data->user1 == 8)
	{
		return true;
	}
	else
		return false;
}