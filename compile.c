#include <stdio.h>

#include "common.h"
#ifdef DEBUG_LOG_GC
#include <stdio.h>
#include "debug.h"
#endif
#include "compiler.h"
#include "scanner.h"

void compile(const char* source) {
    initScanner(source);
    int line = -1;
    for (;;) {
        Token token = scanToken(); 
        if (token.line != line) {
            printf("%4d",token.line);
            line = token.line;
        } else {
            printf(" | "); 
        }
        printf("%2d '%.*s'", token.type, token.length, token.start);

        if (token.type == TOKEN_EOF) break;
    }
}
