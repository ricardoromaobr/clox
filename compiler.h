#ifndef clox_compile_h
#define clox_compile_h

#include "chunk.h"
#include "object.h"

ObjFunction* compile(const char* source);

#endif