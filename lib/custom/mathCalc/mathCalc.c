/* ----------------------------------------------------------------------------------------  */
/*  path: {$ROOT}/lib/custom/MathCalc/mathCalc.c:                                            */
/*  Chikilevskiy A.N / aka "JZKatsman" 2025(c) / call89269081096@gmail.com                   */
/*                                                                                           */
/*    This is work make with educational and personal goal, for good wishes for all          */
/*    sentient beings. You may use this script for any purpose but...                        */
/*    BUT:                                                                                   */
/*    * MUST NOT BE USED TO: DUAL-USE ITEMS,                                                 */  
/*    * DO NOT USE OF THIS SCRIPT OF PART WEAPON SYSTEM OF IT IN OTHER WEAPONS PRODUCTION    */
/*    * PROHIBITED FOR USES BY COPYRIGHT ORGANIZATIONS AND/OR FOR COMMERCIAL AND AI USES     */              
/*    * IT'S CODE STRICTLY FORBIDDEN FOR REMOVE THIS DISCLAIMER                              */
/*                                                                                           */
/*    Блок вычислений                                                                        */
/* ----------------------------------------------------------------------------------------  */
#include <png.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "../../../include/getPng.h"
#include "../../../include/main.h"
#include "../../../include/mathCalc.h"
#include "../../../include/logic.h"

void calcRotate(SpriteRotate* sr, SpriteRotateCoord* srCoor, SpriteItem* itemSprite){
             
             // переведем градусы в радианы.
             long double degrees = (long double)itemSprite[srCoor->l].degrees * M_PI / 180;
             long double angle_radians  = (degrees);
                
             long double centery = (srCoor->ystart + (long double)round((PHEIGHT)/2.0));
             long double centerx = (srCoor->xstart + (long double)round((PWIDTH)/2.0));

             long double currenty = (srCoor->ystart + srCoor->y);
             long double currentx = (srCoor->xstart + srCoor->x);

             long double xangel = ((currentx - centerx) * cos(angle_radians) - (currenty - centery) * sin(angle_radians)) + centerx;
             long double yangel = ((currentx - centerx) * sin(angle_radians) + (currenty - centery) * cos(angle_radians)) + centery;

             //  возвращаемый результат
             sr->x = xangel;
             sr->y = yangel ;
}    


// 1) наложение пикселя на прозрачность
// Билинейная интерполяция рассматривает квадрат 2x2 известных пикселя, окружающих неизвестный.
// В качестве интерполированного значения используется *взвешенное усреднение этих четырёх пикселей*.
// В результате изображения выглядят значительно более гладко, чем результат работы метода ближайшего соседа.
// Билинейную интерполяцию (оптимально для скорости/качества):
/* func bilinear_interpolation(image, x, y):
    x1, y1 = floor(x), floor(y)
    x2, y2 = x1 + 1, y1 + 1
    # Доли расстояния
    dx, dy = x - x1, y - y1
    # Веса пикселей
    p11 = image[y1, x1] * (1 - dx) * (1 - dy)
    p21 = image[y1, x2] * dx * (1 - dy)
    p12 = image[y2, x1] * (1 - dx) * dy
    p22 = image[y2, x2] * dx * dy
return p11 + p21 + p12 + p22
*/

//  2) бикубическая интерполяция

/*
    наложение прозрачности 

 func blend_pixels(pixel1, pixel2):
    """Смешивает два пикселя в формате (R, G, B, A)."""
    R1, G1, B1, A1 = pixel1
    R2, G2, B2, A2 = pixel2
    
    # Нормализация альфа-каналов в [0.0, 1.0]
    a1 = A1 / 255.0
    a2 = A2 / 255.0
    
    # Результирующая прозрачность
    a_result = a1 + a2 * (1 - a1)
    
    # Если результирующая прозрачность 0, возвращаем прозрачный пиксель
    if a_result == 0:
        return (0, 0, 0, 0)
    
    # Смешивание цветовых каналов
    def blend_channel(c1, c2):
        return (c1 * a1 + c2 * a2 * (1 - a1)) / a_result
    
    R = blend_channel(R1, R2)
    G = blend_channel(G1, G2)
    B = blend_channel(B1, B2)
    
    # Преобразование в целые числа [0, 255]
    R = round(R)
    G = round(G)
    B = round(B)
    A_result_int = round(a_result * 255)
    
    return (R, G, B, A_result_int)
*/

 
int blend_channel (int B, int F,
                   double long resultOpaticy,
                   double long alfaBackground,
                   double long alfaSprite){
    return (int)round(((B * alfaBackground + F * alfaSprite * (1 - alfaBackground)) / resultOpaticy));
}    

void overlappingTransparency(colorRGB *c,png_bytep px, int pixel1[]){
    // Нормализация альфа-каналов в [0.0, 1.0]
    double long alfaBackground =  ((double long)pixel1[3]) / 255.0;  
    double long alfaSprite     =  ((double long)px[3]) / 255.0;
    double long resultOpaticy = alfaBackground + alfaSprite * (1 - alfaBackground);
    c->R = (blend_channel(pixel1[2], px[0], resultOpaticy, alfaBackground, alfaSprite)); // R
    c->G = (blend_channel(pixel1[1], px[1], resultOpaticy, alfaBackground, alfaSprite)); // G
    c->B = (blend_channel(pixel1[0], px[2], resultOpaticy, alfaBackground, alfaSprite)); // B
    c->A = 255;
}




            
