/* What the layer's two number doors promise, asked in C (panel 246's R6;
 * lane b34-layer, 2026-11-13), where Heroes' own marks stop a program before
 * C runs: a handle given up for its number names nothing, the handle taken
 * back is a new one, the number is taken back once, and a number the layer
 * never gave, a negative one among them, is refused. Each answers 1, or the
 * negative step that did not hold. The calls here begin and end the
 * number's own life in the program's handle set, so the leak check after
 * `main` finds it balanced. */
#include "hero_net.h"

#include <stdint.h>

static inline int64_t given_up_names_nothing(void) {
    hero_net_socket *s = hero_net_listen("127.0.0.1", 0);

    if (s == NULL) return -1;
    int64_t port = hero_net_port(s);
    int64_t number = hero_net_number(s);

    if (number <= 0) return -2;
    if (hero_net_port(s) != -1 || hero_net_last_error() != HERO_NET_NOT_OPEN) return -3;
    if (hero_net_close(s) != -1) return -4;
    hero_net_socket *back = hero_net_of(number);

    if (back == NULL) return -5;
    if (back == s) return -6;
    if (hero_net_port(back) != port) return -7;
    if (hero_net_port(s) != -1) return -8;
    if (hero_net_of(number) != NULL || hero_net_last_error() != HERO_NET_NOT_OPEN) return -9;
    if (hero_net_close(back) != 0) return -10;
    return 1;
}

static inline int64_t never_given_is_refused(void) {
    int64_t forged[] = { -1, 0, 12345, INT64_C(0x4000000000000000), INT64_C(0x4000000000ffffff), INT64_MIN };

    for (int i = 0; i < 6; i += 1) {
        if (hero_net_of(forged[i]) != NULL) return -1 - i;
        if (hero_net_last_error() != HERO_NET_NOT_OPEN) return -10 - i;
    }
    if (hero_net_number(NULL) != -1 || hero_net_last_error() != HERO_NET_NOT_OPEN) return -20;
    return 1;
}
