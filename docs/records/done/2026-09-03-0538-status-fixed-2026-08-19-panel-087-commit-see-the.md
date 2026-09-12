- [x] **002 — The guard the port kept and the language took away** | Date: 2026-08-19, opening M-separate-compilation. **Found by running the baseline net before touching anything**, which is the only reason it was found at all: it had been in the tree for one commit and CI would have reported it as a pass. | **Status: fixed 2026-08-19** (panel 087, commit `f343fdd`) — see § The repair at the end of this file. The sentence that stood here said *"the repair is a language change (`read_file`'s contract), so it is a panel path"*; the panel path was right and the prediction was wrong, and that is the most useful thing this file records. The finding and the fix have their own commits, as this line asked. | moved here 2026-09-03 from `docs/defects/002-the-guard-the-language-took-away.md` by author instruction — the directory is gone, its text is below, unedited except that its `## ` headings became bold leads | Severity: **★★★** — a legal program is killed. And the killed program is the project's own test net.


  Date: 2026-08-19, opening M-separate-compilation. **Found by running the baseline
  net before touching anything**, which is the only reason it was found at all: it
  had been in the tree for one commit and CI would have reported it as a pass.

  **Status: fixed 2026-08-19** (panel 087, commit `f343fdd`) — see § The repair at
  the end of this file. The sentence that stood here said *"the repair is a language
  change (`read_file`'s contract), so it is a panel path"*; the panel path was right
  and the prediction was wrong, and that is the most useful thing this file records.
  The finding and the fix have their own commits, as this line asked.

  Severity: **★★★** — a legal program is killed. And the killed program is the
  project's own test net.

  **The symptom.**

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

  **The reproducer, five lines and no harness.**

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

  **The cause.**

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

  **Why it fired now, and what the trigger says.**

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

  **The finding, which is bigger than the trigger.**

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

  **What the repair has to decide.**

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

  **The repair — 2026-08-19, panel 087, commit `f343fdd`.**

  **The second answer won, and it cost five lines of C and zero spec tokens.** The
  panel shrank the proposal by two thirds: `spec:179` already types the built-in as
  fallible, so the document was never wrong and the runtime was the thing that lied
  — paying tokens to *describe* this behaviour would have written a defect into the
  spec. §12's rule ("the compiler has the bug") applied to the one place the spec
  cannot see.

  `runtime/parts/os.c:130` asks `hero_utf8_valid` before converting, releases the
  buffer, sets the new `HERO_OS_NOT_TEXT` (`runtime/hero_os.h:51`) and returns the
  empty str — keeping `hero_os.h:44`'s promise that on anything but OK the string
  owns nothing. **The abort inside `hero_str_from_bytes` is deliberately untouched**:
  `hero_str_chars` (`runtime/parts/text.c:87`) takes one byte off an invalid
  sequence and depends on that abort for the language-wide well-formedness
  invariant, so a weaker conversion would have traded this defect for a worse one.
  That was the ffi-pragmatist's uncast veto and it decided the shape. The case is
  `tests/golden/run/fixedbugs-read-file-on-bytes-that-are-not-text.hero`, which
  builds its own fixture through `extern "stdio.h"` because **no Heroes program can
  create a file that is not text** — `write_file` takes a `str`, and a `str` is
  UTF-8 by definition.

  Re-measured 2026-08-23, in the session that writes this paragraph rather than
  recalled from the fix's own commit:

  - the five-line reproducer above → `fail: read_failed`, exit **0** (was `panic:
    hero_str_from_bytes: not well-formed UTF-8`, exit 134)
  - the guard at `suite_records.hero:148` is **reachable at last**: the call that
    killed the process returns `.err`, which is the only thing that branch tests

  **What the fix did not close, and both are queued in `docs/debrief/DECIDE.md`**
  (that path died on 2026-08-26 — see the note at the end of this file):
  the status is collapsed into `read_failed`, so the message still says *could not
  read* about a file that read perfectly well (naming it `file_not_text` is one
  `constant` line and the two compiling seats disagreed, so it is the author's);
  and three of panel 087's four doors are untouched — `args()` has no error channel
  at all, `spec:229` sends a binding author through a null test and then kills them
  anyway, and a "is this pointer safe" predicate is categorically unbindable
  because every `cstr` argument is wrapped in `hero_cstr_nonnull`.

  **Re-read 2026-08-26 — the defect holds, the paragraph above has expired.**

  Asked whether this file still earns its place. **The defect does not need
  re-arguing: the repair still holds**, run again today rather than recalled — the
  five-line reproducer at the top of this file, against the same tracked JPEG,
  prints `fail: read_failed` at exit **0**. What has moved is the paragraph
  immediately above, and each of its three claims was checked against the record and
  the tree, not remembered.

  - **The path is dead.** `docs/debrief/` no longer exists: the lists were re-cut on
    2026-08-26 by what an item *is* rather than where it was written, and the live
    ones are `docs/work/DECIDE.md` (open decisions), `docs/work/SCHEDULED.md` (work
    with a milestone) and `docs/work/DONE.md` (the record). CLAUDE.md §3 carries the
    reason.
  - **Two of the three doors are shut, both on 2026-08-24.** `args()` got its error
    channel — `args_checked() -> [str?]`, the shape the ffi-pragmatist compiled, now
    at `spec:186` and `selfhost/library_source.hero:226` (`DONE.md:620`). And
    `spec:229` stopped sending a binding author through a null test: the sentence
    *"test `c == nullptr` first, because converting one aborts"* is **deleted**, −15
    tokens, the named removal that funded panel 089 (`DONE.md:640`) — and the
    killing it warned about is a named abort now, because every `cstr` argument
    leaves through `guard_cstr_arguments` (`selfhost/emit_ops.hero:181`). Only the
    third is still open as written: a *"is this pointer safe"* predicate remains
    categorically unbindable.
  - **The `read_failed` collapse is still real and still the author's call.**
    `runtime/hero_os.h:46` says in the live header that the Tier-2 wrapper collapses
    `HERO_OS_NOT_TEXT`, and the reproducer above still says *could not read* about a
    file that read perfectly well. **But the item is in no open list**: `DONE.md:638`
    struck it from panel 090's ballot on 2026-08-25 with the words *"It stays open as
    scheduled work, not as a decision"*, and `SCHEDULED.md` does not carry it — ten
    open items on 2026-08-26 and none is this one. Ticked in the record and absent
    from the work is exactly the shape CLAUDE.md §3 was amended to prevent, and it is
    recorded here because this file is where somebody looking for that decision will
    come.
