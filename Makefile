CC ?= clang
OPTS = -std=c99 -Wall -Wextra -Wpedantic -fsanitize=address -g

LEXER_DIR = lexer
LEXER_BUILD = lexer/build
LEXER_APP_TARG = $(LEXER_BUILD)/lexer_app
LEXER_TEST_TARG = $(LEXER_BUILD)/main_test

COMMON_OBJS = $(LEXER_BUILD)/Lexer_functions.o
APP_OBJS = $(LEXER_BUILD)/Lexer_main.o
TEST_OBJS = $(LEXER_BUILD)/Lexer_tests.o

all: $(LEXER_TEST_TARG) $(LEXER_APP_TARG)

$(LEXER_APP_TARG): $(COMMON_OBJS) $(APP_OBJS)
	$(CC) -o $@ $^ $(OPTS)

$(LEXER_TEST_TARG): $(COMMON_OBJS) $(TEST_OBJS)
	$(CC) -o $@ $^ $(OPTS)

$(LEXER_BUILD)/%.o: $(LEXER_DIR)/%.c | $(LEXER_BUILD)
	$(CC) $(OPTS) -I $(LEXER_DIR) -c $< -o $@

$(LEXER_BUILD):
	mkdir -p $@

test: $(LEXER_TEST_TARG)
	./$<

clean:
	rm -rf $(LEXER_BUILD)

.PHONY: all test clean