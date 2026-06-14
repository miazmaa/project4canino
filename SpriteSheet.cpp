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
	float scale = 0.5f;
	maxFrame = 3;
	curFrame = 0;
	frameWidth = boy_width / 4;
	frameHeight = boy_height / 4;
	col = 0;
	row = 0;
	frameCount = 0;
	frameDelay = 6;

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
	//check for collided with foreground tiles
	if (animationDirection == 0)
	{
		if (collided(x, y + frameHeight)) { //collision detection to the left
			x = oldx;
			y = oldy;
		}

	}
	else if (animationDirection == 1)
	{
		if (collided(x + frameWidth, y + frameHeight)) { //collision detection to the right
			x = oldx;
			y = oldy;
		}
	}
	else if (animationDirection == 2) { // up
		if (collided(x, y)) {
			x = oldx;
			y = oldy;
		}
	}
	else if (animationDirection == 3) { // down
		if (collided(x, y + frameHeight)) {
			x = oldx;
			y = oldy;
		}
	}
}

bool Sprite::CollisionEndBlock()
{ 
	return endValue(x + frameWidth / 2, y + frameHeight / 2);
}

void Sprite::DrawSprites(int xoffset, int yoffset)
{
	int fx = col * frameWidth;
	int fy = row * frameHeight;
	float scale = 0.5f;
	al_draw_scaled_bitmap(image,fx, fy,frameWidth, frameHeight,x - xoffset, y - yoffset, frameWidth * scale, frameHeight * scale, 0 );
}