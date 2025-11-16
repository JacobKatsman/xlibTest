#include "../include/logic.h"
typedef long double sAngel; // угол поворота

typedef struct SpriteRotate {
    sAngel x;
    sAngel y;
} SpriteRotate;

typedef struct SpriteRotateCoord {
    int    l;
    sAngel x;
    sAngel y;
    sAngel xstart;
    sAngel ystart;    
} SpriteRotateCoord;

// передаем цвета
typedef struct colorRGB {
    int R;
    int G;
    int B;
    int A;
} colorRGB;

void calcRotate(SpriteRotate* sr, SpriteRotateCoord* srCoor, SpriteItem* itemSprite);
void overlappingTransparency (colorRGB *c, png_bytep px, int pixel1[]);
int blend_channel (int B, int F,
                   double long resultOpaticy,
                   double long alfaBackground,
                   double long alfaSprite); 

