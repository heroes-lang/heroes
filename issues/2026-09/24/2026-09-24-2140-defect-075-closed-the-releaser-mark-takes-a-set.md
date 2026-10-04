---
kind: defect
area: check
milestone: none
filed: 2026-09-23
commit: fb0b7cb6ee473f6a9e69d8c7363e179dc63f83bd
github: none
---


# Defect 075 closed: the releaser mark takes a set, and a release it did not name is stopped before C runs

2026-09-24, M-agreed-retention step 10, in lane `5ebbe8ba`, merged `9f4f9b85`:
panel 175's route A, landed as panel 176's item 1 on the sitting's provisional
resolution. Found at step 1 of this milestone, measuring the second item at the
shape beside it.

- [x] **075 — `acquires` names the call that ends a handle's life, and a program that ends it with another is `check` 0 and `run` 0** | the named releaser is read for existence and never at the call that gives the handle back, and the live set keeps an address and nothing else | **closed 2026-09-24**, M-agreed-retention step 10 (`5ebbe8ba`, merged `9f4f9b85`) | `selfhost/check/acquiring.hero` · `runtime/heroes_runtime.h:205` · `spec § 13`

    **Origin:** 2026-09-23, M-agreed-retention step 1, measuring the milestone's
    second item at the shapes beside it. The contradiction it names across two
    modules turned out to be admitted inside ONE, which is where the defect is.

    **The reproducer**, against the platform's real `stdio.h`:

        extern "stdio.h"
            record File tag __sFILE
            function popen(command: cstr lent, mode: cstr lent) -> File acquires pclose
            function pclose(stream: File consumes) -> i32
            function fclose(stream: File consumes) -> i32

        function main()
            f = popen(command: "true".cstr(), mode: "r".cstr())
            rc = fclose(stream: f)
            print("fclose on a popen stream: ", rc)

    `check` 0, and `run` 0 five times out of five on **all four platforms**:
    Darwin arm64 as written, both Linux legs with `tag _IO_FILE`, Windows x86-64
    with `_popen`, `_pclose` and `tag _iobuf`. Under `--sanitize` on Darwin it
    builds, runs at 0 and writes zero bytes. What `fclose` on a `popen` stream
    does inside each C library is UNRUN here; what is measured is that nothing
    in Heroes objected, on any platform. The same shape through an
    `@out` parameter (`h_open_out(@out: H acquires h_close)`, then
    `h_close2(x: h)`) and across two modules is `check` 0 and `run` 0 as well.

    **The cause, read rather than inferred.** `unread_releaser` asks whether the
    name after `acquires` is an `extern` of this module taking the handle
    `consumes`, and nothing asks it again. `hero_handle_acquired` and
    `hero_handle_consumed` take `const void *` and nothing else, so the set
    balances whichever releaser runs.

    **What is owed.** Spec § 13 says the call *names the one that ends it, which
    the program owes it*, and panel 148 adopted the named form over the bare word
    because the bare word *leaves the mismatched-deallocator class open* (its
    `What conservative would have been`). Measured today, the named form leaves
    it open too.

## The repair

`acquires` names a SET of releasers, `acquires sqlite3_close | sqlite3_close_v2`,
and the set travels with the address: `runtime/parts/alloc.c`'s live set holds,
beside each address, the releasers its acquiring mark named, as one C string
compared by content token by token, because each module is its own translation
unit and only the name is one identity across them. A `consumes` call the set
did not name is stopped before C runs, with a line that says what the two
declarations say and nothing about why they disagree. A call that consumes and
acquires in one — `freopen`, `realloc` — may take back a handle whose set shares
a name with its own mark, which is panel 175's `as`; panel 176's `transfers`
names that on purpose and lands next. The runtime ABI moves 22 → 23.

The parser reads `ident { "|" ident }` and hands on one span; `handles.hero` is
the one home that splits it, for the checker (every name must resolve, the
diagnostic on the one that does not), the two printers (`a | b` back whatever
the author typed; `fmt` is idempotent on it, measured) and the emitter (`a|b`
for C). The checker's lookup stays per module, as its message says.

Why route A alone was refused at panel 175 and lands now: one name aborted a
correct `sqlite3_close_v2`, and nine real pairs of releasers are
interchangeable for one acquisition; the set is what makes the check by name
true of real libraries.

## The spec

§ 13's `acquires` sentence gains *It may name several, `acquires sqlite3_close |
sqlite3_close_v2`, and any one of them ends the life; giving the handle to a
`consumes` call the mark did not name aborts before C runs*, and the `Member`
and `CParam` productions gain `{ "|" ident }`. Priced on the real instrument:
6282 → **6344** vendored, 8361 → **8443** real (`claude-opus-5`, 2026-09-24),
digest `3cb9c9345d5a33fc`; ledger row 6344, paid on the author's word by three
registered predictions.

## The measurements

| golden | before | after |
|---|---|---|
| `run/fixedbugs-a-release-the-mark-did-not-name-is-stopped-before-c` (075's shape over a header) | check 0, run 0, `C ran` | 134 before C, the crossing line, `C ran` never printed |
| `run/handle-set-any-releaser-named-ends-the-life` (`sqlite3_close_v2`'s shape) | refused by `unread_releaser` on the second name | 0 |
| `run/handle-set-a-call-that-also-acquires-pays-its-own-mark` (`freopen`'s shape) | 0 | 0 |
| `check/unread-releaser-in-a-set` | — | `unread_releaser` on the one name that does not resolve |

Lane gate, with the compiler rebuilt from the regenerated seed, whose
fixpoint holds byte for byte at ABI 23: the compiler's own tests 677/677, the
net's 167/167, spec 20/0, grammar 9/0, special 10/0, annotations 175/0, check
136/0, fixes 25/0, unsupported 15/0, emission 514/0, run 149/0, corpus 55/0,
lines 150/0, warnings 210/0, determinism 179/0, canonical 2/0, layout 2/0,
order 3/0, records 24/0. `parse/members.hero` crossed § 11's 300 lines with
the set and was split along the seam its own comment named — the marks now
live in `parse/marks.hero`, which both `members` and `tails` read — and the
gate was rerun from the seed regenerated after the split. On the merged
trunk, compiler rebuilt from the merged seed: spec 20/0, grammar 9/0,
annotations 175/0, check 136/0, emission 514/0, run 149/0, records 24/0,
canonical 2/0, layout 2/0, the compiler's tests 677/677.

**Linux arm64 and x86-64**, each with the compiler built from the regenerated
seed: the four goldens as in the table, plain and under `--sanitize`, and the
run suite 145/0. **Windows**: the same four, plain and under `--sanitize`, and
the run suite 143/0, in 422 s.
