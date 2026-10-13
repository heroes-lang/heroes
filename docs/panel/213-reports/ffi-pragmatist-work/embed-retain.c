#include <stdio.h>
#include <stdint.h>
typedef struct h_m_Point { int64_t f_x; int64_t f_y; } h_m_Point;

#ifdef __APPLE__
#define HERO_LLDB_SUMMARY __attribute__((used, section("__TEXT,__lldbsummaries")))
#else
#define HERO_LLDB_SUMMARY __attribute__((used, retain, section(".lldbsummaries")))
#endif
/* version 1, record size, type-name size, name + NUL, summary size, summary + NUL */
#define NAME "h_m_Point"
#define SUMM "Point(x: ${var.f_x}, y: ${var.f_y})"
HERO_LLDB_SUMMARY static const unsigned char h_m_Point_lldb_summary[] =
    "\x01"
    "\x30"                      /* 1 + 10 + 1 + 36 = 48 */
    "\x0a" NAME "\0"
    "\x24" SUMM;                /* 35 chars + NUL = 36 */

int main(void) {
    h_m_Point p = {3, 4};
    printf("%lld\n", (long long)(p.f_x + p.f_y));
    return 0;
}
