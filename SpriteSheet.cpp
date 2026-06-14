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
	y = 0;
	frameWidth = 64;
	frameHeight = 64;
	image = al_load_bitmap("boy.png");
	if (image) {
		al_convert_mask_to_alpha(image, al_map_rgb(255, 255, 255));
	}
	int boy_width = al_get_bitmap_width(image);
	int boy_height = al_get_bitmap_height(image);
	maxFrame = 3;
	curFrame = 0;
	frameWidth = boy_width / 4;
	frameHeight = boy_height / 4;
	col = 0;
	row = 0;
	frameCount = 0;
	frameDelay = 6;
	scale = 0.5f;
}

void Sprite::UpdateSprites(int width, int height, int dir)
{
	int oldx = x;
	int oldy = y;

	if (dir == 1) { // right key
		animationDirection = 1;
		row = 3; 
		x += 2;
		if (++frameCount > frameDelay) {
			frameCount = 0;
			if (++curFrame > maxFrame) curFrame = 0; 
		}
	}
	else if (dir == 0) { // left key
		animationDirection = 0;
		row = 2; 
		x -= 2;
		if (++frameCount > frameDelay) {
			frameCount = 0;
			if (++curFrame > maxFrame) curFrame = 0; 
		}
	}
	else if (dir == 2) { //up 
		animationDirection = 2;
		row = 1;
		y -= 2;
		if (++frameCount > frameDelay) {
			frameCount = 0;
			if (++curFrame > maxFrame) curFrame = 0;
		}
	}
	else if (dir == 3) { // down
		animationDirection = 3;
		row = 0;
		y += 2;
	}
	else {
		if (dir != -1) {
			animationDirection = dir;
		}
		curFrame = 0;
	}
	col = curFrame;
	int spritew = frameWidth * scale;
	int spriteh = frameHeight * scale;

	int left = x;
	int right = x + spritew - 1;
	int top = y;
	int bottom = y + spriteh - 1;

	if (animationDirection == 0) //left
	{
		if (collided(left, top) || collided(left, bottom)) {
			x = oldx;
			y = oldy;
		}
	}
	else if (animationDirection == 1) //right
	{
		if (collided(right, top) || collided(right, bottom)) {
			x = oldx;
			y = oldy;
		}
	}
	else if (animationDirection == 2) //up
	{
		if (collided(left, top) || collided(right, top)) {
			x = oldx;
			y = oldy;
		}
	}
	else if (animationDirection == 3) // down
	{
		if (collided(left, bottom) || collided(right, bottom)) {
			x = oldx;
			y = oldy;
		}
	}
}

bool Sprite::CollisionEndBlock()
{ 
	int width = frameWidth * scale;
	int height = frameHeight * scale;

	return endValue(x + width / 2, y + height / 2);
}

void Sprite::DrawSprites(int xoffset, int yoffset)
{
	int fx = col * frameWidth;
	int fy = row * frameHeight;
	al_draw_scaled_bitmap(image,fx, fy,frameWidth, frameHeight,x - xoffset, y - yoffset, frameWidth * scale, frameHeight * scale, 0 );
}