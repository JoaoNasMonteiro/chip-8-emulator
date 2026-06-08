CC := clang
BUILD_DIR := build
TARGET_DIR := bin
TARGET_BIN := $(TARGET_DIR)/chip_emulator

CFLAGS := -std=gnu99 -Wall -Wextra
LDFLAGS := -lSDL2

BUILD_TYPE ?= debug

ifeq ($(BUILD_TYPE), debug)
    CFLAGS += -g -DDEBUG_BUILD -fsanitize=address

    LDFLAGS += -fsanitize=address
else
    CFLAGS += -O3 -DNDEBUG
endif

SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRCS))

all: directories $(TARGET_BIN)

directories:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(TARGET_DIR)

$(TARGET_BIN): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.c | directories
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET_DIR)

.PHONY: all clean directories
