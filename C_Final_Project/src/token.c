#include "include/token.h"
#include <stdlib.h>
#include <string.h>

token_T* init_token(int type, char* value) {
    token_T* token = calloc(1, sizeof(struct TOKEN_STRUCT));
    token->type = type;

    // Duplicate the value string to ensure the token owns its data
    if (value) {
        token->value = strdup(value);
    } else {
        token->value = NULL;
    }

    return token;
}