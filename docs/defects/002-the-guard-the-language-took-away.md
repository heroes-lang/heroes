# 002 — The guard the port kept and the language took away

Date: 2026-08-19, opening M-separate-compilation. **Found by running the baseline
net before touching anything**, which is the only reason it was found at all: it
had been in the tree for one commit and CI would have reported it as a pass.

**Status: open.** The repair is a language change (`read_file`'s contract), so it
is a panel path — CLAUDE.md §4. This file is the record of the finding; the panel
and the fix get their own commits.

Severity: **★★★** — a legal program is killed. And the killed program is the
project's own test net.

## The symptom

```
$ ./heroes run tests/harness/main.hero -- ./heroes
  ...
  warnings: 107 passed, 0 failed
panic: hero_str_from_bytes: not well-formed UTF-8
$ echo $?
134
```

Twelve suites green, then SIGABRT in the thirteenth (`records`), no summary line,
no `report.verdict`. `heroes run tests/harness/main.hero -- ./heroes records`
reproduces it alone in seconds.

## The reproducer, five lines and no harness

```
function main()
    got = read_file(path: "site/public/images/giuseppe-arici.jpg")
    match got
        .ok text => print("ok, bytes: " + text.len().to_str())
        .err e => print("fail: " + e.code)
```

`panic: hero_str_from_bytes: not well-formed UTF-8`, exit 134. **Neither arm
runs.** `read_file` is declared `-> str?` at `spec:179`, the program handles both
halves of the fallible, and the process dies before either is reached.

## The cause

`runtime/parts/os.c:114`, in `hero_file_read`:

```c
HeroStr text = hero_str_from_bytes(buffer, (int64_t)got);
```

unguarded. Three statuses exist — `HERO_OS_OK`, `HERO_OS_NOT_FOUND`,
`HERO_OS_FAILED` (`runtime/hero_os.h:39-41`) — and the fourth program state,
*these bytes are not text*, has no code. `hero_str_from_bytes` aborts on invalid
UTF-8 (`runtime/parts/str.c:244`), so the failure that has no status also has no
survivor.

The runtime's own header already knows this is a state a caller should be able to
branch on. `runtime/heroes_runtime.h:120` says `hero_str_from_bytes` is
*"**Exported so a binding can branch instead of dying**"* — and the one entry
point in the runtime that reads a file the program did not write does not branch.

## Why it fired now, and what the trigger says

`abaa7ca` ("site: the author has a face") added
`site/public/images/giuseppe-arici.jpg` — **the first non-text file this
repository has ever tracked**. Measured over `git ls-files`: 1373 tracked files,
exactly one of which is not valid UTF-8, and it is that one.

`tests/harness/suite_records.hero` walks every git-tracked file that is not a
dated record and reads it, looking for a numbered milestone identifier
(CLAUDE.md §14). The walk asks git which files are the project's — deliberately,
since 2026-08-19 — so a tracked JPEG is in scope by design and correctly so: a
list of exclusions is a premise about which junk exists, and that premise had
already expired once.

## The finding, which is bigger than the trigger

**The port kept the guard and the language took away its ability to fire.**

`archive/bootstrap-rs/heroes-cli/tests/milestones.rs:183` — the Rust twin of this
walk:

```rust
let Ok(text) = std::fs::read_to_string(file) else { continue };
```

Rust's `read_to_string` returns `Err(InvalidData)` on non-UTF-8 bytes, so the
Rust instrument **skipped** a file it could not decode, by construction, and
would have survived this commit untouched.

`tests/harness/suite_records.hero:148-150` — the Heroes port of the same lines:

```
text = read_file(path)
if text.is_err()
    continue
```

The branch was transcribed faithfully. It is in the right place, it is written
correctly, and **it can never be taken**, because the failure it exists for kills
the process instead of arriving as a value. Two call sites (`:148`, `:175`) carry
the same dead guard, mirroring the Rust's two (`:183`, `:262`).

This is the shape CLAUDE.md §11 warns about from the other side: a premise that
expires silently while the code around it goes on reading as correct. Here the
code is not merely still readable — it is still *right*. What died is the
language's ability to honour it.

## What the repair has to decide

Not "add a status" — that is the implementation. The question is what
`read_file` **means** when the bytes are not text, and it has two answers with a
real argument on each side:

- **abort** (today). A `str` is UTF-8 by definition (`spec:51`), so a file that
  is not text is not a `str`, and the language aborts elsewhere for exactly this
  kind of category error — an out-of-range index, a `nan` comparison, an
  overflow.
- **`fail("file_not_text", …)`** — `spec:179` already types the built-in as
  fallible, §Failure says codes are stable snake_case strings, and §1.12 makes
  robustness a goal of the language rather than a preference. A program cannot
  ask whether a file is text without dying, so the language cannot express a walk
  over a directory it did not create — and a directory it did not create is the
  only kind a real program walks.

The second is also the only repair that lets the harness's filter **ask the
value** rather than a premise about the world (CLAUDE.md §11): every alternative
available today — an extension deny-list, git's own `-text` heuristic — answers a
different question. Git's was measured: it reports three tracked files as binary,
and two of them are valid-UTF-8 `.hero` fixtures with raw carriage returns
(`tests/golden/check/{raw,stray}-carriage-return.hero`), so filtering on it would
drop `.hero` files from a walk whose job is to read them.

That is a panel question and it goes to the panel as one.
