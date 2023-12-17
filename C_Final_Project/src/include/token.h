#ifndef TOKEN_H
#define TOKEN_H

// Token types for the mathematical programming language
typedef enum {
    TOKEN_ID,           // Identifier
    TOKEN_EQUALS,       // Equals '='
    TOKEN_STRING,       // String literals
    TOKEN_SEMI,         // Semicolon ';'
    TOKEN_LPAREN,       // Left parenthesis '('
    TOKEN_RPAREN,       // Right parenthesis ')'
    TOKEN_LBRACE,       // Left brace '{'
    TOKEN_RBRACE,       // Right brace '}'
    TOKEN_COMMA,        // Comma ','
    TOKEN_PLUS,         // Plus '+'
    TOKEN_MINUS,        // Minus '-'
    TOKEN_STAR,         // Star '*'
    TOKEN_SLASH,        // Slash '/'
    TOKEN_MODULO,       // Modulo '%'
    TOKEN_POWER,        // Power '^'
    // Mathematical functions
    TOKEN_SIN,          // sin function
    TOKEN_COS,          // cos function
    TOKEN_TAN,          // tan function
    TOKEN_ASIN,         // asin function
    TOKEN_ACOS,         // acos function
    TOKEN_ATAN,         // atan function
    TOKEN_LOG,          // log function
    TOKEN_EXP,          // exp function
    TOKEN_SQRT,         // sqrt function
    TOKEN_ABS,          // abs function
    // Mathematical constants and operations
    TOKEN_PI,           // Pi 'π'
    TOKEN_E,            // Euler's number 'e'
    TOKEN_DERIVATIVE,   // derivative operation
    TOKEN_INTEGRAL,     // integral operation
    TOKEN_EOF           // End of file
} TokenType;

typedef struct TOKEN_STRUCT {
    TokenType type;  // Type of the token
    char* value;     // Value of the token (if applicable)
} token_T;

token_T* init_token(TokenType type, char* value);

#endif