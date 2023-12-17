#include "include\lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

lexer_T* init_lexer(char* contents) {
    lexer_T* lexer = calloc(1, sizeof(struct LEXER_STRUCT));
    lexer->contents = contents;
    lexer->i = 0;
    lexer->c = contents[lexer->i];

    return lexer;
}

void lexer_advance(lexer_T* lexer) {
    if (lexer->c != '\0' && lexer->i < strlen(lexer->contents)) {
        lexer->i += 1;
        lexer->c = lexer->contents[lexer->i];
    }
}

void lexer_skip_whitespace(lexer_T* lexer) {
    while (lexer->c == ' ' || lexer->c == 10) {
        lexer_advance(lexer);
    }
}

token_T* lexer_get_next_token(lexer_T* lexer) {
    while (lexer->c != '\0' && lexer->i < strlen(lexer->contents)) {
        if (lexer->c == ' ' || lexer->c == 10)
            lexer_skip_whitespace(lexer);

        if (isalnum(lexer->c))
            return lexer_collect_id(lexer);

        switch (lexer->c) {
            // Add cases for each specific operator and symbol your language uses
            case '+': return lexer_advance_with_token(lexer, init_token(TOKEN_PLUS, lexer_get_current_char_as_string(lexer))); break;
            case '-': return lexer_advance_with_token(lexer, init_token(TOKEN_MINUS, lexer_get_current_char_as_string(lexer))); break;
            case '*': return lexer_advance_with_token(lexer, init_token(TOKEN_STAR, lexer_get_current_char_as_string(lexer))); break;
            case '/': return lexer_advance_with_token(lexer, init_token(TOKEN_SLASH, lexer_get_current_char_as_string(lexer))); break;
            case '(': return lexer_advance_with_token(lexer, init_token(TOKEN_LPAREN, lexer_get_current_char_as_string(lexer))); break;
            case ')': return lexer_advance_with_token(lexer, init_token(TOKEN_RPAREN, lexer_get_current_char_as_string(lexer))); break;
            case '{': return lexer_advance_with_token(lexer, init_token(TOKEN_LBRACE, lexer_get_current_char_as_string(lexer))); break;
            case '}': return lexer_advance_with_token(lexer, init_token(TOKEN_RBRACE, lexer_get_current_char_as_string(lexer))); break;
            case '%': return lexer_advance_with_token(lexer, init_token(TOKEN_MODULO, lexer_get_current_char_as_string(lexer))); break;
            case '^': return lexer_advance_with_token(lexer, init_token(TOKEN_POWER, lexer_get_current_char_as_string(lexer))); break;
            // ... (other symbols)
        }

        // If no case matches, print an error and exit
        fprintf(stderr, "Unexpected character: %c\n", lexer->c);
        exit(EXIT_FAILURE);
    }

    return init_token(TOKEN_EOF, "\0");
}

token_T* lexer_collect_id(lexer_T* lexer) {
    char* value = calloc(1, sizeof(char));
    value[0] = '\0';

    while (isalnum(lexer->c) || lexer->c == '_') {
        char* s = lexer_get_current_char_as_string(lexer);
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);

        lexer_advance(lexer);
    }

    // Check if the identifier matches any specific keyword in your language
    return lexer_check_keyword(value);
}

token_T* lexer_check_keyword(char* value) {
    if (strcmp(value, "sin") == 0) return init_token(TOKEN_SIN, value);
    if (strcmp(value, "cos") == 0) return init_token(TOKEN_COS, value);
    if (strcmp(value, "tan") == 0) return init_token(TOKEN_TAN, value);
    if (strcmp(value, "asin") == 0) return init_token(TOKEN_ASIN, value);
    if (strcmp(value, "acos") == 0) return init_token(TOKEN_ACOS, value);
    if (strcmp(value, "atan") == 0) return init_token(TOKEN_ATAN, value);
    if (strcmp(value, "log") == 0) return init_token(TOKEN_LOG, value);
    if (strcmp(value, "exp") == 0) return init_token(TOKEN_EXP, value);
    if (strcmp(value, "sqrt") == 0) return init_token(TOKEN_SQRT, value);
    if (strcmp(value, "abs") == 0) return init_token(TOKEN_ABS, value);
    if (strcmp(value, "pi") == 0) return init_token(TOKEN_PI, value);
    if (strcmp(value, "e") == 0) return init_token(TOKEN_E, value);
    if (strcmp(value, "derive") == 0) return init_token(TOKEN_DERIVATE, value);
    if (strcmp(value, "integrate") == 0) return init_token(TOKEN_INTEGRATE, value);
    
    return init_token(TOKEN_ID, value); // Default case for identifiers
}

token_T* lexer_advance_with_token(lexer_T* lexer, token_T* token) {
    lexer_advance(lexer);
    return token;
}

char* lexer_get_current_char_as_string(lexer_T* lexer) {
    char* str = calloc(2, sizeof(char));
    str[0] = lexer->c;
    str[1] = '\0';

    return str;
}
