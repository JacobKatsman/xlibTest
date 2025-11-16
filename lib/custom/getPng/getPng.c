/* ----------------------------------------------------------------------------------------  */
/*  path: path: {$ROOT}/lib/custom/getPng/getPng.h:                                          */
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
/*    Чтение графического png  файла                                                         */
/* ----------------------------------------------------------------------------------------  */
 
#include <png.h>
#include  <stdlib.h>
#include "../../../include/getPng.h"

// разобратся в дублировании файлов 
png_bytep* getPng(const char *filenamebackground1, png_bytep *row_pointers){
    // 1. Открытие файла  фона  
    FILE *fp = fopen(filenamebackground1, "rb");
    if (!fp) {
        perror("File open error");
    }
    // 2. Проверка сигнатуры PNG
    png_byte header[8];
    if (fread(header, 1, 8, fp) != 8 || png_sig_cmp(header, 0, 8)) {
        fclose(fp);
        fprintf(stderr, "Not a PNG file\n");
    }
    // 3. Инициализация структур
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        fclose(fp);
    }
    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_read_struct(&png, NULL, NULL);
        fclose(fp);
    }
    // Настройка обработки ошибок
    if (setjmp(png_jmpbuf(png))) {
        png_destroy_read_struct(&png, &info, NULL);
        fclose(fp);
    }
    png_init_io(png, fp);
    png_set_sig_bytes(png, 8); // Пропустить уже прочитанную сигнатуру
    png_init_io(png, fp);
    // Настройка преобразований (конвертируем всё в RGBA)
    png_read_info(png, info);
    // Принудительно конвертируем в 64-bit RGBA
    png_set_expand(png);  // Расширяем палитровые и серые изображения
    png_set_gray_to_rgb(png);  // Конвертируем grayscale в RGB
    png_set_add_alpha(png, 0xFFFF, PNG_FILLER_AFTER);  // Добавляем альфа-канал (16-bit)
    
    int depth = png_get_bit_depth(png, info);
    int color_type = png_get_color_type(png, info);
    if (png_get_color_type(png, info) == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png);
    if (png_get_color_type(png, info) == PNG_COLOR_TYPE_GRAY && depth < 8)
    png_set_expand_gray_1_2_4_to_8(png);
    if (png_get_valid(png, info, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png);
    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb(png);
    if (depth == 16)
    png_set_strip_16(png);
    png_set_add_alpha(png, 0xFF, PNG_FILLER_AFTER); // Для RGBA
    if (png_get_bit_depth(png, info) < 16) {
      png_set_expand_16(png);  // Расширяем до 16 бит
    }
    int channels = png_get_channels(png, info); // Теперь всегда 4 (RGBA)
    int width = png_get_image_width(png, info);
    int height = png_get_image_height(png, info);
 
    for (int y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png, info));
    }
    png_read_image(png, row_pointers);
    fclose(fp); // закрываем Файл
    return row_pointers;
}


