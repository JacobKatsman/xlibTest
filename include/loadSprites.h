// структура картинки
typedef struct SpriteImage {
    char *filenameball;  //указатель на имя файла
    png_bytep *row_pointers_balls; // указатель на прочитанную структуру графического.файла
} SpriteImage;

SpriteImage* alloctionDataStruct();
int initArrChar(SpriteImage* sImage, const char* source);
int initPngBytes(SpriteImage* sImage);
void uploadSprite(SpriteImage **dataImages);
