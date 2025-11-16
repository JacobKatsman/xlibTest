#include "../include/logic.h"
#include "../include/getPng.h"
#include "../include/loadSprites.h"
//#include "../include/mathCalc.h"

// объявили структуру окна
typedef struct Context {
        Display *display;
        GC gc_red;
        Window window; 
        int screen;
} Context;

// объявили структуру для разделяемой памяти
typedef struct ShmStructure {
    XShmSegmentInfo shm;
    XImage *image;
}  ShmStructure;

int clearEntropyRandomNumber(unsigned int* random_ints, size_t array_size);
int createWindowCustom (Context* p);
int checkShm (Context* p);
int createShm (Context* p, ShmStructure* s);
int generateField (Context* p,ShmStructure* s, SpriteItem* itemSprite,
                   png_bytep *row_pointers,
                   SpriteImage **row_pointers_ball);
int block1Loop (Context* p, ShmStructure* s);    
int block2Loop (Context* p, ShmStructure* s);
int switchLoop (Context* p, ShmStructure* s, SpriteItem* itemSprite);
int mainLoop (Context* p, ShmStructure* s);    
void initRenderWindow(Context* p,
                      ShmStructure* s,
                      SpriteItem* itemSprite,
                      png_bytep *row_pointers,
                      SpriteImage **row_pointers_ball);
void renderWindow(Context* p,
                  ShmStructure* s,
                  SpriteItem* itemSprite,
                  png_bytep *row_pointers,
                  SpriteImage **row_pointers_ball);

void renderLetter (ShmStructure* s, SpriteItem* itemSprite, int x1, int y1, int l);
void renderBackground (ShmStructure* s, png_bytep *row_pointers);
void renderSprite (ShmStructure* s, png_bytep *row_pointers_ball, SpriteItem* itemSprite, int l);    

// массив строк 
// extern char **msgB;

static char *msgB[] = {
    " The void has opened, stars abound",
    " The mind amazing, how many  girl around",
    " Surround people goes in the mount",    
   };


