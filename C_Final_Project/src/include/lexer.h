#ifndef LEXER_H
#define LEXER_H
#include "token.h"

typedef struct LEXER_STRUCT {
    char c;             
    unsigned int i;     
    char* contents;
} lexer_T;

// Initializes the lexer with the given contents
lexer_T* init_lexer(char* contents);

// Advances the lexer to the next character
void lexer_advance(lexer_T* lexer);

// Skips whitespace characters
void lexer_skip_whitespace(lexer_T* lexer);

// Retrieves the next token from the source
token_T* lexer_get_next_token(lexer_T* lexer);

// Collects and returns a string token
token_T* lexer_collect_string(lexer_T* lexer);

// Collects and returns an identifier or keyword token
token_T* lexer_collect_id(lexer_T* lexer);

// Advances the lexer and returns the given token
token_T* lexer_advance_with_token(lexer_T* lexer, token_T* token);

// Returns the current character as a string
char* lexer_get_current_char_as_string(lexer_T* lexer);

// Checks if an identifier matches any specific keyword and returns the appropriate token
token_T* lexer_check_keyword(char* value);

#endif