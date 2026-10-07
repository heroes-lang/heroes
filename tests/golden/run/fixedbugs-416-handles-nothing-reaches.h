/* Beside defect 416's cases, lane run400, 2026-10-06: two struct types a
 * handle can bind, one named only by its tag (`netdb.h`'s `struct addrinfo`
 * shape) and one a typedef names (`sqlite3.h`'s shape), each declared and
 * never defined. */
struct only_tag;
typedef struct named_s named_t;
