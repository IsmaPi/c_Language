#ifndef AST_H
#define AST_H
#include <stdlib.h>

// Define types for AST nodes
typedef enum {
    AST_VARIABLE_DEFINITION,
    AST_FUNCTION_DEFINITION,
    AST_VARIABLE,
    AST_FUNCTION_CALL,
    AST_STRING,
    AST_COMPOUND,
    AST_NOOP,
    AST_LITERAL,            // For numeric literals (integers, floats)
    AST_BINARY_OP,          // For binary operations (+, -, *, /, etc.)
    AST_UNARY_OP,            // For unary operations (like -x or custom unary functions)
    AST_TYPE_FLOAT,
    AST_TYPE_INT
} AST_TYPE;

typedef struct AST_STRUCT {
    AST_TYPE type;
    struct SCOPE_STRUCT* scope;

    /* AST_VARIABLE_DEFINITION */
    char* variable_definition_variable_name;
    struct AST_STRUCT* variable_definition_value;

    /* AST_FUNCTION_DEFINITION */
    struct AST_STRUCT* function_definition_body;
    char* function_definition_name;
    struct AST_STRUCT** function_definition_args;
    size_t function_definition_args_size;

    /* AST_VARIABLE */
    char* variable_name;

    /* AST_FUNCTION_CALL */
    char* function_call_name;
    struct AST_STRUCT** function_call_arguments;
    size_t function_call_arguments_size;

    /* AST_STRING */
    char* string_value;

    /* AST_COMPOUND */
    struct AST_STRUCT** compound_value;
    size_t compound_size;

    /* AST_LITERAL */
    union {
        int int_value;
        float float_value;
    };

    /* AST_BINARY_OP and AST_UNARY_OP */
    struct AST_STRUCT* left;
    struct AST_STRUCT* right; // For binary ops, null for unary
    char op; // Operator character (e.g., '+', '-', '*', '/')

    struct AST_STRUCT* unary_operand; // Field for the operand of unary operations

} AST_T;

AST_T* init_ast(AST_TYPE type);

#endif
