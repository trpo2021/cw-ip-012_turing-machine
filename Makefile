APP_NAME = turing
LIB_NAME = lib$(APP_NAME)
TEST_NAME = $(APP_NAME)-test

CC = gcc
CFLAGS = -W -Wall -Wextra -Werror
LFLAGS = -I src -MP -MMD
TESTFLAGS = -g

BIN_DIR = bin
OBJ_DIR = obj
SRC_DIR = src
TEST_DIR = test
THIRDPARTY = thirdparty

APP_PATH = $(BIN_DIR)/$(APP_NAME)
OBJ_PATH = $(OBJ_DIR)/$(SRC_DIR)
LIB_PATH = $(OBJ_DIR)/$(SRC_DIR)/$(LIB_NAME)/$(LIB_NAME).a

OBJ_TEST_PATH = $(OBJ_DIR)/$(TEST_DIR)
TEST_PATH = $(BIN_DIR)/$(TEST_NAME)

APP_SOURCES = $(shell find $(SRC_DIR)/$(APP_NAME) -name '*.c')
APP_OBJECTS = $(APP_SOURCES:$(SRC_DIR)/%.c=$(OBJ_PATH)/%.o)

LIB_SOURCES = $(shell find $(SRC_DIR)/$(LIB_NAME) -name '*.c')
LIB_OBJECTS = $(LIB_SOURCES:$(SRC_DIR)/%.c=$(OBJ_PATH)/%.o)

TEST_SOURCES = $(shell find $(TEST_DIR) -name '*.c')
TEST_OBJECTS = $(TEST_SOURCES:$(TEST_DIR)/%.c=$(OBJ_TEST_PATH)/%.o)

DEPS = $(APP_OBJECTS:.o=.d) $(LIB_OBJECTS:.o=.d) $(TEST_OBJECTS:.o=.d)

-include $(DEPS)

all: $(APP_PATH)

debug: CFLAGS += -DDEBUG -g
debug: all

# make bin/turing
$(APP_PATH): $(APP_OBJECTS) $(LIB_PATH)
	$(CC) $(CFLAGS) $(LFLAGS) $^ -o $@

# make lib
$(LIB_PATH): $(LIB_OBJECTS)
	ar rcs $@ $^

# make obj-files for lib
$(OBJ_DIR)/$(SRC_DIR)/$(LIB_NAME)/%.o: $(SRC_DIR)/$(LIB_NAME)/%.c
	$(CC) $(CFLAGS) $(LFLAGS) -c $< -o $@

# make obj-files for app
$(OBJ_PATH)/$(APP_NAME)/%.o: $(SRC_DIR)/$(APP_NAME)/%.c
	$(CC) $(CFLAGS) $(LFLAGS) -c $< -o $@

test: CFLAGS += -g
test: $(TEST_PATH)

$(TEST_PATH): $(TEST_OBJECTS) $(LIB_PATH)
	$(CC) $(LFLAGS) $(TESTFLAGS) -I $(THIRDPARTY) $^ -o $@

$(OBJ_DIR)/$(TEST_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(LFLAGS) $(TESTFLAGS) -I $(THIRDPARTY) -c $< -o $@

clean:
	$(RM) $(APP_PATH) $(LIB_PATH) $(TEST_PATH)
	find $(OBJ_DIR) -name '*.[od]' -exec $(RM) '{}' \;
	@echo Done!

.clang-format:
	wget -O $@ -c "https://csc-software-development.readthedocs.io/ru/2021/_static/.clang-format"

format: .clang-format
	find . -name "*.[ch]" | xargs clang-format -i
	@echo Done!

.PHONY: all clean format test debug
