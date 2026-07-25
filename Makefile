CC ?= clang
OPTS = -std=c11 -Wall -Wextra -Wpedantic -fsanitize=address -g


SRC_DIR = src
TEST_DIR = tests
BUILD = build
BUILD_SRC = $(BUILD)/src
BUILD_TEST = $(BUILD)/tests
BUILD_HELPER = $(BUILD_SRC)/helper
BUILD_HELPER_TEST = $(BUILD_TEST)/helper
BUILD_LEXER = $(BUILD_SRC)/Lexer
BUILD_LEXER_TEST = $(BUILD_TEST)/Lexer
BUILD_PARSER = $(BUILD_SRC)/Parser
BUILD_PARSER_TEST = $(BUILD_TEST)/Parser

HELPER_DIR = $(SRC_DIR)/helper
HELPER_TEST_DIR = $(TEST_DIR)/helper
HELPER_TEST_TARGS = $(BUILD_HELPER_TEST)/DynamicString_test $(BUILD_HELPER_TEST)/string_tests
HELPER_OBJS = $(BUILD_HELPER)/string_functions.o $(BUILD_HELPER)/DynamicString.o
HELPER_TEST_OBJS = $(BUILD_HELPER_TEST)/string_tests.o $(BUILD_HELPER_TEST)/DynamicString_test.o

LEXER_DIR = $(SRC_DIR)/Lexer
LEXER_APP_TARG = $(BUILD_LEXER)/Lexer_main
LEXER_TEST_DIR = $(TEST_DIR)/Lexer
LEXER_TEST_TARG = $(BUILD_LEXER_TEST)/Lexer_test
LEXER_FILE_TEST = $(LEXER_TEST_DIR)/Lexer_test.pc
LEXER_OBJ = $(BUILD_LEXER)/Lexer_functions.o 
LEXER_MAIN_OBJ = $(BUILD_LEXER)/Lexer_main.o
LEXER_TEST_OBJ = $(BUILD_LEXER_TEST)/Lexer_tests.o

PARSER_DIR = $(SRC_DIR)/Parser
PARSER_APP_TARG = $(BUILD_PARSER)/Parser_main
PARSER_TEST_DIR = $(TEST_DIR)/Parser
PARSER_TEST_TARG = $(BUILD_PARSER_TEST)/Parser_test
PARSER_OBJ = $(BUILD_PARSER)/Parser_functions.o 
PARSER_MAIN_OBJ = $(BUILD_PARSER)/Parser_main.o
PARSER_TEST_OBJ = $(BUILD_PARSER_TEST)/Parser_tests.o


all: $(HELPER_TEST_TARGS) $(LEXER_APP_TARG) $(LEXER_TEST_TARG) $(PARSER_APP_TARG) $(PARSER_TEST_TARG)

# Targets

$(BUILD_HELPER_TEST)/DynamicString_test: $(BUILD_HELPER_TEST)/DynamicString_test.o $(HELPER_OBJS)
	$(CC) -o $@ $^ $(OPTS)

$(BUILD_HELPER_TEST)/string_tests: $(BUILD_HELPER_TEST)/string_tests.o $(HELPER_OBJS)
	$(CC) -o $@ $^ $(OPTS)

$(LEXER_APP_TARG): $(HELPER_OBJS) $(LEXER_OBJ) $(LEXER_MAIN_OBJ)
	$(CC) -o $@ $^ $(OPTS)

$(LEXER_TEST_TARG): $(HELPER_OBJS) $(LEXER_OBJ) $(LEXER_TEST_OBJ)
	$(CC) -o $@ $^ $(OPTS)

$(PARSER_APP_TARG): $(HELPER_OBJS) $(LEXER_OBJ) $(PARSER_OBJ) $(PARSER_MAIN_OBJ)
	$(CC) -o $@ $^ $(OPTS)

$(PARSER_TEST_TARG): $(HELPER_OBJS) $(LEXER_OBJ) $(PARSER_OBJ) $(PARSER_TEST_OBJ)
	$(CC) -o $@ $^ $(OPTS)

# Build object files

$(BUILD_HELPER)/%.o: $(HELPER_DIR)/%.c | $(BUILD_HELPER)
	$(CC) $(OPTS) -I $(HELPER_DIR) -c $< -o $@

$(BUILD_LEXER)/%.o: $(LEXER_DIR)/%.c | $(BUILD_LEXER)
	$(CC) $(OPTS) -I $(LEXER_DIR) -c $< -o $@

$(BUILD_PARSER)/%.o: $(PARSER_DIR)/%.c | $(BUILD_PARSER)
	$(CC) $(OPTS) -I $(PARSER_DIR) -c $< -o $@

$(BUILD_HELPER_TEST)/%.o: $(HELPER_TEST_DIR)/%.c | $(BUILD_HELPER_TEST)
	$(CC) $(OPTS) -I $(HELPER_TEST_DIR) -c $< -o $@

$(BUILD_LEXER_TEST)/%.o: $(LEXER_TEST_DIR)/%.c | $(BUILD_LEXER_TEST)
	$(CC) $(OPTS) -I $(LEXER_TEST_DIR) -c $< -o $@

$(BUILD_PARSER_TEST)/%.o: $(PARSER_TEST_DIR)/%.c | $(BUILD_PARSER_TEST)
	$(CC) $(OPTS) -I $(PARSER_TEST_DIR) -c $< -o $@

# Directory creation

$(BUILD_HELPER):
	mkdir -p $@

$(BUILD_LEXER):
	mkdir -p $@

$(BUILD_PARSER):
	mkdir -p $@

$(BUILD_HELPER_TEST):
	mkdir -p $@

$(BUILD_LEXER_TEST):
	mkdir -p $@

$(BUILD_PARSER_TEST):
	mkdir -p $@



test: $(HELPER_TEST_TARGS) $(LEXER_TEST_TARG) $(PARSER_TEST_TARG)
	for t in $^; do ./$$t; done

Lexer_parse: $(LEXER_APP_TARG) $(LEXER_FILE_TEST)
	./$(LEXER_APP_TARG) $(LEXER_FILE_TEST)

clean:
	rm -rf $(BUILD)

.PHONY: all test clean Lexer_parse