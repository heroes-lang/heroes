# Panel 186, the ffi-pragmatist's report

Written 2026-10-02 as I go, in my copy
`<scratchpad>/186-ffi-pragmatist/` (`git archive 779139d0 | tar -x`), with a
compiler built in it from the seed (`clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes`, `heroes 0.2.0`), `HEROES_RUNTIME` set to the
copy's `runtime/`. Clangs: `/usr/bin/clang` (Apple clang 21.0.0,
clang-2100.3.34.2) and `/opt/homebrew/opt/llvm@22/bin/clang` (Homebrew clang
22.1.8). Nothing timed, at most three processes, no paid run. Every case
file named below is under `<scratchpad>/186-ffi-pragmatist/work/`; the ones a
later reader needs are quoted whole, since the scratchpad goes with the
session.

Status: complete, waiting on § 6's readings (see § 8).

## 1. Today's behaviour, reproduced on my compiler

`work/br.sh` builds a file, records the exit code and stderr, and runs it.
On `779139d0`'s seed compiler:

| case | build | run |
|---|---|---|
| `u17_anon_constructed` | exit 0, 3 clang warnings (excess elements, overrides) | `1056964608`, exit 0 |
| `u18_anon_compared` | exit 0, excess-elements warning | `true`, exit 0 |
| `u07_anon_omits_x` | exit 0, silent | `12` |
| `lane-literals/u16_anon_omits_kind` | exit 0, silent | `15` |
| `u19_struct_omits_middle` | exit 1, *`S3` does not name `c`* | |
| `read.hero` (defect 150) | exit 0, excess-elements warning twice | `7` |
| `critic/one_arm` (`SB` by one arm, `==`) | exit 0, silent | `true` (the two values differ) |
| `critic/one_arm_union` | exit 1, `ffi_union_field` | |
| `critic/u08_mine` | exit 0, silent | `7 12` |
| `critic/bf` (defect 156) | exit 2, *invalid application of 'sizeof' to bit-field* | |
| `critic/arm` | exit 1, `ffi_unknown_tag` | |
| `critic/omit` | exit 1, `missing_fields` | |

All as the shared brief says.

## 2. The real headers

**What I searched, and with which command.** One translation unit per header
set, dumped by clang itself and read by a script of mine (my instrument,
not the compiler's): `clang -std=gnu11 -fsyntax-only -I/opt/homebrew/include
-I/opt/homebrew/opt/sqlite/include -Xclang -ast-dump=json <unit>.c`, then
`work/sweep/sweep.py` walks every complete `RecordDecl` a binding can name
(a tag or a typedef) and counts its anonymous members (`FieldDecl` with no
name or `isImplicit`), named union members, `"isBitfield": true` and
flexible array members. The units: `sdk.c` (45 SDK headers: stdio, stdlib,
signal, time, dirent, termios, pthread, netdb, sys/socket, sys/stat,
sys/event, sys/ucontext, net/if, netinet/in, netinet/tcp, arpa/inet,
ifaddrs, poll, glob, regex, spawn, mach/mach and the rest listed in the
file), `sdl.c` (`SDL3/SDL.h`), `ray.c` (`raylib.h`, `raymath.h`, `rlgl.h`),
`sq.c` (`sqlite3.h`, `zlib.h`, `curl/curl.h`).

| unit | nameable records | unions | holding an anonymous union | anonymous struct | a named union member | a bit-field |
|---|---|---|---|---|---|---|
| SDK | 890 | 27 | 3 | 0 | 10 | 15 |
| SDL3 | 142 | 3 | 0 | 0 | 1 (`SDL_GamepadBinding`, two) | 0 |
| raylib, raymath, rlgl | 43 | 0 | 0 | 0 | 0 | 0 |
| sqlite3, zlib, curl | 77 | 1 | 0 | 0 | 1 (`CURLMsg`) | 1 (`curl_hstsentry`) |

The anonymous unions in a struct are **`glob_t`** (`glob.h:51`, libc's
`glob(3)`: `union { gl_errfunc; gl_errblk; }`, the second a block pointer
under `__BLOCKS__`), `struct mach_port_options` and `struct
processor_basic_info` (mach). The bit-fields are `struct tcphdr`, `struct
tcp_connection_info`, eleven mach message descriptors, and **`struct
curl_hstsentry`** in `curl/curl.h:998`, the header of §4.19's rung 4. Two
shapes the brief does not list, and both are libc's commonest bindings:
**a union member reached through a header MACRO**. `struct sigaction`'s
`sa_handler` is `#define sa_handler __sigaction_u.__sa_handler`
(`sys/signal.h:296`), and `struct in6_addr`'s `s6_addr` is `#define s6_addr
__u6_addr.__u6_addr8` (`netinet6/in6.h:161`). raylib and SQLite hold no
union anywhere, as panel 077's ffi-pragmatist measured.

**Today, on `779139d0`'s compiler** (`work/real/`, `work/sdl/`; run directly,
see the note below):

| binding | what it does | build | run |
|---|---|---|---|
| `glob_one.hero`: `record glob_t`, all 11 members with ONE arm (`gl_errfunc`), `glob()` over a directory, `globfree` | an anonymous union bound by one arm | exit 0, silent | `0`, `2 2`, correct |
| `glob_both.hero`: both arms | | exit 1, `ffi_field_type` (`gl_errblk` is a block pointer, not `ptr`) | |
| `sig_one.hero`: `record SigAction tag sigaction` naming `sa_handler`, `sa_mask`, `sa_flags`; installs `SA_RESTART`, reads it back | a named union through a macro | exit 0, silent | `0 0 true`, correct |
| `sig_both2.hero`: also `sa_sigaction` (the other arm, same bytes) | defect 151's class on real libc | exit 0, **four clang warnings** (excess elements twice, overrides twice) | `0`, `0 2 false`, correct by luck: both arms written `nullptr` |
| `in6.hero`: `record In6 tag in6_addr` naming `s6_addr: u8[16]`, three `inet_pton`, then `a == b`, `a == c` | **`==` on a struct whose only member is a union** | exit 0, silent | `111`, `true false 1`, correct |
| `hsts.hero`: `record Hsts tag curl_hstsentry`, four fields, one a 1-bit field; builds one and reads it | defect 156 on rung 4's header | **exit 2**, *invalid application of 'sizeof' to bit-field* and *address of bit-field requested* | |
| `tcp.hero`: `record Tcp tag tcphdr partial` naming the two ports | bit-fields left out by `partial` | exit 0 | `3` |

**A false alarm I chased, so nobody repeats it**: under my runner's `$(...)`,
zsh leaves SIGUSR1 with `sa_flags` 2, and plain C (`work/real/sigc.c`) reads
the same `2` there and `0` when run directly. The binding is right; the
numbers above are from direct runs.

**`hsts.hero`'s exit 2 is two lines of C**, not a binding problem. In the
generated unit (`build/tu-5d4b494602a8382e/hsts.c`) the construction
(`.includeSubDomains = t3`), the read (`t24.includeSubDomains`) and `==`
(`a->includeSubDomains == b->includeSubDomains`) are legal C; only the field
assertion's `sizeof(((struct curl_hstsentry *)0)->includeSubDomains)` and the
hash's `&v->includeSubDomains` are not, and the hash is emitted although
nothing hashes.

## 3. Panel 073's three readings, against SDL3's real `SDL_Event`

`SDL_Event` is `typedef union SDL_Event { Uint32 type; ... Uint8
padding[128]; } SDL_Event;` (`SDL_events.h:1017`). SDL3 hands events in and
out only through a pointer (`SDL_PushEvent`, `SDL_PollEvent`), so a cell
must be built before it can be filled. Each program calls
`SDL_Init(SDL_INIT_EVENTS)`, pushes a user event, polls it, and opens no
window.

| reading | program | build | run |
|---|---|---|---|
| **one-member binding** | `r3_one.hero`: `record SDL_Event` naming `type: u32`; `SDL_Event(type: ...)` pushed, `SDL_Event(type: 0)` polled into | exit 0, **silent**, no shim | `true true true true` |
| **reading a union's members** | `r1_read.hero`: `record SDL_Event` naming `type`, `common: SDL_CommonEvent`, `user: SDL_UserEvent`; value from `hero_poll()` | exit 0, **excess elements in union initializer, twice** (defect 150 on SDL3) | `true true true true 42`, correct |
| the same, `partial` | `r1_readpartial.hero` | exit 0, silent | the same |
| **one record per arm by `tag`** | `r2_tag.hero`: `record UserEv tag SDL_Event` naming `user` | **exit 1, `ffi_tag_is_a_union`**: *a group's `record` is the header's STRUCT (§4.19). A union has no `record` spelling in this language* | |

Two facts the brief's premise does not hold:

- **The reading "one record per arm by `tag`" is refused today**, on
  SDL_Event and on a minimal `union utag` (`work/h/tu1.hero`, the same
  code). Panel 073's resolution item 7 named it the escape route; panel 077
  item 4 asked that the emitter *spell `union T` where the header says
  union*; what landed is `selfhost/emit/ffi_tag.hero:199-218`, which maps
  clang's tag mismatch to a refusal. `grep -rln ffi_tag_is_a_union tests/`
  finds no golden. So of the three readings, two run.
- **The multi-member reading needs a shim on SDL3**: a record naming two
  arms cannot be built (`missing_fields` naming one of them, measured on
  `work/h/p1.hero` with `partial`; `ffi_union_field` naming both), no SDL3
  function returns an `SDL_Event` by value, and a second record over the
  same typedef is `ffi_unknown_tag` or, by its tag, `ffi_tag_is_a_union`.
  `r1_read` needed `work/sdl/sdl_ev2.h`, two `static inline` lines
  (`hero_poll`, `hero_push_user`). So **on `779139d0` no Heroes program can
  read an SDL3 key or mouse event without hand-written C**: the one
  shim-free binding names `type` alone.

**What (1h) would emit, compiled by hand** (`work/sdl/h1.c`): `SDL_Event got
= (SDL_Event){.type = 0};`, `SDL_PollEvent(&got)`, then `got.user.code` and
`got.key.type`, with the completeness probe `SDL_Event v = {.type = 0};`
under `#pragma clang diagnostic error` for both `-Winitializer-overrides`
and `-Wmissing-field-initializers`. Both clangs, the project's flags: compile
exit 0, **zero warnings**, `polled=1 type_is_user=1 code=42
key_arm_type=32768`, exit 0. No shim.

(Correction to § 2, same day: `curl_hstsentry` is the SDK's
`usr/include/curl/curl.h:998`; there is no Homebrew `curl/curl.h` on this
Mac. `sys/signal.h:296-297` and `netinet6/in6.h:161` verified by `grep -n`.)

## 4. The C each route implies, compiled by hand on both clangs

`work/c/shapes.h` holds every shape of the brief (`SA`, `SB`, `S3`, `W`,
`BF`, `S2U`, `DEEP`, `PADU`, the padded union `PU {i; f; char pad[128]}`,
`Z` with two GNU zero-sized members, `FAM`, an array member `AR`, an
anonymous struct `AS`) and includes the real `signal.h`, `glob.h` and
`netinet/in.h`. `work/c/gen.py` writes each route's C for 34 cases (a shape
and a declared field list: `SA` complete, missing `x`, missing `kind`, `i`
alone, `f` alone; `SB` by `b`, by `q`, by both; `S3` `{a,c}` and complete;
`W` both, `i`, `n`; `S2U`, `DEEP`, `PADU`, `PU` variants; `Z`; `FAM` `{n}`;
`AR`; `AS`; real `sigaction` by one and both macro arms; real `in6_addr`;
real `glob_t` by one and both arms; `BF` all and `kind` alone). `run.py`
compiles one file with `-std=gnu11 -fsyntax-only -ferror-limit=0` and maps
each diagnostic to its case; `both.sh` runs Apple clang 21 and Homebrew
clang 22.1.8 and diffs. **Every file below gave identical verdicts and exit
codes on the two clangs.**

| case | (1a) ranges | (1b) designated, overrides an error | (1f) classify OR sum | today's positional probe | adjacency (§ 4.1) |
|---|---|---|---|---|---|
| `SA` complete | fires `i f` | error | fires | excess warning | fires `between i f` |
| `SA` no `x` (u07) | fires | error | **passes** | silent | fires `between i f`, `after f` |
| `SA` no `kind` (u16) | fires | error | **passes** | silent | fires `starts-at i` |
| `SA` `i` alone, `f` alone (u08) | silent | silent | silent | silent | silent |
| `SB` by `b` (the critic's `==`) | **silent** | **silent** | silent | silent | **fires `between kind b`** |
| `SB` by `q` | silent | silent | silent | silent | silent |
| `SB` both | fires | error | **passes** | excess | fires |
| `S3 {a,c}` (u19) | silent | silent | silent | *missing field 'c'* (the wrong name) | fires `between a c` |
| `W` both (`read.hero`) | fires | error | fires | excess | fires |
| `W` one member | silent | silent | fires | silent | silent |
| `S2U {kind,i,b}` | silent | silent | silent | silent | **silent** (blind) |
| `DEEP` with `i` and `f` | fires | error | fires | excess | fires |
| `PADU {k,a,b}` | fires | error | **passes** | excess | fires |
| `PU {i}` (SDL_Event's shape) | silent | silent | fires | silent | fires `after i` |
| `Z` (GNU zero-sized) | **silent** | silent | silent | silent | silent |
| `FAM {n}`, `AR`, `AS` | silent | silent | silent | silent | silent |
| `sigaction` both macro arms | **fires** (`offsetof` sees through the macro) | error | fires | excess | fires |
| `in6_addr {s6_addr}`, `sigaction` one arm, `glob_t` one arm | silent | silent | silent | silent | silent |
| `glob_t` both arms | fires | error | **passes** (its padding absorbs the overlap) | excess | fires |
| `BF` all three | **clang's text**: *cannot compute offset of bit-field*, *invalid application of 'sizeof' to bit-field* | **silent** (a bit-field takes a designator) | clang's text | silent | clang's text |
| `BF {kind}` | | silent | | *missing field 'flag'* | fires `after kind` |

**(1f) is blind on a real libc header**: `glob_t` naming both arms passes
`sizeof(glob_t) >= Σ`, because two `int` members padded to 8 absorb the
eight bytes the union arms share.

**Q2, designated omission** (`q2des.c`, both `-Wmissing-field-initializers`
and `-Wmissing-designated-field-initializers` as errors): silent on both
clangs for `S3 {.a, .c}`, `SA {.kind, .i}`, `BF {.kind}`, exit 0. The
critic's measurement stands on 21 and 22. **Looked further:** the same unit
as C++20 (`work/c/cxx.cc`, `-x c++ -std=c++20`) names the field on Homebrew
22 (*missing field 'b'*, *'x'*, *'flag'*) and is **silent on Apple 21**, and
C++ lays `struct Empty {}` out at one byte, so `Z` is not C's 4 bytes there
(`static_assert(sizeof(Z) == 4)` fails on both): a C++ probe judges a
different type than the program uses, and two clangs on this Mac disagree.
I found no C form that reports a designated omission.

### 4.1 A route nobody listed: the adjacency form

For the declared fields in order: the first at offset 0; each next one at
the previous one's end rounded up to its own alignment; the last one's end,
rounded to the struct's alignment, at `sizeof(T)`
(`UP(offsetof(T,p) + sizeof(((T *)0)->p), _Alignof(__typeof__(((T *)0)->n))) == offsetof(T,n)`).
Linear (n + 1 assertions), C arithmetic and no warning, so clang judges it
and no clang version's diagnostic semantics enter. It is the only C form I
found that **names where a field is missing** (`between a c`, `after f`,
`starts-at i`, never blaming a declared field) and the only C form that
**sees `SB` by one arm** (the union's alignment, from `q`, leaves a gap
before `b`). Its blind spot: a short arm whose union's remaining bytes look
like alignment padding (`S2U {kind,i,b}`). Its costs, measured in § 5: it
must not run over a union record, nor over a type that is incomplete, and it
makes declaration order the header's order.

### 4.2 (1e): what clang's two dumps show

`work/c/lay.c` (one variable per shape) under `-Xclang -fdump-record-layouts`:

```
         0 | SA
         0 |   int32_t kind
         4 |   union SA::(anonymous at ./shapes.h:6:32)
         4 |     int32_t i
         4 |     float f
         8 |   int32_t x
         0 | BF
         0 |   int32_t kind
     4:0-0 |   uint32_t flag
    4:1-31 |   uint32_t rest
         0 | FAM
         0 |   int32_t n
         4 |   int32_t[] data
           | [sizeof=4, align=4]
         0 | struct sigaction
         0 |   union __sigaction_u __sigaction_u
         0 |     void (*)(int) __sa_handler
         0 |     void (*)(int, struct __siginfo *, void *) __sa_sigaction
```

An anonymous struct prints as `struct AS::(anonymous at ...)`, members
indented. Three costs, each run:

- **The text is not stable across the two clangs of this Mac.** Normalising
  addresses and SDK paths, the only diff is the nested anonymous union in
  `DEEP`: Apple 21 prints `union DEEP::(anonymous at ./shapes.h:12:41)`,
  Homebrew 22 prints `union DEEP::(anonymous struct)::(anonymous at
  ./shapes.h:12:41)` (and `union (unnamed struct)::(unnamed struct)::...`
  for the record's own line). Reproduced on the portable `plat_lay.c`.
- **It cannot see a macro-named member.** A binding of `sigaction` declares
  `sa_handler`; the layout holds `__sa_handler` and `__sigaction_u`. The same
  for `in6_addr`'s `s6_addr` (`__u6_addr8`). A reader that matches declared
  names against the layout either refuses `sig_one.hero` and `in6.hero`,
  which run today, or falls back silently.
- **The two clangs read different SDKs**: Apple 21 the Xcode SDK, Homebrew 22
  `/Library/Developer/CommandLineTools/SDKs/MacOSX26.sdk`. Not a route cost,
  a measurement cost: a seat comparing the two compares two headers too.

Its size is small: under `-fsyntax-only`, a unit holding `SDL3/SDL.h` and
three variables dumps 62 records in 32 737 bytes (`work/c/sdlay.c`).

**The JSON dump `build` already runs** reaches both missing cases. A
never-called function per declared field, `static void hero_lay_SA_i(void) {
(void)((SA *)0)->i; }`, under `-Xclang -ast-dump=json -Xclang
-ast-dump-filter=hero_lay_` (`work/c/acc.c`), gives the access's
`MemberExpr` chain, which the preprocessor and Sema have already resolved:

| declared | the chain clang resolved (both clangs) |
|---|---|
| `SA.i` | `i` ← anonymous member of type `union SA::(anonymous ...)` |
| `SB.b` | `b` ← anonymous `union` |
| `DEEP.i` | `i` ← anonymous `union` ← anonymous `struct` |
| `sigaction.sa_handler` | `__sa_handler` ← `__sigaction_u` (`union __sigaction_u`) |
| `in6_addr.s6_addr` | `__u6_addr8` ← `__u6_addr` (a union) |
| `BF.flag`, `S3.c` | the member alone |

The chain's names and shape are identical on 21 and 22; **its type text is
not** (`union (unnamed union at /Applications/...` on 21 against `union
(unnamed at /Library/...` on 22 for `in6_addr`, and the `DEEP` difference
again). And a filter on the record's own name (`-ast-dump-filter=SA`,
`work/c/sdlay.c`) dumps the typedef'd anonymous struct's whole member tree
(`RecordDecl ... "tagUsed": "union"`, the implicit `FieldDecl`, both
`IndirectFieldDecl`s), identical on both clangs, but the filter is a
substring match: 35 SDL enum constants rode along with it. So the reader
must key on `kind`, `name`, `isImplicit` and `tagUsed`, never on
`qualType`.

### 4.3 Bit-fields: the C a binding needs exists, and it is C's verdict

`work/c/bf2.c`: `typeof` of a bit-field is a hard error in C on both clangs,
so no form can spell the member's type. What works, `work/c/bf3.c` and
`bf4.c`, both clangs, the project's flags (`selfhost/cli/flags.hero:93-108`):

- the field assertion's width without `sizeof` of the member: `_Generic` on
  the member selecting `sizeof(<standard type>)`, clang's own sizes and no
  number this compiler carries; passes on `curl_hstsentry.includeSubDomains`
  and `BF.flag`, no hard error;
- **C's verdict that a member is a bit-field**: `_Static_assert(((struct
  curl_hstsentry){.includeSubDomains = UINT32_MAX}).includeSubDomains ==
  UINT32_MAX, "heroes-ffi-bitfield Hsts includeSubDomains")` fails with our
  marker (*expression evaluates to '1 == 4294967295'*) and passes on every
  full-width member, under a local `ignored "-Wbitfield-constant-conversion"`;
- a construction fit check at run time, `((T){.f = v}).f == v`: `fits(1)=1
  fits(5)=0`, C having stored `1` for `5`, the silent narrowing it exists for;
- a hash that takes no member's address: `hash(&(uint32_t){v->f})`.
  Compile exit 0, zero warnings on both clangs.

So defect 156 can become either a binding (read, build with the fit check,
compare, hash) or an exit-1 diagnostic at the field's line from our own
marker; exit 2 is neither.

## 5. The census, in my copy

`work/pop.txt`: 447 of 1672 tracked `.hero` files hold `^extern ` (`git
ls-tree -r --name-only 779139d0`), each built `--emit-c` from the copy's
root, three at a time, exit code, stdout and stderr per file
(`work/census/<compiler>/`). **The seed compiler: 229 exit 0, 217 exit 1, 1
exit 2** (`probes/critic/bf.hero`); without this sitting's 30 probes, 207,
210 and 0, the critic's baseline exactly.

**(A) Who compares, hashes or keys a map by an `extern` record.** A copy of
`selfhost/` (`work/v_op/`) whose `union_assertions` also writes `/*
heroes-census-operated <name> */` for every record in `used`'s `operated`
set; its exit codes equal the seed's on all 447. **13 files** carry the
marker: 7 over handles (`G`, `H`, `Ob`, `Chunk`, `Thing`, `Mem`, `Outer`
twice), 3 this sitting's probes, 1 panel 176's `getter_wrong_repaired`
(a handle), and **exactly one over a record with fields**:
`tests/golden/run/fixedbugs-a-group-record-in-every-container.hero`,
raylib's `Color`, `a == b` and `{Color: i64}`. Panel 077's historian
predicted zero; the tree holds one, and my `in6.hero` is a second outside it.

**(B) The adjacency form over the tracked tree.** A second copy
(`work/v_adj/`) emits § 4.1's assertions inside `completeness_probes` for
every non-`partial` record with fields. **17 files move, every one this
sitting's probe**, from exit 0 to exit 2: the union records `read.hero`,
`u01`, `u02`, `u04`, `u05`, `u09`, `u14` (correct read-only programs); `SA`
overlapping or omitting (`u06`, `u07` twice, `u16`, `u17` twice, `u18`
twice); and `one_arm`. **None of the 417 earlier files moves**: every
example, every `run` golden, raylib's 35 structs wherever bound, `stat`,
`dirent`, `addrinfo`, `utsname`, so no tracked record declares its fields
out of the header's order and none hides a gap. Run by hand on my real
programs: `r3_one.hero` (SDL_Event by `type`) **fires** `end type`;
`in6.hero`, `glob_one.hero`, `sig_one.hero` and u08 stay silent.

**And one refusal that changed**: `tests/golden/unsupported/fixedbugs-145-a-result-spells-the-typedef.hero`
keeps exit 1, but its second diagnostic, `ffi_return_type` at line 19,
**disappears** and the first one's note is reworded: `offsetof` and
`sizeof` over `struct anon_s`, a type C never completed, add hard errors,
and clang's recovery after them drops the result-type assertion's failure.
The `unsupported` golden would go red. It binds every route that adds
`offsetof` or `sizeof` of a member ((1a), (1f), adjacency): they run only
over a record whose type is complete.

Three stderr diffs more, all this sitting's probes: `bf.hero` (more clang
text at exit 2), `u12` and `u13` (the excess warning gone, the refusal
the same).

**Correction to (B), same session, after counting it.** The list *every
example, every `run` golden, raylib's 35 structs wherever bound, `stat`,
`dirent`, `addrinfo`, `utsname`* was written before it was counted, and it is
wrong. Counted from the instrument's own output (`work/census/adj/*.out`,
the `heroes-census-adj <name> start` lines): **24 earlier files that build
today carry the assertions, over 1038 records and 2158 assertions**; 1000 of
the records are one file's nest (`fixedbugs-140-extern-records-a-thousand-deep-build`).
The other 38 are 24 names: raylib's `Color`, `Vector2`, `Rectangle`,
`Matrix` (16 fields), `Texture`, `Font`, `Camera2D`, `AutomationEvent`,
`VrDeviceInfo` and `VrStereoConfig` (5 files include `raylib.h`), and the
test headers' `Cell`, `Four`, `Handle`, `Holder`, `Inner`, `Kinds`, `Nums`,
`Outer`, `Pair`, `Quad`, `Row`, `Sl`, `Slot`, `Tag`. No `stat`, `dirent`,
`addrinfo` or `utsname` binding received one (a `partial` record gets no
probe). What stands: none of them fires.

## 6. Requests to the coordinator

Every verdict above was read on Apple clang 21.0.0 and Homebrew clang
22.1.8 only. The routes rest on six things whose answer may differ on the
clangs this project is judged by: `-Winitializer-overrides` as an error
((1b)); whether ANY clang reports a designated omission in C (Q2); GNU
zero-sized members' layout (the Windows target especially); the bit-field
forms; the record-layout dump's text ((1e)); the JSON member-access chain
((1e)). Three files, quoted whole below and also in
`<scratchpad>/186-ffi-pragmatist/work/plat/` while the session lives.

**Where, and which files.**

| clang | where | files |
|---|---|---|
| Ubuntu clang 18.1.3 (the CI's Linux legs, the floor) | an `ubuntu:24.04` container with `apt install clang`, or `silkeh/clang:18` saying its exact version | `plat.c`, `plat_posix.c`, `plat_lay.c` |
| clang 20.1.8 (the CI's Windows leg) | a container with LLVM 20.1.8, or a CI run; **owed** if neither | `plat.c`, `plat_lay.c` |
| clang 23.1.1 (the Windows box) | `ssh win` | `plat.c`, `plat_lay.c` (no `plat_posix.c`: no `sigaction` there) |

**The commands**, in the directory holding the files:

```sh
clang --version | head -1
clang -std=gnu11 -fsyntax-only -ferror-limit=0 -fno-caret-diagnostics -fno-color-diagnostics plat.c 2>&1 | grep -E 'error|warning'
clang -std=gnu11 -fsyntax-only -ferror-limit=0 -fno-caret-diagnostics -fno-color-diagnostics -Wno-unused-function plat_posix.c 2>&1 | grep -E 'error|warning'
clang -std=gnu11 -fsyntax-only -Xclang -fdump-record-layouts plat_lay.c 2>&1 | grep -E '^ +[0-9:-]+ \|'
clang -std=gnu11 -fsyntax-only -Wno-unused-function -Xclang -ast-dump=json -Xclang -ast-dump-filter=hero_lay_ plat_lay.c > acc.json; grep -cE '"kind": "MemberExpr"' acc.json; grep -E '"isImplicit"|"name": "(i|hero_lay_[A-Za-z_]+)"' acc.json
```

**What Apple 21 and Homebrew 22.1.8 both printed** (identical; the second
command's lines mapped to their P-number by line): errors at P01, P03, P05,
P06, P07, P11, P12, P13, P14, P18 (each `static assertion failed ... P<nn>`),
P21 and P23 (*initializer overrides prior initialization of this subobject
[-Werror,-Winitializer-overrides]*), P28 (*missing field 'c' initializer*);
warnings at P29 (*excess elements in union initializer*) and P30 (*excess
elements in struct initializer*); **nothing at P25, P26, P27** (the
designated omission). `plat_posix.c`: X01 fails, X04 is the overrides error,
nothing else. `plat_lay.c`: the layout lines differ between the two only on
the nested union's own line (`union (anonymous at plat_lay.c:5:41)` against
`union (unnamed struct)::(unnamed struct)::(anonymous at plat_lay.c:5:41)`);
the JSON holds 5 `MemberExpr`s on both. **What I most need read**: P25 to
P27 on 18.1.3 (the floor), P08 and P16 on 23.1.1 and 20.1.8 (a zero-sized
member on the Windows target), and the layout text on all three.

`plat.c`:

```c
/* Panel 186, ffi-pragmatist: the verdicts each route rests on, one unit, no system
   struct. Run: clang -std=gnu11 -fsyntax-only -ferror-limit=0 -fno-caret-diagnostics
   -fno-color-diagnostics plat.c. Every expected verdict carries a P-number. */
#include <stdint.h>
#include <stddef.h>
typedef struct { int32_t kind; union { int32_t i; float f; }; int32_t x; } SA;
typedef struct { int32_t kind; union { int8_t b; int64_t q; }; } SB;
typedef struct { int32_t a; int32_t b; int32_t c; } S3;
typedef union { int32_t i; uint32_t n; } W;
typedef struct { int32_t kind; uint32_t flag : 1; uint32_t rest : 31; } BF;
typedef struct { int32_t kind; union { int32_t i; float f; }; union { int16_t s; uint8_t b; }; } S2U;
typedef struct { int64_t k; union { int8_t a; int8_t b; }; } PADU;
struct Empty {};
typedef struct { struct Empty e1; struct Empty e2; int32_t x; } Z;
#define SZ(T, f) sizeof(((T *)0)->f)
#define DISJOINT(T, a, b) (offsetof(T, a) + SZ(T, a) <= offsetof(T, b) || offsetof(T, b) + SZ(T, b) <= offsetof(T, a))
#define UP(n, al) (((n) + (al) - 1) / (al) * (al))
#define AL(T, f) _Alignof(__typeof__(((T *)0)->f))
#define ADJ(T, p, n) (UP(offsetof(T, p) + SZ(T, p), AL(T, n)) == offsetof(T, n))
#define END(T, l) (UP(offsetof(T, l) + SZ(T, l), _Alignof(T)) == sizeof(T))
/* classify, the premise of today's detector */
_Static_assert(__builtin_classify_type(*(W *)0) != 13, "P01 W classifies as a union: expected to FAIL");
_Static_assert(__builtin_classify_type(*(SA *)0) != 13, "P02 SA classifies as a struct: expected to PASS");
/* (1a) range assertions */
_Static_assert(DISJOINT(SA, i, f), "P03 1a SA i f: expected to FAIL");
_Static_assert(DISJOINT(SA, kind, i), "P04 1a SA kind i: expected to PASS");
_Static_assert(DISJOINT(SB, b, q), "P05 1a SB b q: expected to FAIL");
_Static_assert(DISJOINT(W, i, n), "P06 1a W i n: expected to FAIL");
_Static_assert(DISJOINT(PADU, a, b), "P07 1a PADU a b: expected to FAIL");
_Static_assert(DISJOINT(Z, e1, e2) && DISJOINT(Z, e2, x), "P08 1a Z zero-sized: expected to PASS");
/* (1f) classify OR sum */
_Static_assert(sizeof(SA) >= SZ(SA, kind) + SZ(SA, i) + SZ(SA, f), "P09 1f SA kind i f: expected to PASS (blind)");
_Static_assert(sizeof(PADU) >= SZ(PADU, k) + SZ(PADU, a) + SZ(PADU, b), "P10 1f PADU: expected to PASS (blind)");
/* the adjacency form */
_Static_assert(ADJ(SA, i, f), "P11 adj SA between i f: expected to FAIL");
_Static_assert(END(SA, f), "P12 adj SA after f (x left out): expected to FAIL");
_Static_assert(ADJ(S3, a, c), "P13 adj S3 between a c (b left out): expected to FAIL");
_Static_assert(ADJ(SB, kind, b), "P14 adj SB between kind b (one arm): expected to FAIL");
_Static_assert(ADJ(S2U, i, b) && END(S2U, b), "P15 adj S2U kind i b: expected to PASS (blind)");
_Static_assert(ADJ(Z, e1, e2) && ADJ(Z, e2, x) && END(Z, x), "P16 adj Z: expected to PASS");
/* the bit-field forms: a width line with no sizeof of the member, and C's own verdict that a member is narrower */
#define WIDTH(e) _Generic((e), _Bool: sizeof(_Bool), char: sizeof(char), signed char: sizeof(signed char), unsigned char: sizeof(unsigned char), short: sizeof(short), unsigned short: sizeof(unsigned short), int: sizeof(int), unsigned int: sizeof(unsigned int), long: sizeof(long), unsigned long: sizeof(unsigned long), long long: sizeof(long long), unsigned long long: sizeof(unsigned long long), float: sizeof(float), double: sizeof(double), default: 0)
_Static_assert(WIDTH(((BF *)0)->flag) == sizeof(uint32_t), "P17 width of a bit-field by _Generic: expected to PASS");
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wbitfield-constant-conversion"
_Static_assert(((BF){.flag = UINT32_MAX}).flag == UINT32_MAX, "P18 BF flag is full width: expected to FAIL");
_Static_assert(((BF){.kind = INT32_MAX}).kind == INT32_MAX, "P19 BF kind is full width: expected to PASS");
#pragma clang diagnostic pop
static uint64_t take(const uint32_t *p) { return *p; }
__attribute__((unused)) static uint64_t P20_hash_by_value(const BF *v) { return take(&(uint32_t){v->flag}); }
/* (1b): designated probes, overrides armed as an error */
#pragma clang diagnostic push
#pragma clang diagnostic error "-Winitializer-overrides"
__attribute__((unused)) static void P21_1b_SA_all_expected_ERROR(void) { SA v = {.kind = 0, .i = 0, .f = 0, .x = 0}; (void)v; }
__attribute__((unused)) static void P22_1b_SA_one_arm_expected_silent(void) { SA v = {.kind = 0, .i = 0, .x = 0}; (void)v; }
__attribute__((unused)) static void P23_1b_W_both_expected_ERROR(void) { W v = {.i = 0, .n = 0}; (void)v; }
__attribute__((unused)) static void P24_1b_BF_expected_silent(void) { BF v = {.kind = 0, .flag = 0, .rest = 0}; (void)v; }
#pragma clang diagnostic pop
/* Q2: does THIS clang report a field a DESIGNATED initializer leaves out, in C? silent on Apple 21 and Homebrew 22 */
#pragma clang diagnostic push
#pragma clang diagnostic error "-Wmissing-field-initializers"
#pragma clang diagnostic error "-Wmissing-designated-field-initializers"
__attribute__((unused)) static void P25_designated_S3_without_b(void) { S3 v = {.a = 0, .c = 0}; (void)v; }
__attribute__((unused)) static void P26_designated_SA_without_x(void) { SA v = {.kind = 0, .i = 0}; (void)v; }
__attribute__((unused)) static void P27_designated_BF_without_flag(void) { BF v = {.kind = 0}; (void)v; }
/* today's positional probe, for its texts */
#pragma clang diagnostic ignored "-Wmissing-braces"
__attribute__((unused)) static void P28_positional_S3_a_c_names_c(void) { S3 v = {0, 0}; (void)v; }
__attribute__((unused)) static void P29_positional_W_two_excess(void) { W v = {0, 0}; (void)v; }
__attribute__((unused)) static void P30_positional_SA_four_excess(void) { SA v = {0, 0, 0, 0}; (void)v; }
__attribute__((unused)) static void P31_positional_SA_three_silent(void) { SA v = {0, 0, 0}; (void)v; }
#pragma clang diagnostic pop
```

`plat_posix.c`:

```c
/* Panel 186, ffi-pragmatist: union members a binding reaches through header MACROS.
   Linux and macOS only. Run: clang -std=gnu11 -fsyntax-only -ferror-limit=0
   -fno-caret-diagnostics -fno-color-diagnostics -Wno-unused-function plat_posix.c */
#include <signal.h>
#include <stddef.h>
#include <netinet/in.h>
#define SZ(T, f) sizeof(((T *)0)->f)
#define DISJOINT(T, a, b) (offsetof(T, a) + SZ(T, a) <= offsetof(T, b) || offsetof(T, b) + SZ(T, b) <= offsetof(T, a))
_Static_assert(DISJOINT(struct sigaction, sa_handler, sa_sigaction), "X01 1a sigaction arms through macros: expected to FAIL");
_Static_assert(DISJOINT(struct sigaction, sa_handler, sa_flags), "X02 1a sigaction handler flags: expected to PASS");
_Static_assert(SZ(struct in6_addr, s6_addr) == sizeof(struct in6_addr), "X03 s6_addr covers in6_addr: expected to PASS");
#pragma clang diagnostic push
#pragma clang diagnostic error "-Winitializer-overrides"
static void X04_both_arms_expected_ERROR(void) { struct sigaction v = {.sa_handler = 0, .sa_sigaction = 0}; (void)v; }
static void X05_one_arm_expected_silent(void) { struct sigaction v = {.sa_handler = 0, .sa_flags = 0}; (void)v; }
#pragma clang diagnostic pop
```

`plat_lay.c`:

```c
/* Run: clang -std=gnu11 -fsyntax-only -Xclang -fdump-record-layouts plat_lay.c
   and: clang -std=gnu11 -fsyntax-only -Wno-unused-function -Xclang -ast-dump=json -Xclang -ast-dump-filter=hero_lay_ plat_lay.c */
#include <stdint.h>
typedef struct { int32_t kind; union { int32_t i; float f; }; int32_t x; } SA;
typedef struct { int32_t kind; struct { union { int32_t i; float f; }; int32_t y; }; int32_t x; } DEEP;
typedef struct { int32_t kind; uint32_t flag : 1; uint32_t rest : 31; } BF;
typedef struct { int32_t n; int32_t data[]; } FAM;
SA hero_l_SA; DEEP hero_l_DEEP; BF hero_l_BF; FAM hero_l_FAM;
static void hero_lay_SA_i(void) { (void)((SA *)0)->i; }
static void hero_lay_DEEP_i(void) { (void)((DEEP *)0)->i; }
```

## 7. Verdicts

Provisional on § 6's readings where the line says so. I stand on design.md
§1.11 (*FFI ergonomics rank alongside comprehension*; *a thin C shim ... for
the hard cases only*), §1.12 (*any C library must be bindable*) and §4.19
(a wrong binding is a compile error; rung 4 is curl, rung 5 raylib).

| route | verdict | what decides it, all run |
|---|---|---|
| **(1a)** ranges | **approve**, as the detector for two declared fields sharing bytes, at `used` records only | fires on all 13 overlapping cases, through `sigaction`'s macros, on real `glob_t`; no false positive on GNU zero-sized members; C arithmetic, no warning's meaning. Owes a bit-field filter (clang's hard text today; § 4.3's marker form filters it in C) and must not run over an incomplete type (`fixedbugs-145`). Blind to `SB` by one arm |
| **(1b)** designated, overrides an error | **approve**, at `used` records only; provisional on P21 to P24 on 18.1.3 and 20.1.8 | the same 12 verdicts as (1a), and silent on a bit-field record where (1a) hard-errors; over every record it refuses `read.hero` and five more correct read-only programs (§ 5's union rows, and the critic's § 9) |
| **(1c)** refuse any record over a struct holding an anonymous union | **veto** | refuses `glob_one.hero`, libc's `glob(3)` on the real `glob_t`, which builds silent and runs correctly today, and u08 |
| **(1d)** nested group in the declaration | **object** | makes union-ness visible to the checker, but a flat declaration of a field the header keeps in a union still needs a detector in `build`, so it adds a form without removing a mechanism; every binding of a union pays a line per group |
| **(1e)** via `-fdump-record-layouts` | **object** | its text changed between Apple 21 and Homebrew 22 on this Mac (a union inside an anonymous struct), and it cannot see `sa_handler` or `s6_addr`, so it either refuses `sig_one.hero` and `in6.hero` or checks them by nothing |
| **(1e)** via the JSON dump `build` already runs | **approve**, as the source of paths and member names, with every verdict a C assertion; provisional on the chain's shape on 18.1.3, 20.1.8, 23.1.1 | the only measured source that sees anonymous members at any depth AND header macros, identical in structure on 21 and 22; read by `kind`, `name`, `isImplicit`, `tagUsed`, never `qualType`, whose text differs. Unbuilt: whether it rides the existing dump unit with no new process |
| **(1f)** classify OR sum | **object** | blind on real `glob_t` with both arms, and on `SA` missing `x` or `kind`, `SB` both, `PADU` |
| **(1g)** refuse `==`/`hash`/map key unless proven union-free | **veto** as a blanket rule | refuses `tests/golden/run/fixedbugs-a-group-record-in-every-container.hero` (raylib `Color`, `==` and `{Color: i64}`, runs today) and `in6.hero`; with a proof it is (1e), and I approve that |
| **(1h)** construction-arity | **approve** | the only route that lets a Heroes program read an SDL3 key or mouse event with no hand-written C; today none can (§ 3); its C runs on both clangs with zero warnings (`work/sdl/h1.c`) |
| **adjacency form** (§ 4.1, nobody listed it) | **approve** as Q2's completeness probe for STRUCTS with a complete type, not unions | names the gap instead of a declared field; sees `SB` by one arm; over the tracked tree it moves no program but this sitting's probes (1038 records, 2158 assertions). Fires on a union record bound by one member (`r3_one.hero`), so unions stay out of it |
| Q1's any-arity rule, *a record holding any field in a union* | **veto** as written | refuses `in6.hero`: `==` on two IPv6 addresses, correct today because `s6_addr` covers its union. I approve the narrower rule: refused where a declared field lies in a union it does not cover (`SB` by `b`), judged in C, `sizeof` of the arm against its siblings', names from (1e) |
| the C++20 probe (looked further for Q2) | **object** | names the omission on 22 and not on 21, and lays out `struct Empty {}` at one byte |

**Q2, the union record's probe.** One member named stays complete: `r3_one`
runs, no operation observes the claim, and `==` is classify's already. Two or
more members, read-only, stay legal and must stop warning: `#pragma clang
diagnostic ignored "-Wexcess-initializers"` in the probe block silences
`read.hero`'s and u17's warning and still names `c` for `S3` (both clangs,
`work/plat/excess.c`). The mapper guard (the critic's) is owed whatever else
lands: clang naming a declared member means the reading is misaligned.

**Q3.** Two probes: overlap and coverage at `used` records ((1a)/(1b),
quadratic but only there: `Color` 6 pairs, `Matrix` 120 if ever compared);
adjacency at every non-`partial` struct record with a complete type
(linear). The dump adds names, never verdicts.

**Q4.** For `SA` under (1h): `record SA` declaring `kind`, `i`, `f`, `x`,
built naming one arm, `SA(kind: 1, f: 0.5, x: 3)`, every field readable.
The message for a struct must stop saying *`T` is a `union`*: *`i` and `f`
share bytes in an anonymous union of `u.h`'s `SA`*. A bit-field binds, with
§ 4.3's four C forms, because curl's HSTS callback must read and write
`includeSubDomains` and a refusal sends that program to a shim; exit 1 from
the marker if the panel prefers a refusal; exit 2 never.

**Prediction.** Under (1e) via the JSON dump with (1h), an SDL3 event loop
over the real `SDL_Event`, built as `SDL_Event(type: 0)`, polled, then
reading `e.key.scancode` and `e.user.code`, builds at exit 0 with zero
warnings and no file of C beside it, and `in6.hero` stays exit 0 with `true
false 1`. And on Ubuntu clang 18.1.3, P25 to P27 are silent: no clang this
project meets reports a designated omission in C.

**What would change my verdict.** The (1c) veto lifts only if `glob_one`
and u08 stay legal. The (1g) and any-arity vetoes lift if the rule is
limited to fields that do not cover their union, or if a measured program
shows `==` over a covering arm answering wrongly. (1b) becomes `object` if
18.1.3 or 20.1.8 disagree on P21 to P24; (1e)-JSON becomes `object` if the
chain's shape differs on any of the three; the adjacency form becomes
`object` if a real binding that runs today declares its fields out of the
header's order. If P25 to P27 fire on 18.1.3 they fire on the floor and
nowhere above it, which is worse, not better: a verdict one clang gives is
not a verdict.

## 8. Cost

All of it in my copy: one seed build, two instrumented compiler builds
(`work/v_op/`, `work/v_adj/`, never to be committed), three census passes
of 447 files three at a time, about 40 hand-written C units and 20 small
`.hero` programs. Nothing timed, no paid run, nothing written in the trunk
but this file.

Status: complete, waiting on § 6's readings.

**Corrections to § 7, same session, on rereading it against the commands.**
(1a)'s row says *all 13 overlapping cases*: it is **13 pairs in 12 cases**
(`S2U` with all five fields fires two pairs). (1b)'s row says that over every
record it refuses *`read.hero` and five more correct read-only programs (§ 5's
union rows)*: § 5's rows were the ADJACENCY instrument's; (1b) itself was run
on my 34 cases, where it errors on `W` naming both members, and by the critic
on `read.hero` (its § 9). That it refuses the lane's `u01`, `u02`, `u04`,
`u05`, `u09` and `u14` is an inference, unrun. And Q2's paragraph says the
pragma *silences `read.hero`'s and u17's warning*: it silences the PROBE's
excess-elements warning, which is `read.hero`'s only one and one of u17's
two; u17's other, *initializer overrides* at its construction on line 9, is
not touched by it, and (1a)/(1b) at `used` records refuse that program
anyway.

## 9. Settled on the coordinator's platform readings

Read 2026-10-02 from `docs/panel/186-reports/coordinator-platform-readings.md`
and from the raw outputs it names (`<scratchpad>/p186/plat-out/`), each
diagnostic mapped back to its P-number by line (`plat.c:54` P21, `:56` P23,
`:68` P28, `:69` P29, `:70` P30).

- **`plat.c`**: Debian clang 18.1.8, Debian clang 20.1.8 (a Linux target, and
  `x86_64-pc-windows-msvc` and `x86_64-w64-windows-gnu`), and clang 23.1.1 on
  the Windows box each flag exactly P01, P03, P05, P06, P07, P11, P12, P13,
  P14, P18, P21, P23, P28 (errors), P29 and P30 (warnings): the set Apple 21
  and Homebrew 22 flagged. So, on five clangs and three targets: **(1b)'s
  armed `-Winitializer-overrides` is the same verdict** (P21 and P23 errors,
  P22 and P24 silent); **no clang reports a designated omission in C** (P25
  to P27 silent; on 18.1.8 `plat.c:62` says *unknown warning group
  '-Wmissing-designated-field-initializers', ignored*); **GNU zero-sized
  members raise no false positive** for (1a) or adjacency (P08, P16), on
  both Windows targets too; **the bit-field forms hold** (P17 and P19 pass,
  P18 fires).
- **`plat_posix.c`**: on 18.1.8 AND on 20.1.8 (both raw files hold the
  section; the readings file says 20 was not run, and its own output says it
  was), X01 fails through glibc's spelling, `__builtin_offsetof(struct
  sigaction, __sigaction_handler.sa_handler)`, and X04 is the overrides
  error. (1a) and (1b) see through the macro on glibc as on Darwin.
- **`-fdump-record-layouts`**: two texts for the same record, 18.1.8, 20.1.8
  and Apple 21 against Homebrew 22 and 23.1.1. And something my § 4.2 did
  not see: **under `-fsyntax-only` it prints only the records Sema happened
  to lay out**. For `plat_lay.c` that is ONE record, the nested anonymous
  union, on all five clangs (counted again here on 21 and 22: 1); `SA`,
  `DEEP`, `BF` and `FAM`, declared as variables, are not printed. My § 4.2
  layouts came from `-c -o /dev/null` (`lay.c`), and `sdlay.c`'s 62 records
  are what SDL3's own `sizeof` assertions forced. A route on this dump would
  have to force each record's layout and still read a text that differs by
  version.
- **The JSON dump**: 5 `MemberExpr` on all five clangs, the same `name`
  lines (`hero_lay_SA_i`, `i`, `hero_lay_DEEP_i`, `i`); no `isImplicit` on
  a `MemberExpr` anywhere, so the anonymous link is the one with no name, on
  every clang, and that is what a reader keys on.

**Not run, and said so**: the CI's own Ubuntu clang 18.1.3 and the CI's
Windows leg's own 20.1.8 build. The readings are the same majors from other
builders; I take them as the verdicts, and the CI run of whichever route
lands is what reads the exact builds.

**My provisional verdicts, settled.** (1b)'s condition is met: approve.
(1e) through the JSON dump: the chain is the same on all five, approve.
`-fdump-record-layouts`: object, now on two measured counts (two texts across
five clangs, and it prints what Sema laid out rather than what was asked).
Q2: no clang reports a designated omission in C, so the completeness probe
stays positional or becomes the adjacency form; a designated probe can never
be its replacement. The first half of my prediction (silent on 18) held, on
18.1.8; the SDL3 half stays open until a route is built.

**One line per route:**

- **(1a)** ranges: **approve**, at `used` records, after a bit-field filter and only over complete types.
- **(1b)** designated probe, overrides armed: **approve**, at `used` records; the same verdict on five clangs and three targets.
- **(1c)** refuse records over a struct holding an anonymous union: **veto** (`glob_one.hero` on libc's `glob_t`, and u08, run today).
- **(1d)** nested group in the declaration: **object** (a flat declaration still needs a detector in `build`).
- **(1e)** via `-fdump-record-layouts`: **object** (two texts, prints only what Sema laid out, blind to `sa_handler` and `s6_addr`).
- **(1e)** via the JSON dump `build` already runs: **approve**, as the source of paths and names, every verdict a C assertion.
- **(1f)** classify OR sum: **object** (blind on real `glob_t` with both arms).
- **(1g)** refuse `==`/`hash`/map key unless proven union-free: **veto** as a blanket rule (raylib `Color` in `fixedbugs-a-group-record-in-every-container.hero`, and `in6.hero`).
- **(1h)** construction-arity: **approve** (the only route to a shim-free SDL3 event loop).
- **adjacency form** (new): **approve** as the completeness probe for struct records with a complete type, never over a union record.
- **Q1's any-arity rule as written**: **veto** (`in6.hero`); approve it narrowed to a field that does not cover its union.
- **C++20 probe** (new): **object** (clangs disagree, and C++ lays the type out differently).

Status: complete; § 6's readings received and settled above.
