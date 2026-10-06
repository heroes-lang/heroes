/* Beside defect 400's cases, lane run400, 2026-10-06: a struct the header
 * names only by its tag and gives no typedef, the shape of `netdb.h`'s
 * `struct addrinfo`. The bare word `opaque` names nothing in C; only
 * `struct opaque` does. Declared and never defined, so a handle is the one
 * way to bind it, and shipped beside the cases so they run on every
 * platform. */
struct opaque;
