#include "s3.h"
#include "u.h"
void f1(void) { S3 v = {.a = 0, .c = 0}; (void)v; }
void f2(void) { SA v = {.kind = 0, .i = 0}; (void)v; }
void f3(void) { SA v = {.kind = 0, .i = 0, .f = 0, .x = 0}; (void)v; }
