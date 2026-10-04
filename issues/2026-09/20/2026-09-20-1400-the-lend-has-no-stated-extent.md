# The lend has no stated extent, and every route presupposed one

2026-09-20. M-declared-extents step 11, panel 169, the full panel: five seats and
a completeness critic.

## The decision

| | |
|---|---|
| date | 2026-09-20 |
| decision | **`spec § 13` states how long a lend is readable, and it comes first**; the caller-side rule for defect 068 is **not adopted as worded** and whatever lands is **stated over the IR** rather than the AST; **066 and 068 are two classes**; **R5, give the ownership away, is the route for 066**; and **defect 069's six-line repair is the milestone's blocking item** |
| reason | the ergonomist, reading only the spec, measured that the same rule **closes defect 068 under one reading of the extent and closes nothing under the other**, and wrote both and could not tell which compiles. The warden, on `--refresh`, priced the missing sentence at +27 real and found it living in `examples/gallery/13-lease.hero:3` — the teaching material — and absent from the document that IS the prompt |
| design.md § | §1.12, §4.19:2332-2338, §1.2, §1.11 |
| panel | 169, provisional |

## Two seats with isolated inputs found the same missing sentence

The spec-warden reads the repository; the llm-ergonomist reads nothing but
`spec/heroes-spec.md`. Neither knew what the other wrote. Both arrived at *the
document never says how long a lend is readable*, and the ergonomist called it
**the whole sitting**. The coordinator's first draft of the synthesis recorded
**neither**, which the completeness critic named as the sitting's own completeness
failure and which is corrected in the file rather than quietly fixed.

## The third hole in one sitting, and it is not a `@`

The ballot said *not re-assigned*. The engineer built the rule, attacked the
shape beside it, found `fill(@t)` and widened to *not written*. The critic then
ran the built module and found the third: **a lent root RE-DECLARED on the second
turn of a loop** — `check` 0, prints `72` then `1`, zero sanitizer lines, and the
built rule gives **0 diagnostics**. `judge_statement` returns on `.declare`.

**And the compiler says the two are one instruction.** `--dump-ir`: one slot for
the whole function, `store s <- $t20` inside the loop body, byte for byte what
068's own reproducer emits. **The AST distinguishes `declare` from `mutate`; the
IR does not, and the machine does not.**

So the rule moves layers. Over the AST the write set is an open set that went
from one to two to three today; over the IR it is closed and it is two. That is
`.claude/rules/module-shape.md`'s rule one level up: a fact about the value
cannot expire, a premise about the world expires silently.

## 066 and 068 are two classes, and the best-resourced FFI ever built says so

The historian's finding: 068 is not escape analysis, it is **exclusivity**, which
ships statically and caller-side in Rust, Swift, Ada, Austral and Nim, and was
never withdrawn. Panel 167's *nobody enforces retention statically* was about the
**callee**. And Java's FFM enforces the caller-side half at run time and refuses
the foreign half in its own javadoc — *no insight into the lifetime intended for
said region of memory by the foreign function*. **Defect 068 is FFM's exception
case; defect 066 is FFM's no-insight case.**

One correction the historian makes against itself: every surveyed language
**exempts raw pointers by name**. So a caller-side rule reaching `.ptr()` is a
deliberate departure, justified because in Rust the raw pointer is the escape
hatch and in Heroes it is the only road to C.

## The route nobody listed came from the seat that compiles

The ffi-pragmatist registered **R5, give the ownership away**: the author
allocates, C frees with the function the author names. Compiled against the real
`sqlite3.h` and `libsqlite3.dylib`, clean under `-Wall -Wextra -Werror` and the
sanitizers, and **it survives the replaced allocator that killed panel 168's
trailing header**, because the author's destructor is paired with the author's
allocator. Three sittings asked how Heroes can hand C bytes that survive; the
answer is that it should not.

**And one thing stands in front of it**: defect 069, filed three hours earlier as
a diagnostics defect. No spelling hands C a destructor.

## What the sitting refused, and one thing it un-filed

R4, withdrawing the field lend, is **vetoed**: it is C's only write path into
Heroes memory. R1, the handle route, is a real spelling that **closes zero of
066** — `acquires` counts, it does not order, and the live set is an exit check
rather than a use check. R3 is precedented as a marker and never as a proof.

Two defects are filed, both re-run before filing: **071**, the consume guard
emitted after the call it guards, and **072**, two allocator families collapsed
onto one handle type by defect 029's own refusal. **A third the ffi seat proposed
is not filed**: `spec § 13` already exempts *what a `ptr` points at* in those
words, so the `const void *` position is a named exemption rather than a hole.

## And the coordinator ran the critic in the wrong order

`/panel` § 3b puts the completeness critic before the synthesis. It ran after a
first draft. The file says so, and what it cost is visible in the diff: the
one-line resolution, the verdict table and five of the eight resolution items
changed, and two findings would have changed the draft before anybody read it.
