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

**Why here, before M-core-packages.** That milestone declares bindings against
roughly eighteen C headers. Every `char[N]` field among them lands on this wall,
and a binding written before the wall moves is a binding rewritten after it.
The same argument M-arm-platform used for its own position, one row over.

*******************************************************************************
**OPEN: 1**

- [ ] **M-readable-bytes** | the inbound direction: a C byte field becomes a `str`, and a fixed-array field becomes buildable without writing every element | `spec § 11` · `spec § 13` · `selfhost/check/builtins.hero` · `selfhost/inventory.hero`

    **Origin:** author instruction 2026-09-18, on panel 161's unresolved
    finding, which panel 161's llm-ergonomist reached from the specification
    alone by writing the program.

    **What the sitting has to settle**, and both halves block the one program:

    - **The read.** What turns a run of bytes into a `str`, what it is called,
      and what it does with bytes that are not valid UTF-8 — `spec § 3` says
      `str` is *immutable UTF-8*, so the answer is a `str?` or an abort and the
      sitting says which. Whether it answers `[u8]`, `i8[N]`, `u8[N]` or all
      three is the same question asked four times.
    - **The build.** A fixed-array field of 256 elements needs a literal of 256
      elements today. Panel 081 recorded this gap and did not close it; it is
      recorded again here because the program that provoked it is the same one.

    **Two things the sitting must not lose.** `read_file` already returns a
    `str?` from arbitrary bytes, so the failure mode is already chosen
    somewhere; and `spec § 10` fixes the outbound convention, *`s[i]` yields a
    `u8`*, which the inbound name should not contradict.

*******************************************************************************
