# Defect 037 closes, and the qualifier is obeyed, never guessed

2026-09-15. No sitting: panels 150, 151 and 152 had ruled and the author had
ratified all three the same morning, so this is the landing of what they left
owed — panel 152 R1's second half, R3 and R4, and panel 150 R3 and R4.

## The compiler compiles the author's word first, and reads what clang says about it

`record AI tag addrinfo` is emitted `addrinfo *`, as the author wrote it. When
clang refuses that with *must use 'struct' tag to refer to type 'addrinfo'* —
a sentence it produces only when the bare word names a struct tag of exactly
that spelling — the build learns the word and compiles once more with `struct
addrinfo *`. **Nothing is read from the header and nothing is probed.** Panel
151 measured that a spelling read from the header can no longer disagree with
the header; panel 152 measured that a one-bit probe says yes to a misspelled
tag too, because `struct nosuchtype *p;` is legal C. This route keeps every
refusal clang would make of the author's word, since the author's word is what
is compiled first, and obeys only the one request clang makes by name.

The mechanics are one new module, `cli/assemble.hero`, which is a ROUND —
pointee check, units, link — and a loop in `cli/produce.hero` that runs it
again when a refusal taught a tag. A program whose tags need nothing runs one
round and pays nothing; the one-module case builds in 0.47 s cold and 0.30 s
warm. `--emit-c`, the seed and the blessed emissions write the author's word,
because they are the first emission.

## The silent half is refused rather than widened

`acquires`, `borrows` and `consumes` on a type that reaches no handle are
`error[unread_mark]` (panel 150 R3). On a `ptr` the word could never be read —
the completeness rule keys on the handle's declaration and a `ptr` has none —
and panel 150 measured that obeying a wrong one frees memory `malloc` never
gave. Every mark in the tree already sat on a handle, so the rule refused no
program, and with `struct` supplied every struct C names with two words has a
handle spelling. The design.md Part 6 row names the falsifier: two opaque
`void *` families in one program, which `one_tag_one_type` caps at one `tag
void`.

## Two predictions missed, and the misses are on the instrument they named

The spec moved **+1 vendored and +0 real**: the −3 phrase *what the header
leaves opaque* and panel 150's +7 correction cancelled on `claude-opus-5`. Panel
152 R3 said 7978 and panel 150's warden said 7981; both are MISSED under the
warden's own clause — *any other number means the landed text is not the text
priced* — and the ledger row says so rather than rounding either toward the
number that landed. design.md:2192 stops saying the tag is written *verbatim*
(panel 152 R4), and says what is now true: the author's word is the identity,
and `struct` is supplied where the header's own refusal names it.

## What the day's first net taught

1855 passed, 6 failed. Four were one cause I made: the spec's pins moved in
`selfhost/measure/pinned.hero` while the seed was being emitted, so the binary
under test carried the old digest and three `spec` checks and one `surface`
row went red against a document that was right. Two were real: `emit/ctype.hero`
had grown to 399 lines of code against a decided 395, and the handle's spelling
moved into `handles.hero`, where the definition of a handle already was; and
the two new run programs had no blessed emission, which the suite says how to
bless. The seed was regenerated after both, and the compiler built from it
re-emits it byte for byte.

## What stays open

Defects 042 and 043, both filed by panel 152's llm-ergonomist and neither ruled
on. Panel 153 convenes on both.
