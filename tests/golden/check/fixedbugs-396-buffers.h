#ifndef FIXEDBUGS_396_BUFFERS_CHECK_H
#define FIXEDBUGS_396_BUFFERS_CHECK_H

/* Defect 396's declaration shapes (lane b13-land-buf, 2026-10-07). */
#include <stdint.h>
#include <stdio.h>

struct pfd { int32_t fd; int16_t events; int16_t revents; };
typedef struct opaque *Opaque;

void digest32(unsigned char *md);
void no_extent(unsigned char *md);
void strings(char **out);
void handles(Opaque *out);
void nowhere(unsigned char *md);
void by_value(unsigned char *md);
void reals(double *p);
int records(struct pfd *p, unsigned n);

#endif
