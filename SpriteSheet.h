#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <iostream>
using namespace std;
int collided(int x, int y);
bool endValue(int x, int y);
class Sprite
{
    friend int collided(int x, int y);
    friend bool endValue(int x, int y);

public:
    Sprite();
    ~Sprite();
    void InitSprites(int width, int height);
    void UpdateSprites(int width, int height, int dir);
    void DrawSprites(int xoffset, int yoffset);
    bool CollisionEndBlock();
    float getX() { return x; }
    float getY() { return y; }
    int getWidth() { return frameWidth; }
    int getHeight() { return frameHeight; }

private:
    float x;
    float y;
    int maxFrame;
    int curFrame;
    int frameCount;
    int frameDelay;
    int frameWidth;
    int frameHeight;
    int animationColumns;
    int animationDirection;
    int col; 
    float scale;
    int row;   
    ALLEGRO_BITMAP* image;
};