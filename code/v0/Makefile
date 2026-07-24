CC := clang
BUILD_DIR := build
TARGET_DIR := bin

CFLAGS := -Wall -Wextra -Werror -std=gnu99
LDFLAGS := -lSDL2

BUILD_TYPE ?= debug

ALL_SRCS := $(wildcard *.c)

ifeq ($(BUILD_TYPE), fuzz)
    CFLAGS += -g -DDEBUG_BUILD -O0 -fsanitize=fuzzer,address,undefined -v
    LDFLAGS += -fsanitize=fuzzer,address,undefined -v
    TARGET_BIN := $(TARGET_DIR)/chip_emulator_fuzzer

    SRCS := $(filter-out main.c render.c, $(ALL_SRCS))

else ifeq ($(BUILD_TYPE), debug)
    CFLAGS += -g -DDEBUG_BUILD -O0 -fsanitize=address
    LDFLAGS += -fsanitize=address
    TARGET_BIN := $(TARGET_DIR)/chip_emulator_debug

    SRCS := $(filter-out fuzz.c, $(ALL_SRCS))

else
    CFLAGS += -O3 -DNDEBUG
    TARGET_BIN := $(TARGET_DIR)/chip_emulator
    SRCS := $(filter-out fuzz.c, $(ALL_SRCS))
endif

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
