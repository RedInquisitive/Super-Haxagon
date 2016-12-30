#pragma once

#include <3ds.h>
#include <stdio.h>

#include "triangle.h"

#ifdef __cplusplus
#include "font/source/BmpFont.h"
extern "C" {
#endif

void writeFont(Point p, const char* s, bool large);

#ifdef __cplusplus
}
#endif
