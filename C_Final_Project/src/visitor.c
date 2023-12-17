#include "include/visitor.h"
#include "include/scope.h"
#include <stdio.h>
#include <string.h>

static AST_T* builtin_function_print(visitor_T* visitor, AST_T** args, int args_size)
{
    for (int i = 0; i < args_size; i++)
    {
        AST_T* visited_ast = visitor_visit(visitor, args[i]);

        switch (visited_ast->type)
        {
            case AST_STRING: printf("%s\n", visited_ast->string_value); break;
            default: printf("%p\n", visited_ast); break;
        }
    }

    return init_ast(AST_NOOP);
}

static AST_T* builtin_function_exit(visitor_T* visitor, AST_T** args, int args_size)
{
    for (int i = 0; i < args_size; i++)
    {
        AST_T* visited_ast = visitor_visit(visitor, args[i]);

        switch (visited_ast->type)
        {
            case AST_NOOP: printf("You exited\n"); exit(0); break;
            default: printf("%p\n", visited_ast); break;
        }
    }

    return init_ast(AST_NOOP);
}

static AST_T* builtin_function_clear(visitor_T* visitor, AST_T** args, int args_size)
{
    for (int i = 0; i < args_size; i++)
    {
        AST_T* visited_ast = visitor_visit(visitor, args[i]);

        switch (visited_ast->type)
        {
            case AST_NOOP: system("clear"); break;
            default: printf("%p\n", visited_ast); break;
        }
    }

    return init_ast(AST_NOOP);
}

visitor_T* init_visitor() {
    visitor_T* visitor = calloc(1, sizeof(struct VISITOR_STRUCT));
    return visitor;
}

// Function to visit binary operation AST nodes
AST_T* visitor_visit_binary_op(visitor_T* visitor, AST_T* node) {
    // Ensure that the node is indeed a binary operation
    if (node->type != AST_BINARY_OP) {
        fprintf(stderr, "Attempt to visit a non-binary-op node as a binary op\n");
        exit(EXIT_FAILURE);
    }

    // Visit the left and right operands
    AST_T* left = visitor_visit(visitor, node->left);
    AST_T* right = visitor_visit(visitor, node->right);

    AST_T* result = init_ast(AST_LITERAL);
    // Perform the operation based on the operator
    // You will need to implement logic for each operator your language supports
    switch (node->op) {
        case '+': // Handle addition
            result->int_value = left->int_value + right->int_value;
            break;
        case '-': // Handle subtraction
            result->int_value = left->int_value - right->int_value;
            break;
        case '*': // Handle multiplication
            result->int_value = left->int_value * right->int_value;
            break;
        case '/': // Handle division
            if (right->int_value == 0) {
                fprintf(stderr, "Division by zero\n");
                exit(EXIT_FAILURE);
            }
            result->int_value = left->int_value / right->int_value;
            break;
        default:
            fprintf(stderr, "Unknown binary operator: %c\n", node->op);
            exit(EXIT_FAILURE);
    }

    return result;
}

// Function to visit unary operation AST nodes
AST_T* visitor_visit_unary_op(visitor_T* visitor, AST_T* node) {
    // Ensure that the node is indeed a unary operation
    if (node->type != AST_UNARY_OP) {
        fprintf(stderr, "Attempt to visit a non-unary-op node as a unary op\n");
        exit(EXIT_FAILURE);
    }

    // Visit the operand
    AST_T* visited_operand = visitor_visit(visitor, node->unary_operand);

    // Perform the operation based on the operator
    AST_T* result = init_ast(AST_LITERAL);
    switch (node->op) {
        case '-':
            // Here we check if the operand is an integer or a float and negate accordingly
            if (visited_operand->type == AST_TYPE_INT) { // Assuming you have defined AST_TYPE_INT for integer literals
                result->type = AST_TYPE_INT;
                result->int_value = -visited_operand->int_value; // Negate the int value
            } else if (visited_operand->type == AST_TYPE_FLOAT) { // And AST_TYPE_FLOAT for float literals
                result->type = AST_TYPE_FLOAT;
                result->float_value = -visited_operand->float_value; // Negate the float value
            } else {
                fprintf(stderr, "Unary '-' operator not supported for the given type\n");
                exit(EXIT_FAILURE);
            }
            break;
        // Implement other unary operations as needed...
        default:
            fprintf(stderr, "Unknown unary operator: %c\n", node->op);
            exit(EXIT_FAILURE);
    }
    
    return result;
}

AST_T* visitor_visit_literal(visitor_T* visitor, AST_T* node) {
    return node;
}

AST_T* visitor_visit(visitor_T* visitor, AST_T* node) {
    switch (node->type) {
        case AST_VARIABLE_DEFINITION:
            return visitor_visit_variable_definition(visitor, node);
        case AST_FUNCTION_DEFINITION:
            return visitor_visit_function_definition(visitor, node);
        case AST_VARIABLE:
            return visitor_visit_variable(visitor, node);
        case AST_FUNCTION_CALL:
            return visitor_visit_function_call(visitor, node);
        case AST_STRING:
            return visitor_visit_string(visitor, node);
        case AST_COMPOUND:
            return visitor_visit_compound(visitor, node);
        case AST_NOOP:
            return node;
        case AST_BINARY_OP:
            return visitor_visit_binary_op(visitor, node);
        case AST_UNARY_OP:
            return visitor_visit_unary_op(visitor, node);
        case AST_LITERAL:
            return visitor_visit_literal(visitor, node);
        default:
            fprintf(stderr, "Uncaught statement of type %d\n", node->type);
            exit(1);
    }
    return init_ast(AST_NOOP);
}

AST_T* visitor_visit_variable_definition(visitor_T* visitor, AST_T* node)
{
    scope_add_variable_definition(
        node->scope,
        node        
    ); 

    return node;
}

AST_T* visitor_visit_function_definition(visitor_T* visitor, AST_T* node)
{
    scope_add_function_definition(
        node->scope,
        node        
    );

    return node;
}

AST_T* visitor_visit_variable(visitor_T* visitor, AST_T* node)
{
    AST_T* vdef = scope_get_variable_definition(
        node->scope,
        node->variable_name
    );
    
    if (vdef != (void*) 0)
        return visitor_visit(visitor, vdef->variable_definition_value);

    printf("Undefined variable `%s`\n", node->variable_name);
    exit(1);
}

AST_T* visitor_visit_function_call(visitor_T* visitor, AST_T* node)
{
    if (strcmp(node->function_call_name, "print") == 0)
    {
        return builtin_function_print(visitor, node->function_call_arguments, node->function_call_arguments_size);
    }

    if (strcmp(node->function_call_name, "exit") == 0)
    {
        return builtin_function_exit(visitor, node->function_call_arguments, node->function_call_arguments_size);
    }

    if (strcmp(node->function_call_name, "clear") == 0)
    {
        return builtin_function_clear(visitor, node->function_call_arguments, node->function_call_arguments_size);
    }

    AST_T* fdef = scope_get_function_definition(
        node->scope,
        node->function_call_name
    );

    if (fdef == (void*)0)
    {
        printf("Undefined method `%s`\n", node->function_call_name);
        exit(1);
    }

    for (int i = 0; i < (int) node->function_call_arguments_size; i++)
    {
        // grab the variable from the function definition arguments
        AST_T* ast_var = (AST_T*) fdef->function_definition_args[i];

        // grab the value from the function call arguments
        AST_T* ast_value = (AST_T*) node->function_call_arguments[i];

        // create a new variable definition with the value of the argument
        // in the function call.
        AST_T* ast_vardef = init_ast(AST_VARIABLE_DEFINITION);
        ast_vardef->variable_definition_value = ast_value;

        // copy the name from the function definition argument into the new
        // variable definition
        ast_vardef->variable_definition_variable_name = (char*) calloc(strlen(ast_var->variable_name) + 1, sizeof(char));
        strcpy(ast_vardef->variable_definition_variable_name, ast_var->variable_name);

        // push our variable definition into the function body scope.
        scope_add_variable_definition(fdef->function_definition_body->scope, ast_vardef);
    }
    
    return visitor_visit(visitor, fdef->function_definition_body);
}

AST_T* visitor_visit_string(visitor_T* visitor, AST_T* node)
{
    return node;
}

AST_T* visitor_visit_compound(visitor_T* visitor, AST_T* node)
{
    for (int i = 0; i < node->compound_size; i++)
    {
        visitor_visit(visitor, node->compound_value[i]);
    }

    return init_ast(AST_NOOP);
}