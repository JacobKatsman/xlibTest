/* ----------------------------------------------------------------------------------------  */
/*  path: path: {$ROOT}/lib/pngImage/png.c:                                                  */
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
/*    просто прочитаем png file                                                              */
/* ----------------------------------------------------------------------------------------  */
 
#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include <string.h>
#include "../../include/getPng.h"
#include "../../include/pngimage.h"

// разобратся в дублировании файлов
PngImage read_png_file(const char *filename) {
    // 1. Открытие файла  фона 
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("File opening failed");
        exit(EXIT_FAILURE);
    }

    // Проверка сигнатуры PNG
    png_byte header[8];
    if (fread(header, 1, 8, fp) != 8 || png_sig_cmp(header, 0, 8)) {
        fclose(fp);
        fprintf(stderr, "Not a PNG file\n");
    }    
    
    fread(header, 1, 8, fp);
    
    if (png_sig_cmp(header, 0, 8)) {
        fclose(fp);
        fprintf(stderr, "Not a PNG file");
        exit(EXIT_FAILURE);
    }

    // Инициализация структур
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    // аллоцировать структуру png 
    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_read_struct(&png, NULL, NULL);
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    // Настройка обработки ошибок
    if (setjmp(png_jmpbuf(png))) {
        png_destroy_read_struct(&png, &info, NULL);
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    png_init_io(png, fp);
    png_set_sig_bytes(png, 8);
    png_read_info(png, info);

    PngImage image;
    image.width = png_get_image_width(png, info);
    image.height = png_get_image_height(png, info);
    image.color_type = png_get_color_type(png, info);
    image.bit_depth = png_get_bit_depth(png, info);

// Преобразование в в 8-битный RGBA, если нужно
    if (image.bit_depth == 16) png_set_strip_16(png);
    if (image.color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png);
    if (image.color_type == PNG_COLOR_TYPE_GRAY && image.bit_depth < 8) png_set_expand_gray_1_2_4_to_8(png);
    if (png_get_valid(png, info, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png);
    if (image.color_type == PNG_COLOR_TYPE_RGB || image.color_type == PNG_COLOR_TYPE_GRAY) png_set_add_alpha(png, 0xFF, PNG_FILLER_AFTER);
    
    // Преобразование в 8-битный RGBA
    png_read_update_info(png, info);

    // Выделение памяти под строки
    image.row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * image.height);
    for (int y = 0; y < image.height; y++) {
        image.row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png, info));
    }
    // Чтение изображения
    png_read_image(png, image.row_pointers);

    fclose(fp);
    png_destroy_read_struct(&png, &info, NULL);
    
    return image;
}

void free_png_image(PngImage *image) {
    for (int y = 0; y < image->height; y++) {
        free(image->row_pointers[y]);
    }
    free(image->row_pointers);
}

