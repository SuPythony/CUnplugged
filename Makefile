CC = gcc
CFLAGS = -I./src -Wall -Wextra -lm
SRC_DIR = src
BUILD_DIR = build

SRC = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/*/*.c) $(wildcard $(SRC_DIR)/*/*/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))

ifeq ($(OS),Windows_NT)
    TARGET = CUnplugged.exe
    MKDIR = powershell -Command "New-Item -ItemType Directory -Force -Path $(dir $@)" > NUL
    RM = rmdir /S /Q
else
    TARGET = CUnplugged
    MKDIR = mkdir -p $(dir $@)
    RM = rm -rf
endif

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(CFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@$(MKDIR)
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	-$(RM) $(BUILD_DIR)
	-$(RM) $(TARGET)
