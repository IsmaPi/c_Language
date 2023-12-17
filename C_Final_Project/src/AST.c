#include "include/AST.h"

AST_T* init_ast(AST_TYPE type) {
    AST_T* ast = calloc(1, sizeof(struct AST_STRUCT));
    ast->type = type;

    // Initialize all pointers and values to NULL or 0
    ast->scope = NULL;

    /* AST_VARIABLE_DEFINITION */
    ast->variable_definition_variable_name = NULL;
    ast->variable_definition_value = NULL;

    /* AST_FUNCTION_DEFINITION */
    ast->function_definition_body = NULL;
    ast->function_definition_name = NULL;
    ast->function_definition_args = NULL;
    ast->function_definition_args_size = 0;

    /* AST_VARIABLE */
    ast->variable_name = NULL;

    /* AST_FUNCTION_CALL */
    ast->function_call_name = NULL;
    ast->function_call_arguments = NULL;
    ast->function_call_arguments_size = 0;

    /* AST_STRING */
    ast->string_value = NULL;

    /* AST_COMPOUND */
    ast->compound_value = NULL;
    ast->compound_size = 0;

    /* AST_LITERAL */
    // Initialize the literal value based on type
    if (type == AST_LITERAL) {
        ast->int_value = 0; // Default to 0 for integer literals
        ast->float_value = 0.0f; // Default to 0.0 for float literals
    }

    /* AST_BINARY_OP and AST_UNARY_OP */
    ast->left = NULL;
    ast->right = NULL;
    ast->op = 0; // Initialize operator to null character

    return ast;
}

AST_T* init_ast_with_two_children(AST_TYPE type, AST_T* left, AST_T* right) {
    AST_T* ast = init_ast(type);
    ast->left = left;
    ast->right = right;
    return ast;
}