CC ?= clang
OPTS = -std=c99 -Wall -Wextra -Wpedantic -fsanitize=address -g

LEXER_DIR = lexer
LEXER_BUILD = lexer/build
LEXER_APP_TARG = $(LEXER_BUILD)/lexer_app
LEXER_TEST_TARG = $(LEXER_BUILD)/main_test

HELPER_DIR = helper
HELPER_BUILD = helper/build

HELPER_OBJS = $(HELPER_BUILD)/string_functions.o $(HELPER_BUILD)/DynamicString.o

COMMON_OBJS = $(LEXER_BUILD)/Lexer_functions.o $(HELPER_OBJS)
APP_OBJS = $(LEXER_BUILD)/Lexer_main.o
TEST_OBJS = $(LEXER_BUILD)/Lexer_tests.o


LEXER_FILE_TEST = $(LEXER_DIR)/Lexer_test.pc

all: $(LEXER_TEST_TARG) $(LEXER_APP_TARG)

$(LEXER_APP_TARG): $(COMMON_OBJS) $(APP_OBJS)
	$(CC) -o $@ $^ $(OPTS)

$(LEXER_TEST_TARG): $(COMMON_OBJS) $(TEST_OBJS)
	$(CC) -o $@ $^ $(OPTS)

$(LEXER_BUILD)/%.o: $(LEXER_DIR)/%.c | $(LEXER_BUILD)
	$(CC) $(OPTS) -I $(LEXER_DIR) -c $< -o $@

$(HELPER_BUILD)/%.o: $(HELPER_DIR)/%.c | $(HELPER_BUILD)
	$(CC) $(OPTS) -I $(HELPER_DIR) -c $< -o $@

$(LEXER_BUILD):
	mkdir -p $@

$(HELPER_BUILD):
	mkdir -p $@

test: $(LEXER_TEST_TARG)
	./$<

Lexer_parse:
	./$(LEXER_APP_TARG) $(LEXER_FILE_TEST)

clean:
	rm -rf $(LEXER_BUILD) $(HELPER_BUILD)

.PHONY: all test clean Lexer_parse