#include <png.h>
typedef struct PngImage{
    int width;
    int height;
    png_byte color_type;
    png_byte bit_depth;
    png_bytep *row_pointers;
    png_bytep *row_pointers_ball;
} PngImage;

PngImage read_png_file(const char *filename); 
void free_png_image(PngImage *image);
    


