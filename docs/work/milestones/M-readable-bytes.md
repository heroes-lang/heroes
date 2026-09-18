# M-readable-bytes — a C byte buffer becomes text a program can print

**Scheduled and opened 2026-09-18 by author instruction**, on panel 161's
largest unresolved finding, and in the author's own framing: **there is no way
to turn a C `char[65]` field into text, so whoever binds `struct utsname` to
print the machine's name cannot do it. Resolve it.** The instruction was given
in Italian and is written here for what it meant, as CLAUDE.md §11 asks; the
original is in this commit's own history.

**What it delivers.** The program below, compiling and printing. Nothing
smaller is the deliverable, because the gap was found by writing that program
and not by reading the language.

```
extern "sys/utsname.h"
    record Utsname tag utsname
        sysname: i8[256]
        …
    function uname(@name: Utsname) -> i32

function main()
    u: Utsname @ …
    _ = uname(@u).to_i64().must()
    print(u.sysname)
```

**What warrants it.** design.md §1.11: everything comes from C, so everything a
program touches arrives through §4.19. A header field the language can bind and
cannot read is a binding that gives nothing, and
`.claude/rules/c-boundary.md`'s standing instruction is that the FFI be
**complete** — *a library Heroes cannot bind is a library the author must leave
C code around for*.

**Two walls, both measured 2026-09-18 on this Mac, and one program hits both.**

| the wall | what the compiler says |
|---|---|
| bytes to text | `print` and `to_str` take *an integer, a float, `bool` or `str`*, and refuse `i8[256]` and `[u8]` alike |
| building the struct | `Utsname(sysname: [0])` is `fixed_array_length`: the literal must hold exactly 256 elements, **804 characters on one line** |
| `partial` as an escape | does not help: a declared field's length is checked whether or not the record is `partial` |
| a `tag` with no fields | accepted, and wrong: that is a HANDLE, C's pointer to the type, so `uname(@u)` would pass a pointer to a pointer |

**The direction is what makes this a language question rather than a built-in
somebody forgot.** Heroes goes from `str` to bytes and cannot come back: `s[i]`
yields a `u8` and `s.chars()` yields single-character `str`, and nothing in
`spec § 11`'s conversions or `spec § 13`'s boundary rules goes the other way.
`c.validated()` is the one inbound door and it answers only a `cstr`, which
`spec § 13` says no record holds. So this is not one missing name; it is a
one-way door in the type system, and every program that receives bytes from C
is on the wrong side of it.

**What it is not.** It is not a standard library (CLAUDE.md § 13), and it is not
UTF-8 validation policy invented here: `read_file` already returns a `str?` from
arbitrary file bytes, so the language already has an opinion about what
happens when bytes are not valid text, and this milestone owes consistency with
it rather than a new answer.

**TWO OF THE FOUR ROWS ABOVE WERE FALSIFIED AT THE CLOSE, 2026-09-18, and they
are corrected underneath rather than rewritten.** `partial` **does** help: it
lets a program declare only the fields it reads, which cuts the literal for
`struct utsname` from five arrays of 256 to one — 4052 program tokens to 903,
measured by panel 163's completeness critic. And the second wall was **not a
wall**: a record a C function fills is obtained from a function that returns it,
which runs today, costs zero compiler lines, and is already used seven times in
this repository's own golden tests. Both sentences were written here by a
coordinator who had a shell and did not run them, which is the pattern panel 163
turned into a rule for every brief.

**Why here, before M-core-packages.** That milestone declares bindings against
roughly eighteen C headers. Every `char[N]` field among them lands on this wall,
and a binding written before the wall moves is a binding rewritten after it.
The same argument M-arm-platform used for its own position, one row over.

*******************************************************************************
**OPEN: 0**

*******************************************************************************
