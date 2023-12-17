#ifndef SCOPE_H
#define SCOPE_H
#include "AST.h"

// Structure to represent a scope in the programming language
typedef struct SCOPE_STRUCT {
    AST_T** function_definitions;       // Array of function definitions in the scope
    size_t function_definitions_size;   // Number of function definitions

    AST_T** variable_definitions;       // Array of variable definitions in the scope
    size_t variable_definitions_size;   // Number of variable definitions
} scope_T;

// Function prototypes for scope management
scope_T* init_scope();

// Adds a function definition to the scope
AST_T* scope_add_function_definition(scope_T* scope, AST_T* fdef);

// Retrieves a function definition from the scope by its name
AST_T* scope_get_function_definition(scope_T* scope, const char* fname);

// Adds a variable definition to the scope
AST_T* scope_add_variable_definition(scope_T* scope, AST_T* vdef);

// Retrieves a variable definition from the scope by its name
AST_T* scope_get_variable_definition(scope_T* scope, const char* name);

#endif