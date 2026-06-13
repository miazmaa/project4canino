#include "SpriteSheet.h"

Sprite::Sprite()
{
	image = NULL;
}
Sprite::~Sprite()
{
	al_destroy_bitmap(image);
}
void Sprite::InitSprites(int width, int height)
{
	x = 80;
	y = -10;


	maxFrame = 8;
	curFrame = 0;
	frameCount = 0;
	frameDelay = 6;
	frameWidth = 50;
	frameHeight = 64;
	animationColumns = 8;
	animationDirection = 1;

	image = al_load_bitmap("guy.bmp");
	al_convert_mask_to_alpha(image, al_map_rgb(255, 0, 255));
}

void Sprite::UpdateSprites(int width, int height, int dir)
{
	int oldx = x;
	int oldy = y;
	animationDirection = dir;

	if (dir == 1) { //right key
		animationDirection = 1;
		x += 2;
		if (++frameCount > frameDelay)
		{
			frameCount = 0;
			if (++curFrame > maxFrame)
				curFrame = 1;
		}
	}
	else if (dir == 0) { //left key
		animationDirection = 0;
		x -= 2;
		if (++frameCount > frameDelay)
		{
			frameCount = 0;
			if (++curFrame > maxFrame)
				curFrame = 1;
		}
	}
	else if (dir == 3) { // UP
		y -= 2;
		if (++frameCount > frameDelay) {
			frameCount = 0;
			if (++curFrame > maxFrame) curFrame = 1;
		}
	}
	else if (dir == 4) { // DOWN
		y += 2;
		if (++frameCount > frameDelay) {
			frameCount = 0;
			if (++curFrame > maxFrame) curFrame = 1;
		}
	}
	//check for collided with foreground tiles
	if (animationDirection == 0)
	{
		if (collided(x, y + frameHeight - 5)) { //collision detection to the left
			x = oldx;
			y = oldy;
		}

	}
	else if (animationDirection == 1)
	{
		if (collided(x + frameWidth, y + frameHeight - 5)) { //collision detection to the right
			x = oldx;
			y = oldy;
		}
	}
	else if (animationDirection == 3) { 
		if (collided(x + 5, y) || collided(x + frameWidth - 5, y)) { //collision detection top tiles
			y = oldy;
		}
	}
	else if (animationDirection == 4) { // Moving Down
		if (collided(x + 5, y + frameHeight) || collided(x + frameWidth - 5, y + frameHeight)) { // collision detection bottom tiles
			y = oldy;
		}
	}
}

bool Sprite::CollisionEndBlock()
{ 
	if (animationDirection == 1) { //if moving right
		if (endValue(x + frameWidth + 1, y + frameHeight - 5))
			return true;
	}
	else
	{
		if (endValue(x + (frameWidth / 2), y + frameHeight - 1))
			return true;
	}
	return false;
}

void Sprite::DrawSprites(int xoffset, int yoffset)
{
	int fx = (curFrame % animationColumns) * frameWidth;
	int fy = (curFrame / animationColumns) * frameHeight;

	if (animationDirection == 1 || animationDirection == 3 || animationDirection == 4) {
		al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x - xoffset, y - yoffset, 0);
	}
	else if (animationDirection == 0) {
		al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x - xoffset, y - yoffset, ALLEGRO_FLIP_HORIZONTAL);
	}
	else if (animationDirection == 2) {
		al_draw_bitmap_region(image, 0, 0, frameWidth, frameHeight, x - xoffset, y - yoffset, 0);

	}
}
