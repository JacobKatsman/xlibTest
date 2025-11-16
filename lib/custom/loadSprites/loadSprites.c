/* ----------------------------------------------------------------------------------------  */
/*  path: {$ROOT}/lib/custom/loadSpeites/loadSprites.c:                                      */
/*  Chikilevskiy A.N / aka "JZKatsman" 2025(c) / call89269081096@gmail.com                   */
/*                                                                                           */
/*    This is work make with educational and personal goal, for good wishes for all          */
/*    sentient beings. You may use this script for any purpose but...                        */
/*    BUT:                                                                                   */
/*    * MUST NOT BE USED TO: DUAL-USE ITEMS,                                                 */  
/*    * DO NOT USE OF THIS SCRIPT OF PART WEAPON SYSTEM OF IT IN OTHER WEAPONS PRODUCTION    */
/*    * PROHIBITED FOR USES BY COPYRIGHT ORGANIZATIONS AND/OR FOR COMMERCIAL USES 'AI'       */              
/*    * IT'S CODE STRICTLY FORBIDDEN FOR REMOVE THIS DISCLAIMER                              */
/*                                                                                           */
/*    подгрузка  спрайтов                                                                    */
/* ----------------------------------------------------------------------------------------  */
#include <png.h>
#include <string.h>
#include <stdlib.h>
#include "../../../include/getPng.h"
#include "../../../include/main.h"
#include "../../../include/loadSprites.h"

//  Блок выделения памяти на структуру спрайтов
//  1 выделил память на структуру
SpriteImage* alloctionDataStruct() {
     int nineSprite = STYPE;
     SpriteImage *sImage = (SpriteImage*)malloc(nineSprite * sizeof(SpriteImage));
     if ( sImage == NULL) {
         fprintf(stderr, "Non allocated memeory SpriteImage for  failed\n");
         exit(0);
     }
     sImage->filenameball = NULL;
     sImage->row_pointers_balls = NULL;
     return sImage;
}

// 2 выделили память под строку
int initArrChar(SpriteImage* sImage, const char* source) {
    sImage->filenameball = (char*)malloc(255 * sizeof(char));
    if (sImage->filenameball == NULL) {
         fprintf(stderr, "Non allocated memeory filenameball for  failed\n");        
        exit(0); 
    }
    strcpy(sImage->filenameball, source);
    return 0; 
}
// 3  выделили память под спрайты 
int initPngBytes(SpriteImage* sImage) {
    sImage->row_pointers_balls = (png_bytep*)malloc(sizeof(png_bytep) * WIDTH  * HEIGHT);;
    if (sImage->row_pointers_balls == NULL) {
         fprintf(stderr, "Non allocated memeory row_pointers_balls for  failed\n");                
        exit(0);
    }
    return 0; 
}

void uploadSprite(SpriteImage **dataImages) {
     for (int x=0; x<STYPE; x++) {
     SpriteImage* dataImage = alloctionDataStruct();
     if (dataImage == NULL) {
         fprintf(stderr, "1 Non-allocated memeory SpriteImage\n");
         exit(0);}
     if (initArrChar(dataImage, "0") != 0) {
         fprintf(stderr, "2 Non-allocated memeory filename\n");
         exit(0);}
     if (initPngBytes(dataImage) != 0) {
         fprintf(stderr, "3 Non-allocated memeory png filename\n");
         exit(0);}
      dataImages[x] = dataImage;
     }

     dataImages[0]->filenameball = "./image/1.png";
     getPng(dataImages[0]->filenameball, dataImages[0]->row_pointers_balls);
     dataImages[1]->filenameball = "./image/2.png";
     getPng(dataImages[1]->filenameball, dataImages[1]->row_pointers_balls);
     dataImages[2]->filenameball = "./image/3.png";
     getPng(dataImages[2]->filenameball, dataImages[2]->row_pointers_balls);
     
     dataImages[3]->filenameball = "./image/4.png";
     getPng(dataImages[3]->filenameball, dataImages[3]->row_pointers_balls);
     dataImages[4]->filenameball = "./image/5.png";
     getPng(dataImages[4]->filenameball, dataImages[4]->row_pointers_balls);
     dataImages[5]->filenameball = "./image/6.png";
     getPng(dataImages[5]->filenameball, dataImages[5]->row_pointers_balls);     
     dataImages[6]->filenameball = "./image/7.png";
     getPng(dataImages[6]->filenameball, dataImages[6]->row_pointers_balls);
     dataImages[7]->filenameball = "./image/8.png";
     getPng(dataImages[7]->filenameball, dataImages[7]->row_pointers_balls);
     dataImages[8]->filenameball = "./image/9.png";
     getPng(dataImages[8]->filenameball, dataImages[8]->row_pointers_balls);

     // выведем
     //for (int x=0; x<STYPE; x++) {
     //  printf("String: %s\n", dataImages[x]->filenameball);
     //}
    
}
