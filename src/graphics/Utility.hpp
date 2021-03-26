#pragma once

#include <cstdio>
#include <cstdlib>
#include <iostream>

#ifdef _WIN32
#   define ASSERT(x) if(!(x)) __debugbreak();
#else
#   define ASSERT(x) if(!(x)) { std::cerr << "FATAL ASSERTION\n"; std::exit(-1); }
#endif

#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);