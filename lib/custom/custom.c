/* ----------------------------------------------------------------------------------------  */
/*  path: {$ROOT}/lib/custom:                                                                */
/*  Chikilevskiy A.N / aka "JZKatsman" 2025(c) / call89269081096@gmail.com                   */
/*                                                                                           */
/*    This is work make with educational and personal goal, for good wishes for all          */
/*    sentient beings. You may use this script for any purpose but...                        */
/*    BUT:                                                                                   */
/*    * MUST NOT BE USED TO: DUAL-USE ITEMS,                                                 */  
/*    * DO NOT USE OF THIS SCRIPT OF PART WEAPON SYSTEM OF IT IN OTHER WEAPONS PRODUCTION    */
/*    * PROHIBITED FOR USES BY COPYRIGHT ORGANIZATIONS AND/OR FOR ANY COMMERCIAL and AI USES */              
/*    * FOR THIS CODE STRICTLY FORBIDDEN REMOVED CURRENT DISCLAIMER                          */
/*                                                                                           */
/*    Блок управления окном                                                                  */
/* ----------------------------------------------------------------------------------------  */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XShm.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/random.h> // Для getentropy
#include <openssl/rand.h>
#include <openssl/err.h>
#include <pthread.h>
#include "../../include/main.h"
#include "../../include/custom.h"
#include "../../include/bitmap.h"
#include "../../include/getPng.h"
#include "../../include/pngimage.h"
#include "../../include/mathCalc.h"
#include <png.h>
#include <math.h>


int error_handler(Display *display, XErrorEvent *error){
    char buffer[1024];
    XGetErrorText(display, error->error_code, buffer, sizeof(buffer));
    fprintf(stderr, "Xlib Error: %s\n", buffer);
    return 0;
}

void fix_window_size (Context* p) {
    // Устанавливаем заголовки окна
    XStoreName(p->display, p->window, "Circumambulate 1.0");
    // Создаем структуру подсказок размеров
    XSizeHints hints;
    hints.flags = PMinSize | PMaxSize;
    hints.min_width = hints.max_width = WIDTH;   // фиксированная ширина
    hints.min_height = hints.max_height = HEIGHT; // фиксированная высота
    // Применяем подсказки к окну
    XSetWMNormalHints(p->display, p->window, &hints);
    XFlush(p->display);
}

// обеcпечиваем криптографически стойкое случайное число
int clearEntropyRandomNumber(unsigned int* random_ints, size_t array_size){
    if (!random_ints) return 0; // Проверка входного указателя
    if (RAND_poll() != 1) {
            fprintf(stderr, "Ошибка: не удалось собрать достаточную энтропию.\n");
            return 1 ;
        }
        if (RAND_bytes((unsigned char*)random_ints, array_size) != 1) {
             fprintf(stderr, "Ошибка генерации: %s\n", ERR_error_string(ERR_get_error(), NULL));
            return 1;
        }
        return 0;      
}   

int createWindowCustom (Context* p) { 
  p->display = XOpenDisplay(NULL); if (!p->display) {
        fprintf(stderr, "Cannot open display\n");
        return 1;
   };
    p->screen = DefaultScreen(p->display);
    // создание окна 
    p->window = XCreateSimpleWindow(
        p->display, RootWindow(p->display, p->screen),
        0, 0, WIDTH, HEIGHT, 1,
        WhitePixel(p->display, p->screen),
        WhitePixel(p->display, p->screen)
    );
    XMapWindow(p->display, p->window);
    p->gc_red = XCreateGC(p->display, p->window, 0, NULL);
    return 0;
}

int checkShm (Context* p) {
    int major, minor;
    Bool pixmaps;
    if (!XShmQueryVersion(p->display, &major, &minor, &pixmaps)) {
        fprintf(stderr, "Shared memory not supported\n");
        XCloseDisplay(p->display);
        exit(0);
    }
    return 0;
}

//  TODO поправить 
int createShm (Context* p, ShmStructure* s) {
    s->image = XShmCreateImage(
        p->display, DefaultVisual(p->display, p->screen),
        24,
        ZPixmap, 0, &s->shm, WIDTH, HEIGHT
        ); 
    if (!s->image) {
        fprintf(stderr, "XShmCreateImage failed\n");
        XCloseDisplay(p->display);
        exit(0);
        }
    // зафиксирует размер окна
    fix_window_size(p);
    // что такое функция  shmget
    s->shm.shmid = shmget(IPC_PRIVATE, s->image->bytes_per_line * s->image->height * sizeof(unsigned long), IPC_CREAT | 0777);
    s->shm.shmaddr = s->image->data = shmat(s->shm.shmid, 0, 0);
    s->shm.readOnly = False;
    // Прикрепляем память к X-серверу
    if (!XShmAttach(p->display, &s->shm)) {
       fprintf(stderr, "XShmAttach failed\n");
       shmdt(s->shm.shmaddr);
       shmctl(s->shm.shmid, IPC_RMID, 0);
       XDestroyImage(s->image);
       XCloseDisplay(p->display);
       return 1;
    }    
    return 0;
}

// отдельный блок рендера 
void renderBackground (ShmStructure* s, png_bytep *row_pointers) {
    // 1 прорисовка фона renderBackground
    for (int i = 0; i < s->image->height; i++ ) {
        for (int j = 0; j < s->image->width;  j++) {
             png_bytep row = row_pointers[j];
             png_bytep px = &(row[i * 4]); // 4 байта на пиксель (RGBA)
             unsigned char *pixelbg = (unsigned char*)&s->image->data[j * s->image->bytes_per_line + i * 4];
             pixelbg[0] = px[2];   // Blue
             pixelbg[1] = px[1];   // Green
             pixelbg[2] = px[0];   // Red
             pixelbg[3] = px[3];   // Alpha            
         }
    }
}

void renderLetter (ShmStructure* s, SpriteItem* itemSprite, int x1, int y1, int l){
     // 3 прорисовка букв (внутри прорисовки спрайтов) renderLetter 
     char strNum[20];
     sprintf(strNum, "%d", itemSprite[l].cost); // Преобразование числа в строку
     int len = strlen(strNum);
     int inneroffset = 15;
     inneroffset = ((PWIDTH)/2.0) - (15 * len)/2 ;
     // цикл по всем спрайтам
     for (int m = 0; m < strlen(strNum); m++) {
     int num = (char)strNum[m]  - '0';

     // ВЫДЕЛИТЬ  прорисуем битовую карту
     for (int a = 0; a < 16; a++) {
        for (int b = 0; b < 16; b++) {
            unsigned char *pixel2 = (unsigned char*)&s->image->data[(y1 + a + 30) * s->image->bytes_per_line + (inneroffset + (x1 + b)) * 4];
            // Проверяем бит (старший бит слева)
            unsigned char* bitmap = bitmap_8[num];
            if (bitmap[a/2] & (1 << (7 - b/2))) {
             // Размер строки с выравниванием (кратен 4 байтам)
             pixel2[0] = 210;   // Blue
             pixel2[1] = 210;   // Green
             pixel2[2] = 210;   // Red
             pixel2[3] = 0;     // Alpha
            }  
        }
      }
     //  прорисовка битовой карты
      inneroffset = inneroffset + 15;
     }
}

void  renderSprite (ShmStructure* s, png_bytep *row_pointers_ball, SpriteItem* itemSprite, int l ) {
    int x1, y1;
    // 2  прорисовка спрайтов  renderSprite
    x1 = itemSprite[l].xcoor;
    y1 = itemSprite[l].ycoor;
    // рендерим спрайт 
    for (int y = 0; y < PHEIGHT; y++) {
        for (int x = 0; x < PWIDTH ; x++ ) {
            // здесь рендерим наш отдельный спрайт номером itemSprite[l].type
            // берем по строчкам!
            png_bytep row   = row_pointers_ball[y];
            png_bytep px =  &(row[x * 4]); // 4 байта на пиксель (RGBA)

            //  повернули пиксель
            SpriteRotate* srResult;
            srResult = (struct SpriteRotate*) malloc(sizeof(struct SpriteRotate));
            srResult->x = 0; srResult->y = 0;

            SpriteRotateCoord*  srCoor;
            srCoor = (struct SpriteRotateCoord*) malloc(sizeof(struct SpriteRotateCoord));
            srCoor->l = l;
            srCoor->x = x;
            srCoor->y = y;
            srCoor->xstart = x1;
            srCoor->ystart = y1;            
            
            //  поворачиваем
            calcRotate(srResult, srCoor, itemSprite);
            //2. рисование  (px[3] != 0);
            if (px[3] != 0) {
              if ((srResult->x >= 0)  &&  (srResult->y >= 0)) {
               unsigned char *pixel1 = (unsigned char*)&s->image->data[(((int)floor(srResult->y))
                                       * s->image->bytes_per_line) + (int)floor(srResult->x) * 4];
               // интерполируем
               long double a2 = (double long)px[3] / 255.0;
               long double a1 = (double long)pixel1[3] / 255.0;
               long double a_result = a1 + a2 * (1 - a1);

               // RNew = R1 + t * (R2 - R1), t = 0..1
               pixel1[0] = (int)round((1 - a_result) * pixel1[0] + a_result * px[2]); 
               pixel1[1] = (int)round((1 - a_result) * pixel1[1] + a_result * px[1]);
               pixel1[2] = (int)round((1 - a_result) * pixel1[2] + a_result * px[0]);
               pixel1[3] = (int)round(a_result * 255);   // Alpha

               // это убогий способ интерполяции     
               unsigned char *pixel2 = (unsigned char*)&s->image->data[(((int)floor(srResult->y + 1))
                                       * s->image->bytes_per_line) + (int)floor(srResult->x) * 4];
               // интерполируем
               long double a22 = (double long)px[3] / 255.0;
               long double a12 = (double long)pixel2[3] / 255.0;
               long double a_result2 = a12 + a22 * (1 - a12);

               // RNew = R1 + t * (R2 - R1), t = 0..1
               pixel2[0] = (int)round((1 - a_result2) * pixel2[0] + a_result2 * px[2]); 
               pixel2[1] = (int)round((1 - a_result2) * pixel2[1] + a_result2 * px[1]);
               pixel2[2] = (int)round((1 - a_result2) * pixel2[2] + a_result2 * px[0]);
               pixel2[3] = (int)round(a_result2 * 255);   // Alpha
               

            }  
           }   

               free(srResult);
               free(srCoor);    
             
     }
    }
    // перерисовываем подписи цифрами
    if ((y1 - 50) > 0) { renderLetter(s, itemSprite, x1, (y1 - 50), l);}  
    
}
    
int generateField (Context* p,ShmStructure* s, SpriteItem* itemSprite,
                       png_bytep *row_pointers,
                       SpriteImage **row_pointers_ball
                      ) {
    renderBackground(s, row_pointers);
    // перебираем все спрайты в отдельных потоках
    for (int l = 0; l < LENGHT; l++) {
      if (itemSprite[l].active == 1) {
           // * рендерить в потоке
           int x = itemSprite[l].type;
           renderSprite (s, row_pointers_ball[x]->row_pointers_balls, itemSprite, l);
           // *
      }
    }  
    return 0;
}

// конец блока рендеринга
int block1Loop (Context* p, ShmStructure* s) {
            XSetErrorHandler(error_handler);

            // Копируем изображение в окно
            /*XShmPutImage(
                p->display,
                p->window,
                DefaultGC(p->display, p->screen),
                s->image,
                0, 0,   // src x,y
                0, 0,   // dst x,y
                WIDTH, HEIGHT, // ширина, высота
                True
            );*/
            
            XPutImage(
                 p->display, p->window, DefaultGC(p->display, p->screen),
                 s->image, 0, 0, 0, 0, WIDTH, HEIGHT
                 );    
}

// действие 2
int block2Loop (Context* p, ShmStructure* s) {
       int num_lines = sizeof(msgB) / sizeof(msgB[0]);
       XFlush(p->display);
       return 0;
}

int switchLoop (Context* p, ShmStructure* s, SpriteItem* itemSprite) {
  XEvent report;
  XSelectInput(p->display, p->window, ExposureMask | KeyPressMask);
  XNextEvent (p->display, &report);
  switch ( report.type ) {
    case Expose :
     if ( report.xexpose.count != 0 )
      break;
             //....    
      break;
      case KeyPress :
      //         if (report.xkey.keycode == 0x71) {
      //         if (report.xkey.keycode == 0x72) {
      //         if (report.xkey.keycode == 0x6f) {
      //         if (report.xkey.keycode == 0x74) {
      //         if (report.xkey.keycode == 0x09) {         
      if (report.xkey.keycode == 0x09) {
      XFreeGC ( p->display, p->gc_red );    
      XCloseDisplay ( p->display );
      exit (0);
     }
  }
  return 0;
}    

void initRenderWindow(Context* p,
                      ShmStructure* s,
                      SpriteItem* itemSprite,
                      png_bytep *row_pointers,
                      SpriteImage **row_pointers_ball){
  // * инициализируем массив спрайтов
  for (int i = 0; i < LENGHT; i++) {
      initLogic(itemSprite, i);
    }
  // * вызываем генерацию изображения!
  generateField(p, s, itemSprite, row_pointers, row_pointers_ball);
  // * ТОЛЬКО прорисовка 
  block1Loop (p, s);
}

void renderWindow(Context* p,
                  ShmStructure* s,
                  SpriteItem* itemSprite,
                  png_bytep *row_pointers,
                  SpriteImage **row_pointers_ball) {
  for (int i = 0; i < LENGHT; i++) {
  //  пересчитали спрайты
  //  пересчитываем в потоке 
      calcLogic(itemSprite, i);
      fillLogic(itemSprite, i);
  }  
  // * вызываем генерацию изображения!
  //   здесь рендерится наш отдельный спрайт  
      generateField(p, s, itemSprite, row_pointers, row_pointers_ball );
      block1Loop (p, s);
}


int mainLoop (Context* p, ShmStructure* s) {
    // задержка
    const double fps = 440.0;
    const useconds_t delay = (useconds_t)(1000000 / fps);
 
    SpriteItem* itemSprite = (SpriteItem*)malloc(LENGHT * sizeof(SpriteItem));
    const char *filenamebackground = "/home/useralex/twopoint/image/A1200-1200.png";
    //PngImage imagebackground = read_png_file(filenamebackground);
    // Выделение памяти для данных
    png_bytep *row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * WIDTH  * HEIGHT);
    getPng(filenamebackground, row_pointers);

    //  uploadSprite();
    // 3. блок подгрузки спрайтов
    SpriteImage* dataImages[STYPE];
    uploadSprite(dataImages);
   
    // Преобразование в 32-бит ARGB (для X11)
    s->image->data = malloc(WIDTH * HEIGHT * sizeof(uint32_t)  * 4);

    // TODO передаем весь массив спрайтов
    initRenderWindow(p,s, itemSprite, row_pointers, dataImages);
 
    // ****
    // основной цикл
    // ****
    while (1) {
        // Проверка событий (неблокирующая)
      while (XPending(p->display)) {
         switchLoop (p,s,itemSprite);
      }
        // TODO вызов рендера раз интервал времени
      renderWindow(p,s,itemSprite, row_pointers, dataImages);
        // XFlush(p->display);       // Отправить команды на сервер
        XSync(p->display, False); // Дождаться обновления экрана
        usleep(delay);
    }
    return 0; 
}

  

