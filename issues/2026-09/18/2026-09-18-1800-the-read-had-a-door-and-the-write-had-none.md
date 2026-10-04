# The read had a door, and the write had none

2026-09-18. Panel 164, convened between milestones on defect 061, which panel 162
had filed as *a fixed-array field cannot be passed to C at all*.

## The decision

| | |
|---|---|
| date | 2026-09-18 |
| decision | the language gains ONE type rule — `f.ptr()` lends a binding's fixed byte field to a `ptr` parameter the call gives the extent to, and C may write back through it. A field reaching a `cstr` stays refused, and the automatic decay is refused on veto |
| reason | the direction the sitting was convened on already worked; the two that did not were a binary field answered WRONGLY at exit 0, and a write direction nothing in the language could reach |
| ratification | by delegation, in the session that convened it. The author has not read the synthesis |

## What the sitting found before it found anything else

**Its own premise was false, and three seats measured it independently.** Defect
061 said in bold *"No Heroes-side shim can route around it"*, and the shared
brief carried that sentence as the question. Run:

```
slot_len(t.name.validated_bytes().must().cstr())
  terminated i8[8]      ->  2    exit 0
  UNTERMINATED i8[8]    ->  8    exit 0
```

Eight, not seventeen. The same struct in C prints **17** under `-Weverything`
with ASan and UBSan silent. The true sentence is narrower — no Heroes-side shim
lends the field's **ADDRESS** — and the route that works **copies**.

**And the emblem came from the safe class.** POSIX requires `struct utsname`'s
members to be terminated, so `strlen(u.sysname)` — the defect's own headline
program — is the case where the decay would have been sound. The historian
fetched it. A sitting arguing route 1 from that example was weighing it on the
13 of 50 fields with a terminator rather than the 37 without.

## The vetoes, and they are refusals rather than prices

Four seats vetoed the automatic decay and two vetoed a `cstr` lend, on one
ground: design.md §1.11 lists NUL-termination among the decisions *not to be
revisited*, and it is load-bearing three times — `hero_str_cstr` is zero-copy
**because of** the NUL, `validated` reads **to** it, and `guard_arguments`
checks only null **because** it is assumed. The ffi-pragmatist put it in those
words: *there is no ergonomic gain I will trade for it.*

The historian's survey is the same finding from outside: of nine languages that
bind C, **eight refuse the decay**. The ninth is Nim, whose own manual says the
implicit conversion *"will be removed in future releases"* — and CLAUDE.md §6
already rules on Nim: copy the surface, never the implementation.

## The blind spot, which is bigger than what the sitting was convened on

**The write direction had no door at all.** 50 of 141 pointer parameters across
16 real headers are non-`const` — C fills them. `getcwd(@buf: ptr, size: u64)`
binds and `heroes check` passes it, and nothing in the language could produce the
argument. It was on defect 061's **own unrun list** and stayed there through five
briefs; the completeness critic ran it.

The critic also found the option set was **seven** and not four. Route 5, a
terminator declared on the field, is **refused**: it adds a fourth notion of the
field's length to three that already agree, and Cyclone's own rule makes the
usable run N−1 while all three say N. Route 6, the extent declared on the
PARAMETER, is **queued**: it is the only route where the compiler CHECKS the
extent, and its whole value is a number nobody has measured.

## The defect inside the repair, and it is worth more than the feature

The first implementation rendered the lend from `emit/storageless.hero`, which is
right for `validated_bytes` and wrong here: the IR loads a record into a fresh
temporary before reading a field of it, so C was handed the address of a **COPY**.

```
read: 16      after C wrote: 16      <- the copy
read: 16      after C wrote: 36      <- the place
```

Reading through a copy is correct by accident. Writing through it is **lost in
silence**, at exit 0, with every suite green. It was caught only because the
probe made C write and then **read back**. `emit/field_lend.hero` follows the
load to the cell, and `emit/unread.hero` learned that a `ptr` reads a PLACE and
not its operand — which is that module's whole stated subject.

## What it cost

Spec **8106** real on `claude-opus-5`, +66 net, digest `3c065c560426eb07`,
ledger row 83; the −6 removal panel 162 found and declined to spend is inside it.
Two modules split along seams their own docs already named — `check/lend_types.hero`
out of `check/lending.hero`, `lend_errors.hero` out of `ffi_errors.hero` — so
`layout` stays green with no ceiling raised. Compiler's own tests **659**, the
net's own **158**, `run` 127, `annotations` 166, `records` 24. The seed
regenerated and the fixpoint verified byte-identical.

## And the tag that could not be moved

`m-readable-bytes` was pushed over defects 061 and 062, and the remote's ruleset
*release and milestone tags are immovable* refused the deletion. Both defects are
now closed, and `TAGGED_OVER` in `tests/harness/suite_records.hero` records the
mistake rather than forgiving it: the row names the exact defects and the done
records that closed them, and `records/tagged` refuses it the day one of them
reopens. Author decision, 2026-09-18.
