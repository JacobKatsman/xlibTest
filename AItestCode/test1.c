#include <stdio.h>
#include <stdlib.h>
#include <png.h>

int process_png(const char *input_filename, const char *output_filename) {
    FILE *infile = fopen(input_filename, "rb");
    if (!infile) {
        perror("Ошибка открытия исходного файла");
        return -1;
    }

    // 1. Проверка сигнатуры PNG
    unsigned char header[8];
    if (fread(header, 1, 8, infile) != 8 || png_sig_cmp(header, 0, 8)) {
        fprintf(stderr, "Файл не является корректным PNG\n");
        fclose(infile);
        return -1;
    }

    // 2. Инициализация структур для чтения
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info_ptr = png_create_info_struct(png_ptr);
    
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(infile);
        return -1;
    }

    png_init_io(png_ptr, infile);
    png_set_sig_bytes(png_ptr, 8);

    // Читаем только заголовки (информацию о файле), а не пиксели
    png_read_info(png_ptr, info_ptr);

    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    // Нормализация формата для удобства обработки (приведение к 8 бит на канал)
    if (bit_depth == 16) png_set_strip_16(png_ptr);
    if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png_ptr);
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_expand_gray_1_2_4_to_8(png_ptr);
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png_ptr);
    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) png_set_gray_to_rgb(png_ptr);

    // Обновляем информацию после трансформаций (теперь у нас гарантированно RGB или RGBA, 8 бит на канал)
    png_read_update_info(png_ptr, info_ptr);
    color_type = png_get_color_type(png_ptr, info_ptr);
    size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    // 3. Выделение памяти под единый одномерный массив байт
    size_t image_data_size = rowbytes * height;
    png_bytep raw_image_data = (png_bytep)malloc(image_data_size);
    
    // Создаем вспомогательный массив указателей на строки для libpng
    png_bytepp row_pointers = (png_bytepp)malloc(sizeof(png_bytep) * height);
    for (png_uint_32 i = 0; i < height; i++) {
        row_pointers[i] = raw_image_data + i * rowbytes;
    }

    // Считываем пиксели напрямую в наш выделенный массив
    png_read_image(png_ptr, row_pointers);
    
    // Чтение завершено, файл можно закрыть
    fclose(infile);

    // =========================================================================
    // 4. МЕСТО ДЛЯ ВАШЕЙ ОБРАБОТКИ МАССИВА `raw_image_data`
    // =========================================================================
    int channels = (color_type == PNG_COLOR_TYPE_RGBA) ? 4 : 3;
    
    printf("Размер: %ux%u, Каналов: %d, Байт в строке: %zu\n", width, height, channels, rowbytes);

    // Пример: Инверсия цветов (не трогая Альфа-канал, если он есть)
    for (png_uint_32 y = 0; y < height; y++) {
        png_bytep row = raw_image_data + (y * rowbytes);
        for (png_uint_32 x = 0; x < width; x++) {
            png_bytep pixel = &(row[x * channels]);
            
            pixel[0] = 255 - pixel[0]; // R
            pixel[1] = 255 - pixel[1]; // G
            pixel[2] = 255 - pixel[2]; // B
            // pixel[3] — это Альфа-канал (прозрачность), его не трогаем
        }
    }
    // =========================================================================

    // 5. Запись обработанного массива в новый файл
    FILE *outfile = fopen(output_filename, "wb");
    if (!outfile) {
        perror("Ошибка создания нового файла");
        free(row_pointers);
        free(raw_image_data);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return -1;
    }

    png_structp write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop write_info_ptr = png_create_info_struct(write_ptr);

    if (setjmp(png_jmpbuf(write_ptr))) {
        fclose(outfile);
        free(row_pointers);
        free(raw_image_data);
        png_destroy_write_struct(&write_ptr, &write_info_ptr);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return -1;
    }

    png_init_io(write_ptr, outfile);

    // Устанавливаем параметры для нового файла
    png_set_IHDR(write_ptr, write_info_ptr, width, height, 8, color_type,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    
    png_write_info(write_ptr, write_info_ptr);

    // Передаем массив указателей на строки нашего измененного массива и записываем
    png_write_image(write_ptr, row_pointers);
    png_write_end(write_ptr, NULL);

    // 6. Очистка всей памяти
    fclose(outfile);
    free(row_pointers);
    free(raw_image_data);
    png_destroy_write_struct(&write_ptr, &write_info_ptr);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}

int main() {
    return process_png("input.png", "output.png");
}
