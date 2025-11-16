/* ---------------------------------------------------------------------------- */
/*  path: {$ROOT}/lib/logic:                                                    */
/*  Chikilevskiy A.N / aka "JZKatsman" 2025(c) / call89269081096@gmail.com      */
/*                                                                              */
/*  Особенности поведения спрайтов                                              */
/* ---------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include "../../include/logic.h"
#include "../../include/main.h"

//  инициализировали весь массив
void initLogic(SpriteItem* itemSprite, int i) {
    int xActiveRand;
    bool xBoolActive;
    int xClockwiseRand;      
    int xCoorRand;
    int xCostRand;
    long double xAngel;
    int xSpeed;

         // инициализурем координаты по x
         xCoorRand = rand() % (MAX);
         itemSprite[i].xcoor = xCoorRand;



         // инициализиуем активность 
         xBoolActive = 0;
         xActiveRand = 0 + rand() % POSSIBILITIES_P;
         if  (xActiveRand == 3) {
             // здесь
             xBoolActive = 1;
             /* int flag = 0; 
             for (int j=0; j < LENGHT; j++){
              double long  metrica = sqrt(pow(abs(itemSprite[j].xcoor - itemSprite[i].xcoor),2) - pow(abs(itemSprite[j].ycoor - itemSprite[i].ycoor),2));
              if  ((metrica < 50) && (i != j)){
                  xBoolActive = 0;
              }
              }*/    
         }


         xSpeed = 3 + rand() % 5;

         // инициализируем генерируемый тип
         itemSprite[i].type = 0 + rand() % STYPE;         
         // инициализируем координаты по y
         itemSprite[i].ycoor = BEGIN_Y_COOR + xSpeed * itemSprite[i].type;
         
         // инициализиуем наврпавленеи поворота         
         itemSprite[i].degrees = 0;
         xClockwiseRand = 0 + rand() % 2; // 2 направления вращения
         if (xClockwiseRand == 1){
               itemSprite[i].degrees =  13;
         } else {
               itemSprite[i].degrees = -13;
         }

         // инициализируем активность  
         itemSprite[i].active = xBoolActive;
         // инициализируем поворот 
         itemSprite[i].clockwise = xClockwiseRand;
         // инициализируем стоимость
         xCostRand = (1 + rand() % 999);
         itemSprite[i].cost = xCostRand;
         // инициализируем скорость врещения
         xSpeed = 1 + rand() % 6;
         itemSprite[i].speed = xSpeed;

}

// пересчитали старые значения и убрали ненужные 
void calcLogic(SpriteItem* itemSprite, int i) {
  int yCoor;
  int xActiveRand;
  int xCoorRand;
  int xCostRand;
  bool xBoolActive;
  int xClockwiseRand;  
  long double xAngel;
  int xLuftLeft;
  int xLuftRight;
  int xSpeed;

      // вычисляем если состояние активно  
      if (itemSprite[i].active == 1) {

        // движение спрайта вниз
        yCoor = itemSprite[i].ycoor;
        itemSprite[i].ycoor = yCoor + itemSprite[i].speed * itemSprite[i].speed;

        // переводим в неактивное состояние еслт дошел до низа 
        if (yCoor > 1200) {
            itemSprite[i].ycoor = 0;    
            itemSprite[i].active = 0;
        }

        // прибавляем углы повотора 
        if (itemSprite[i].clockwise == 1){
            itemSprite[i].degrees +=  13; //itemSprite[i].speed ;
         } else {
            itemSprite[i].degrees -=  13; //itemSprite[i].speed ;
         }

}
}

// заполнили массив новыми значениями
void fillLogic(SpriteItem* itemSprite, int i) {

         int yCoor;
         int xCoorRand;
         int xActiveRand;
         bool xBoolActive;
         long double xAngel;
         int xClockwiseRand;
         int xCostRand;
         int xSpeed;
         // вычисляем если состояние пассивно              
        if (itemSprite[i].active == 0) {

         // переопределяем соостояние координат    
         xCoorRand = rand() % (MAX);
         itemSprite[i].xcoor = xCoorRand;

          
         // скорость вращение
         xSpeed = 3 + rand() % 5;
         itemSprite[i].speed = xSpeed;

         

         
         // стоимость 
         xCostRand = (1 + rand() % 999);
         itemSprite[i].cost = xCostRand;

         // создаем новый спрайт 
         xActiveRand = 0 + rand() % POSSIBILITIES_P;
         if  (xActiveRand == 3) {
             xBoolActive = 1;
             // здесь
             /*int flag = 0; 
               for (int j=0; j<LENGHT; j++){
                   double long  metrica = sqrt(pow(abs(itemSprite[j].xcoor - itemSprite[i].xcoor),2) - pow(abs(itemSprite[j].ycoor - itemSprite[i].ycoor),2));
                   printf ("- %Lf -", metrica);
                   if  ((metrica < 50) && (i != j)){
                     xBoolActive = 0;
                   }
                   }*/
             // переопределяем тип
             itemSprite[i].type = 0 + rand() % STYPE;
             itemSprite[i].ycoor = BEGIN_Y_COOR + xSpeed * itemSprite[i].type;
             itemSprite[i].active = xBoolActive;
         } else {
             itemSprite[i].active = 0;
         }

         // переопределяем вращение с новым направлением
         itemSprite[i].degrees = 0;
         xClockwiseRand = 0 + rand() % 2; // 2 направления вращения 
         if (xClockwiseRand == 1) {
             itemSprite[i].degrees +=  13; //itemSprite[i].speed;
         }  else {
             itemSprite[i].degrees -=  13; //itemSprite[i].speed;
         }
         
    }
}
