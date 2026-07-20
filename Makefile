CC ?= clang
OPTS = -std=c99 -Wall -Wextra -Wpedantic -fsanitize=address -g

LEXER_DIR = lexer
LEXER_BUILD = lexer/build
LEXER_TEST_TARG = $(LEXER_BUILD)/main_test

LEXER_FILES = $(shell find $(LEXER_DIR) -name '*.c')
LEXER_OBJ_FILES = $(patsubst $(LEXER_DIR)/%.c, $(LEXER_BUILD)/%.o, $(LEXER_FILES))

all: $(LEXER_TEST_TARG)

$(LEXER_TEST_TARG): $(LEXER_OBJ_FILES)
	$(CC) -o $(LEXER_TEST_TARG) $(LEXER_OBJ_FILES) $(OPTS)

$(LEXER_BUILD)/%.o: $(LEXER_DIR)/%.c | $(LEXER_BUILD)
	$(CC) $(OPTS) -I $(LEXER_DIR) -c $< -o $@

$(LEXER_BUILD):
	mkdir -p $@

test: $(LEXER_TEST_TARG)
	./$<

clean:
	rm -rf $(LEXER_BUILD)

.PHONY: all test clean