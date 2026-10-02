/* Beside defect 150's and 151's run cases, lane land186, 2026-10-02 (panel
   186): the unions the header's layout now reads, each with a maker, so no
   case leans on a system header. */
#include <stdint.h>
#include <string.h>

/* a union read through two of its members (defect 150) */
typedef union { int32_t i; uint32_t n; } W;
static inline W make_w(void) { W w; w.i = 7; return w; }

/* a struct holding an anonymous union, bound by one arm (defect 151) */
typedef struct { int32_t kind; union { int32_t i; float f; }; int32_t x; } SA;
static inline SA make_sa(int32_t i) { SA s; memset(&s, 0, sizeof s); s.kind = 1; s.i = i; s.x = 3; return s; }

/* a member with no bytes on every target: a zero-length array. A GNU empty
   struct stood beside it until 2026-10-02, and has 4 bytes for MSVC's target
   (the Windows box's clang 23.1.1), so it is witnessed for both targets by
   `selfhost/cli/layout.hero`'s test rather than held here. */
typedef struct { int32_t z[0]; int32_t x; } ZE;
static inline ZE make_ze(void) { ZE v; memset(&v, 0, sizeof v); v.x = 4; return v; }

/* a union holding an anonymous struct, bound by its wide arm */
typedef struct { int32_t kind; union { struct { int32_t a; int32_t b; }; int64_t q; }; } UAS;
static inline UAS make_uas(void) { UAS v; memset(&v, 0, sizeof v); v.kind = 2; v.a = 4; v.b = 5; return v; }

/* Compared (panel 186 R3): arms as wide as their unions, an integer, an
   array of bytes and a pointer, and two a header macro reaches into a named
   union, the way libc's `s6_addr` and `sa_handler` are reached. */
typedef struct { int32_t kind; union { int8_t b; int64_t q; }; } SB;
static inline SB make_sb(int64_t q) { SB s; memset(&s, 0, sizeof s); s.kind = 1; s.q = q; return s; }
typedef struct { int32_t kind; union { uint8_t bytes[4]; uint32_t word; }; } SARR;
static inline SARR make_sarr(uint32_t w) { SARR s; memset(&s, 0, sizeof s); s.kind = 1; s.word = w; return s; }
typedef struct { int32_t kind; union { void *p; intptr_t n; }; } SPTR;
static inline SPTR make_sptr(int64_t n) { SPTR s; memset(&s, 0, sizeof s); s.kind = 1; s.n = (intptr_t)n; return s; }
typedef struct { union { uint8_t a8[16]; uint32_t a32[4]; } addr_u; } ADDR;
#define addr_bytes addr_u.a8
static inline ADDR make_addr(uint8_t last) { ADDR a; memset(&a, 0, sizeof a); a.addr_u.a8[15] = last; return a; }
typedef struct { union { void (*one)(int); void (*two)(int, void *); } on_u; int32_t flags; } ACT;
#define act_one on_u.one
static inline ACT make_act(int32_t flags) { ACT a; memset(&a, 0, sizeof a); a.flags = flags; return a; }

/* panel 077's two union goldens, compared by an arm as wide as the union */
typedef union { int32_t i; float f; } U;
static inline U make_u(int32_t i) { U u; u.i = i; return u; }

/* a packed union as wide as its five-byte arm, which no formula over its
   members' alignments admits */
#pragma pack(push, 1)
typedef struct { uint8_t k; union { uint8_t c[5]; uint32_t i; }; } PK;
#pragma pack(pop)
static inline PK make_pk(uint8_t last) { PK p; memset(&p, 0, sizeof p); p.k = 1; p.c[4] = last; return p; }
