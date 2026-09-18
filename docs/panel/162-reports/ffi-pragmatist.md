# Panel 162 — ffi-pragmatist

Every command below was run on 2026-09-18 on this Mac (Darwin 25.6.0, arm64), in
a copy of the tree at
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/4cbbc5a6-a98e-4b44-80b5-b6957110cd39/scratchpad/tree`,
with `build/` removed and the seed compiler built by
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`. Nothing is copied
from a document. Programs are in `.../scratchpad/w/`.

- `verdict`: **object** (with a **veto** on one named route, § 6)
- `section`: design.md §1.11 and §4.19; `spec § 13`; `spec § 10`

**One claim in the first draft of this report was wrong and is corrected in
§ 3**: I wrote that the handle route is *one library away from corruption*
before I had run it against a userspace writer. I then ran it, and the runtime
**caught it**. The correction is the most useful thing in this report and it is
kept where it happened (CLAUDE.md § RUN IT; records are corrected underneath,
never deleted).

---

## 1. The existing doors: all five are shut, and one of them matters more than the read half

The brief asked whether `s.cstr()`, `c.validated()` or `x: cstr @ s.lease()`
already reaches a `char[N]` field. **None does.** Header verified first, with the
command:

```
$ grep -n "char\|struct\|_SYS_NAMELEN" $(xcrun --show-sdk-path)/usr/include/sys/utsname.h
72:#define _SYS_NAMELEN    256
74:struct  utsname {
75:	char    sysname[_SYS_NAMELEN];  /* [XSI] Name of OS */
...
83:int uname(struct utsname *);
```

Five programs, each declaring the five `i8[256]` fields at the header's width,
each initialised with a **full 256-element literal** so the only error left is
the door being tried. `./heroes check`, verbatim:

| door tried | verbatim |
|---|---|
| `u.sysname.cstr()` | ``error[bad_operand]: `cstr` takes `str`, found `i8[256]` `` |
| `u.sysname.validated()` | ``error[type_mismatch]: expected `cstr`, found `i8[256]` `` |
| `u.sysname.lease()` | ``error[bad_operand]: `lease` takes `str`, found `i8[256]` `` |
| `strlen(u.sysname)`, `strlen(s: cstr) -> u64` | ``error[type_mismatch]: expected `cstr`, found `i8[256]` `` |
| `u.sysname.to_str()` | ``error[bad_operand]: `to_str` takes an integer, a float, `bool` or `str`, found `i8[256]` `` |

**The fourth row is new to this sitting and it is the one I stand on.** Heroes
does not give a fixed array C's array-to-pointer decay. In C, `strlen(u.sysname)`
is the line every binding writes; in Heroes the field cannot be handed to a
`cstr` parameter at all. So the field is not merely unreadable by Heroes — **it
is unpassable to C**, and no shim written in Heroes can route around it, because
the shim needs the same conversion. Under `.claude/rules/c-boundary.md`'s
completeness rule (*a library Heroes cannot bind is a library the author must
leave C code around for*) that is the sharper failure, and it is not what the
shared brief measured.

The only door that works today is one byte at a time. Measured, built, run:

```
    while n < 256 && u.sysname[n].to_i64().must() != 0
        n @ n + 1
```
prints `6`, then `68 97 114 119 105 110` — `Darwin`. The bytes are reachable;
nothing turns them into a `str` and nothing hands them back to C.

## 2. Route 2 of the shared brief is caught by clang, and that is the thesis working

`spec § 13` says *no record holds [a `cstr`]*. Route 2 proposes moving that rule,
so I tested whether an `extern` group can lie about the field:

```
extern "sys/utsname.h"
    record Utsname tag utsname partial
        sysname: cstr
```

`./heroes check` **passes silently**. `./heroes build` refuses, verbatim:

```
internal error: compiling the generated C failed:
.../d3_fieldcstr.hero:3:181: error: static assertion failed due to requirement
'sizeof (((struct utsname *)0)->sysname) == sizeof(char *)': heroes-ffi-field struct utsname sysname
... note: expression evaluates to '256 == 8'
error: clang refused the generated C
```

This is `importc`-style verification doing exactly its job, and it is the
strongest evidence in this report that the boundary is sound today. Three more
probes, all against real headers, all refused at `build`:

| probe | verbatim |
|---|---|
| `function malloc(size: u64) -> Utsname` (fabricate a handle) | ``error[ffi_return_type]: `malloc` does not return `Utsname` — that is what `stdlib.h` says, and clang read it`` |
| `u8[14]` against `char sa_data[14]` | ``error[ffi_field_type]: `Sockaddr.sa_data` is not `u8[14]` in `sys/socket.h` — clang read the header's struct and the field disagrees`` |
| `uname(@name: Utsname)` where `Utsname` is a fieldless handle | ``error[ffi_parameter_type]: `name` of `uname` is declared a different kind of thing from the header's `struct utsname *``` |

And the positive control builds and runs: `u8[128]` against
`unsigned char fh_data[128]` in `sys/mount.h`, beside `i8[14]` against
`char sa_data[14]`, prints `ok`. **Both element types are load-bearing and the
compiler distinguishes them correctly** — plain `char` is signed on this leg, so
`i8[N]` and `u8[N]` are not interchangeable and a route naming one is incomplete
on measurement.

Two consequences the sitting must carry:

- **Any route that puts a `cstr` where the header says `char[N]` must defeat this
  `_Static_assert`.** So route 2 cannot be a field declaration; it has to be a
  **projection at the use site**, leaving the field `i8[256]` and the assert
  intact. Say that in the resolution or an implementer reaches for the field
  spelling and lands back here.
- **FFI verification happens at `build`, not at `check`.** All four refusals
  above passed `heroes check` silently. That is a fact about the tool surface an
  author meets on day one, and it is worth the sitting knowing it even though it
  is not the sitting's question.

## 3. The build half: four routes refused, one accepted, and the corruption claim withdrawn

`uname(@u)` needs a `u`. Every route I could write:

| route | `check` | `build` | ran |
|---|---|---|---|
| full 256-element literal, five fields | clean | clean | **`68`** — it works |
| `partial` + `Utsname(sysname: [0])` | ``error[fixed_array_length]: `i8[256]` holds exactly 256, and this literal has 1 … (§4.19)`` | — | — |
| uninitialised `u: Utsname` | ``error[expected_binding_symbol]: expected `@` to declare a mutable, or `=` to bind, found end of line`` | — | — |
| field spelled `cstr` (§ 2) | clean | **clang refuses** | — |
| handle + `malloc` (§ 2) | clean | **`ffi_return_type`** | — |
| handle passed with `@` (§ 2) | clean | **`ffi_parameter_type`** | — |
| **handle, `u: Utsname @ nullptr`, `uname(u)`** | **clean** | **clean** | **`survived`, exit 0** |

**The correction.** I first read the last row as a corruption waiting for a
library that dereferences in userspace, and wrote that down. Then I ran it: same
shape, `record Tm tag tm`, `localtime_r(clock: nullptr, result: t)` with `t`
null — a pure libc function that writes through the pointer. Verbatim:

```
panic: a null pointer was read through — a handle or `ptr` holding `nullptr`
reached C where C dereferences it, at offset 0x0, called from b6localtime.main
exit=134
```

**The runtime catches it.** design.md §1.12 holds at this boundary, by
measurement and not by argument, and the `uname` case printed `survived` only
because Darwin's `uname` is a syscall whose `copyout` to a null address returns
`EFAULT` without ever dereferencing. So: **no route I found compiles and
corrupts.** My earlier sentence is withdrawn.

**What is left is a smaller finding and still a real one.** The fieldless-`tag`
handle is the *only* build route that compiles, and it is a dead end by
construction: `nullptr` is its only literal (`spec § 13`), and § 2 shows there is
no way to fabricate a non-null one, because `malloc` cannot be declared to return
it. So a program that takes the handle route compiles, links, runs, panics at
134 — or, against a syscall, **exits 0 having done nothing and reported
success**. That last case is the one I would file: `uname(NULL)` returns `-1`,
`to_i64()` cannot fail, `.must()` passes it through, and the program prints a
success it did not have.

**And the compiler's own note aims the author into the wall.** The
`ffi_parameter_type` refusal of `uname(@name: Utsname)` says:

> *if it is a struct, declare it as a `record` inside this same `extern` group
> and the header owns its layout*

That is correct advice pointing at the one form the language cannot instantiate
without 804 characters of literal. The build half is not an ergonomic nicety; it
is the destination the diagnostics already send people to.

## 4. How real headers use `char[N]`: most are not NUL-terminated text

Sixteen system headers, one clang JSON AST walk, deduplicated:

```
$ clang -fsyntax-only -Xclang -ast-dump=json all.c > all.json
total complete-struct fields: 712
char[N] fields: 50
```

Headers: `sys/utsname.h dirent.h pwd.h grp.h netinet/in.h sys/socket.h net/if.h
netdb.h sys/stat.h time.h termios.h sys/un.h arpa/inet.h ifaddrs.h sys/mount.h
utmpx.h`. Classification is mine; the header's own comment is quoted where it
settles the class.

**Not text at all — 21 of 50 (42%).** `_opaque_pthread_*.__opaque` ×9 (to
`char[8176]`), `__mbstate8[128]`, `__darwin_arm_sme_za_state.__za[4096]`,
`__darwin_arm_sme2_state.__zt0[64]`, `__darwin_arm_sve_p_state.__p[16][32]`,
`__darwin_arm_sve_z_state.__z[16][256]`, `fhandle.fh_data[128]`
(`/* file handle value */`), `searchstate.ss_fsstate[548]`,
`ip_opts.ip_opts[40]`, `sockaddr.sa_data[14]`, `sockaddr_in.sin_zero[8]`,
`sockaddr_storage.__ss_pad1[6]`, `__ss_pad2[112]`.

**Fixed-width text, terminator not guaranteed — 14.** `utmpx.ut_id[4]`
(`/* id */`, `_UTX_IDSIZE` is 4, and a 4-character id fills it),
`utmpx.ut_user[256]` `ut_line[32]` `ut_host[256]`, `lastlogx.ll_line[32]`
`ll_host[256]`, six `IFNAMSIZ` fields (`ifreq.ifr_name` `/* if name, e.g. "en0" */`,
`ifaliasreq.ifra_name`, `ifdrv.ifd_name`, `ifmediareq.ifm_name`,
`ifstat.ifs_name`, `net_event_data.if_name`; `IFNAMSIZ` is 16 and a name may
occupy all of it), `vfsconf.vfc_name[15]`.

**Text whose length is carried by a SIBLING FIELD — 2, and they matter out of
proportion.** Verbatim:

```
sys/dirent.h:106:	__uint16_t  d_namlen;   /* length of string in d_name */
sys/dirent.h:108:	char      d_name[__DARWIN_MAXPATHLEN];
sys/un.h:77:	unsigned char   sun_len;        /* sockaddr len including null */
sys/un.h:79:	char            sun_path[104];  /* [XSI] path name (gag) */
```

**Reliably NUL-terminated text — 13.** the five `utsname` fields, `statfs` and
`vfsstatfs` `f_fstypename`/`f_mntfromname`/`f_mntonname` (3+3),
`netfs_status.ns_mountopts[512]`, `mach_service_port_info.mspi_string_name[255]`,
`ifstat.ascii[801]`.

**So the answer to the brief's question 1 is no.** 13 of 50 (26%) are reliably
NUL-terminated text. **A route that assumes termination is wrong for
three-quarters of the population, and wrong in the dangerous direction**: on a
full `ut_id[4]`, a terminator-seeking read walks into `ut_line`; on `sa_data[14]`
there is no terminator at all and it walks off the struct. That is a read out of
bounds produced by a *correct* binding — §1.12 territory, and unlike § 3's null
handle there is no runtime guard for it, because the pointer is valid.

The dirent/`sockaddr_un` pair is the constructive half: **the header already says
where the text stops, in a field the binding already declares.** A route where
the caller supplies the end (the shared brief's route 3) is the only one of the
four that is right for all four classes: correct for `d_name` with `d_namlen`,
for `ut_id` with 4, for `sysname` with a scan the program does, and it never
invents a terminator for `sa_data`.

## 5. The shapes beside it (CL-061)

- **`i8[N]` and `u8[N]` are both needed and both checked.** Measured in § 2:
  `u8[128]` on `unsigned char fh_data[128]` builds; `u8[14]` on
  `char sa_data[14]` is `ffi_field_type`. A route naming one element type is
  incomplete on measurement, not on taste. `spec § 10` fixes `s[i] -> u8`, so the
  inbound name must accept the signed one too or half the text fields are out.
- **Two-dimensional `char[N][M]` exists and is refused**, cleanly, at `check`:
  `__darwin_arm_sve_p_state.__p` is `char[16][32]`. Verbatim:
  ``error[ffi_field_type]: `i8[16][32]` cannot be a field of a `record` in an
  `extern` group — the header owns the layout … (§4.19)``. No route in this
  sitting changes that; naming it keeps the refusal honest.
- **`const char[N]`**: panel 161 measured that this is refused at every spelling
  and that it is the `const`. Not re-measured here; not contradicted. **Zero** of
  the 50 fields are `const`-qualified, so the refusal costs nothing on this
  population.
- **`char[N]` as the last field**: `dirent.d_name` is last and `d_reclen` bounds
  the record. A slice-shaped route handles it; a terminator-shaped route reads
  past `d_reclen` on a short record.
- **`char[1]` as a tag byte**: none in this population; it is the degenerate
  fixed-width case, and the one where a terminator assumption over-reads with
  certainty.
- **Nested**: `sockaddr_storage` holds `__ss_pad2`; `ifreq` holds a union of
  `struct sockaddr`, which holds `sa_data[14]`. Not separately measured.

## 6. What I veto, as a refusal

**I refuse any resolution that changes how a `char[N]` field is DECLARED in an
`extern` record** — spelling it `cstr`, `str`, `[u8]`, or anything whose `sizeof`
is not the header's. The `_Static_assert` quoted in § 2 is the property design.md
§1.11 and §4.19 rest on at the boundary: a wrong FFI signature is a compile
error. It fired at `256 == 8` naming the struct and the field, and it must keep
firing. The field stays `i8[N]`/`u8[N]`; whatever this sitting adds is a
projection at the use site. This is not a price and there is no version of it I
will trade for ergonomics.

I do **not** veto: a built-in answering a byte run, a slice-shaped read, a
repeat or zero-fill literal, or a zero default for a fixed array in an `extern`
record. None of those touches layout.

## 7. What I object to

**Route 4, refusal, is not available on this measurement.** design.md Part 6
holds a refusal to naming the program fact that would make it wrong, and § 1
supplies it: `strlen(u.sysname)` — the most ordinary line in any C binding — is
`error[type_mismatch]: expected cstr, found i8[256]`. A language whose founding
constraint is that everything comes from C cannot refuse to pass a struct field
to C. Note this survives my own correction in § 3: robustness is intact, and the
objection is completeness, which is `.claude/rules/c-boundary.md`'s own word.

**Route 2, the `cstr` view, is the wrong shape on the population.** 37 of 50
fields are not reliably NUL-terminated text, and `c.validated()` is defined as
reading to a NUL. Adopting it makes the *correct* binding of `ut_id[4]` and
`sa_data[14]` an out-of-bounds read with no runtime guard.

**The build half must ship in the same resolution as the read half.** § 3 shows
the compiler today accepts exactly one build route, that route is a dead end, and
the `ffi_parameter_type` note aims the author at the form that needs 804
characters. Closing the read half alone leaves that in place.

## 8. Prediction, falsifiable, with its milestone

At the close of **M-readable-bytes**:

1. **A terminator-only route will make `utmpx.ut_id` and `dirent.d_name`
   wrong.** Concretely: bind `struct utmpx` and read `ut_id` from a record whose
   id fills all four bytes; a terminator-seeking read returns bytes from
   `ut_line`. If the milestone ships a terminator-only route and this program
   does **not** over-read, I am wrong.
2. **Rung 3 of design.md §4.19's ladder (SQLite) needs no C shim under a
   slice-shaped rule.** `sqlite3_column_text` returns
   `const unsigned char *` with `sqlite3_column_bytes` beside it — the same
   length-carried-by-a-sibling shape as `d_namlen`. `examples/sqlite/main.hero`
   does not read a text column today (grepped: no `column_text`). If the adopted
   route cannot express that pair without a C shim, it has failed the ladder it
   was written for.
3. **`strlen(u.sysname)` will compile.** If the milestone closes with the
   `uname` program printing and that line still saying
   `error[type_mismatch]: expected cstr, found i8[256]`, the completeness rule is
   unmet and my verdict stands unchanged.
4. **`u: Utsname @ nullptr; uname(u)` on a fieldless handle will be a compile
   error**, not a silent exit 0.

## 9. Condition that changes my verdict

I move from **object** to **approve** for a resolution that, together:

- leaves the field declared at the header's width so § 2's `_Static_assert` still
  fires (non-negotiable, § 6);
- **lets the caller say where the bytes stop** and does not require a terminator,
  because 37 of 50 measured fields have none;
- accepts `u8[N]` as well as `i8[N]`, which § 2 measured as two distinct types
  the compiler already refuses to confuse;
- makes the field **passable to a `cstr`/`ptr` parameter** as well as readable,
  so a binding can still call `strlen`, `inet_ntop`, `memcmp`;
- answers a `str?` on invalid UTF-8, consistent with `read_file`'s already-chosen
  failure mode, which is the shared brief's own cheapest-answer test;
- ships **some** build route for a struct with a long array, literal or default —
  I do not care which, both are sound, and § 3 measured what the language steers
  an author to when there is none.

I do not require a diagnostic for the fieldless handle in this milestone, having
withdrawn the corruption claim: it panics at 134 rather than corrupting, so it is
a defect and not a soundness hole. It should still be filed.
