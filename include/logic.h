// объявили структуру для хранения динамичного состояния спрайта

//
// #define CHECK_RANGE(val, min, max) ((val) < (min) ? (min) : ((val) > (max) ? (max) : (val)))

//x = CHECK_RANGE(150, 0, 100); // x станет 100
//x = CHECK_RANGE(150, 0, 100); // x станет 100
//x = CHECK_RANGE(-10, 0, 100); // x станет 0
//x = CHECK_RANGE(50, 0, 100);  // x станет 50

#ifndef LOGIC_H // Защита от повторного включения
#define LOGIC_H
#define MAX 1200
#endif

#ifndef SYMBOL
 #define SYMBOL

 typedef int xCoor;
 typedef int yCoor;               
 typedef int stateActive;            // активность
 typedef long double stateAngel;     // угол поворота
 typedef int stateClockwise;         // угол поворота
 typedef int stateCost;              // стоимость
 typedef int stateSpeed;             // скорость
 typedef int stateType;              // тип спрайта

 typedef struct SpriteItem {
    xCoor xcoor;
    yCoor ycoor;
    stateActive active;
    stateAngel degrees;
    stateClockwise clockwise;
    stateCost cost;
    stateSpeed speed;
    stateType type;
    
} SpriteItem;

void initLogic();
void calcLogic(SpriteItem* itemSprite, int l);
void fillLogic(SpriteItem* itemSprite, int l);


#define BEGIN_Y_COOR -120;
#define BEGIN_X_COOR  120;
#define POSSIBILITIES_P  400;

#endif
