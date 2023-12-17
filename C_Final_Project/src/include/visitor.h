#ifndef VISITOR_H
#define VISITOR_H
#include "AST.h"
#include "scope.h"  // Assuming you use scopes for variable management

// Visitor structure for traversing and interpreting the AST
typedef struct VISITOR_STRUCT {
    // Current scope: Useful for tracking variables and their values
    scope_T* current_scope;

    // Error handling fields
    int error_flag;  // A flag to indicate if an error has occurred
    char* error_message;  // A message describing the error

    int recursion_depth;  // To track recursion depth if needed

} visitor_T;

// Function prototypes for the visitor
visitor_T* init_visitor();

AST_T* visitor_visit(visitor_T* visitor, AST_T* node);
AST_T* visitor_visit_variable_definition(visitor_T* visitor, AST_T* node);
AST_T* visitor_visit_function_definition(visitor_T* visitor, AST_T* node);
AST_T* visitor_visit_variable(visitor_T* visitor, AST_T* node);
AST_T* visitor_visit_function_call(visitor_T* visitor, AST_T* node);
AST_T* visitor_visit_string(visitor_T* visitor, AST_T* node);
AST_T* visitor_visit_compound(visitor_T* visitor, AST_T* node);
AST_T* visitor_visit_binary_op(visitor_T* visitor, AST_T* node);
AST_T* visitor_visit_unary_op(visitor_T* visitor, AST_T* node);

// Declare the visitor_visit_literal function
AST_T* visitor_visit_literal(visitor_T* visitor, AST_T* node);

#endif // VISITOR_H