# 059 — M-readable-bytes: the direction `str` never had, and a wall that was not there

## Goal

The author's instruction, on panel 161's largest unresolved finding: **there is
no way to turn a C `char[65]` field into text, so whoever binds `struct utsname`
to print the machine's name cannot do it. Resolve it.**

Two walls were measured at the opening. Reading a byte field as text, and
building a record with a 256-element array field so a C function can fill it.

One of the two was real.

## What surprised

**The conversion was already built, and one line was the whole difference.**
`hero_str_try_from_cstr` has existed since panel 089, `str` already carries a
length as well as a NUL, and `read_file` already answers `.err` with `not_text`
on invalid UTF-8 — measured, `printf 'abc\xff\xfe\x80def'`. Panel 162's compiler
seat put it in one sentence: line 304 is `size_t n = strlen(p);`, and **that one
line is the entire difference between what existed and what the sitting wanted.**
A C `char[N]` field is not a `cstr` and need not be terminated, so the new
function takes the extent from the caller instead of scanning for a zero.

**A sitting's resolution can be wrong three times and still be the right
resolution.** Panel 162 resolved *three existing built-ins widen*. `validated` is
a library function, not a built-in; making it one was refused on a measurement,
because `ir/owned_release.hero` finds that declaration by name and origin and
synthesises calls to it. `repeat` cannot be widened as written, because a call
would have to take its type from context. And the third correction is in the
error clang gave when the first emitter draft read the operand from a temporary:
a fixed array has no C storage to be a temporary in. Each correction is written
into the sitting's own file rather than applied quietly.

**The second wall did not exist, and the brief that said it did was mine.** Three
seats of panel 163 ran the experiment the brief had not: a record a C function
fills is **asked for**, not built. A function returning it — in the `extern`
group or in Heroes — plus `partial` to declare only the fields read. It runs
today, and the completeness critic found that **this repository's own golden
tests already return a struct by value from C in seven places**.

**And the critic separated a variant nobody else had.** Three seats ran the
version with a hand-written C shim; the one with a **Heroes** producer and no
author C at all is what saves the resolution from `.claude/rules/c-boundary.md`,
which calls *a library the author must leave C code around for* the failure the
FFI must not have. 874 tokens with no C, against 3979 for the full literal.

**Six of one brief's premises were false and three shared a shape.** *A call
typed by context does not exist* (there are seven such arms and two are calls),
*`zero_of` already provides a zero default* (it is a C11 type probe inside an
expression that is never evaluated), *`partial` does not help* (it cuts a
literal from 4052 tokens to 903). Every one was written by a coordinator who had
a shell and did not run it. CL-077 binds a brief's NUMBERS to a command; the rule
this milestone adds binds the other half, and it is now in the panel skill:
**a brief's negative sentences are run, or they go out as questions.**

**A refusal can be dead four different ways.** Leaving an out-parameter
uninitialised was refused by Rust's own withdrawal in writing, by Zig keeping it
only with a keyword and runtime poisoning, by POSIX's value-result idiom — the
ffi-pragmatist measured `getsockname` returning success while writing nothing —
and by this repository's own `ir/inout.hero`, which already records a deleted
out-cell store printing the same four lines under both sanitisers because the
indeterminate local happened to be zero.

## What broke and why

- **`use of undeclared identifier 't1300'`**, from clang, on the first emitter
  draft. Cause: the operand was read from a temporary, and `emit/ctype.hero`
  answers `C has no assignable array` for a fixed array on purpose. Fix:
  `emit/storageless.hero`, the module that exists for exactly that class and
  renders such a value at its use site. The error led to the right module rather
  than to a patch.

- **`assert failed: entries.len() == 39`.** The built-in inventory's own test,
  the moment `validated_bytes` landed — and it caught a worse mistake than the
  count: the first draft inserted the name beside `to_f32`, and **the table's
  order is identity**, a builtin reference carrying an index into it. `to_u8`
  would have moved from 35 to 36 and every name after it with it. Appended
  instead, as `lease` and `end_lease` were.

- **Nine failures in the full net, all one name.** The `unsupported` diagnostic's
  second note enumerates every built-in the backend emits, so a forty-first made
  eight snapshots stale at once. Corrected surgically — one substring — rather
  than regenerated, because a regenerator rewrites a snapshot and cannot invent
  the `#~` annotation beside it.

- **The formatter moved a comment out of an array literal, twice.** Written
  beside a list element, it came back below the function. Not filed: it
  relocates rather than drops, and the file's own convention puts each flag's
  reason in the block above. Written down because the next session to comment a
  list element will meet it.

- **A spec sentence that was advice, not a rule, priced and refused.** *A record
  a function fills is asked for, not built* costs **+37 real** measured alone;
  the correction it was bundled with costs **+10**. The specification is the
  whole language and nothing else, so the +10 landed and the +37 did not.
