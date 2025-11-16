
CC := gcc
CFLAGS := -pedantic -Wall -Wwrite-strings -g3 -w -Wextra -Iinclude -MMD -MP -lX11 -lXext -lcrypto -lpng -lz -lm -pthread
#LDFLAGS := -lm
TARGET := myapp

# Поиск всех .c файлов в поддиректориях lib
SRC_DIR := 'lib'
SOURCES := $(shell find $(SRC_DIR) -name '*.c')

# Генерация списка объектных файлов (.o)
OBJ_DIR := build
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))

#Для автоматической пересборки при изменении .h файлов добавьте:
DEPS := $(shell find include -name '*.h')

# Основная цель
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) main.c $^ -o $@ $(CFLAGS)

# Правило компиляции каждого .c в .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Очистка
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean
