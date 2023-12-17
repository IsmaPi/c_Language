#ifndef VISITOR_H
#define VISITOR_H
#include "AST.h"

// Visitor structure for traversing and interpreting the AST
typedef struct VISITOR_STRUCT {
    // You can add fields here if you need to maintain state
    // within the visitor (e.g., for storing results, managing environment, etc.)
} visitor_T;

// Function prototypes for the visitor
visitor_T* init_visitor();

// Core visit functions for different AST node types
AST_T* visitor_visit(visitor_T* visitor, AST_T* node);
AST_T* visitor_visit_variable_definition(visitor_T* visitor, AST_T* node);
AST_T* visitor_visit_function_definition(visitor_T* visitor, AST_T* node);
AST_T* visitor_visit_variable(visitor_T* visitor, AST_T* node);
AST_T* visitor_visit_function_call(visitor_T* visitor, AST_T* node);
AST_T* visitor_visit_string(visitor_T* visitor, AST_T* node);
AST_T* visitor_visit_compound(visitor_T* visitor, AST_T* node);

// Add additional visit functions as needed for handling specific AST node types
// such as mathematical operations, literals, etc.
AST_T* visitor_visit_binary_op(visitor_T* visitor, AST_T* node);
AST_T* visitor_visit_unary_op(visitor_T* visitor, AST_T* node);
AST_T* visitor_visit_literal(visitor_T* visitor, AST_T* node);

#endif