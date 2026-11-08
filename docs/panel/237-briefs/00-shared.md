# Panel 237, what every seat reads first

The sitting rules on three things batch 24 landed or held on 2026-11-08 (`proposal.md` beside this
file), on decision 2342 (https://github.com/heroes-lang/heroes/issues/2342). **Full panel**:
compiler-engineer, ffi-pragmatist, spec-warden, historian, the llm-ergonomist as a blind seat beside
a reader test, and a completeness critic over these briefs before any seat starts and over the
reports after. **Repaired on the critic's first pass** (`237-reports/critic-briefs.md`, 22
findings), before any seat or paid session started; what each repair took is in the commit that
made it.

## The tree, frozen

- Every seat measures the tree at **`163912031`** (the merge of `p236-land` into `b24-round`), which
  holds 2333's landing (`96d42c250`) and 2322's repair reverted (`ebdc8a28c`; the repair itself is
  `37fb22678`). Lane `p237` stands on it plus the sitting's own commits, which touch
  `docs/panel/237-*` alone.
- Each seat works in **its own detached worktree**, `.claude/worktrees/237-<seat>/`, made by the
  coordinator at `163912031`, and nowhere else: never in `p237`, the trunk, a lane or another seat's
  worktree. A `build/` it makes is its own. (The `/panel` skill names a scratchpad copy; this
  project's contract forbids a file outside the repository's root, so the worktrees stand under
  `.claude/worktrees/` as panels 235 and 236 did, a deviation the synthesis records.)
- **Build your own compiler** in your worktree: `clang -I runtime seed/heroes.c runtime/runtime.c -o
  heroes` (seconds), then `./heroes build selfhost/main.hero -o heroes-next` **in its own command**
  (minutes on this machine today, beside other work); judge with `./heroes-next`. At this commit
  `./heroes-next --version`'s second line reads `sources 0d7a6741345899d8` (the coordinator's build
  in `p237` and the critic's in its own worktree).
- The machine is shared with three lanes of batch 24. **Measure a cost in instructions**
  (`/usr/bin/time -l` prints *instructions retired*), never in seconds.

## What was measured while this brief was written

- **The refusal.** A folder holding this `main.hero`

  ```
  function main()
      x: i64 = 9223372036854775807
      print(x + 1)
  ```

  and a `stdint.h` of one line, `#include_next <stdint.h>`: `heroes build`, `heroes run` and
  `heroes build --emit-c` exit 1 with

  ```
  error[shadowed_header]: `stdint.h` stands in for the C library's `<stdint.h>`, which the C this compiler writes reads: a folder this build names is searched before the system's, so its file is read in the library's place
    at main.hero:1:1
  ```

  and the note *rename the file, or move it out of `.`: the folder is this program's own, one
  `--include` names, or one a package's `pkg-config` answers, and every `#include <stdint.h>` of the
  build reads what it holds*. The same `main.hero` with no `stdint.h` aborts at run, `panic: integer
  overflow` at `main.hero:3:11`, exit 134. **Of the five verbs, `check` alone exits 0** on the folder;
  the critic measured `heroes test` exiting 1 as well (`critic-briefs.md`).
- **The environment reaches the same class, and nothing refuses it** (the critic, findings 7 and 8):
  a `stdbool.h` defining `true` as 0 in a folder named by `CPATH` or `C_INCLUDE_PATH` makes
  `print(true)` print `false` at exit 0, where the same file beside `main.hero` is refused; and
  `CCC_OVERRIDE_OPTIONS='+-D__builtin_add_overflow...'` turns an `i64` sum past the top into
  `-9223372036854775808` at exit 0. Panel 056 names `CPATH` beside `--include` as a way to name a
  header folder; clang's `-v` prints both variables' folders in its search list.
- **The search orders on this Mac's clang** (the critic, finding 9): a folder named by `-isystem` is
  searched before the platform's, so its `stdint.h` is read: **`-isystem` is not a route**. Only
  `-idirafter` (both include forms) and `-iquote` (quoted includes only) keep the file out; and a
  group's header is emitted as an angle include, `#include <money.h>`, which `-iquote` does not
  serve (finding 10).
- **What the refusal and a reorder reach** (finding 11): the runtime's opening reads 36 library
  headers; 0 files of the 85 `-I` folders of the 507 packages `pkg-config` lists here bear one of
  their names, so the refusal stops no package on this Mac; 0 tracked files outside `runtime/` bear
  one (`git ls-files`). But 3 headers in 2 package folders share a name with a platform header
  (poppler's `Form.h`; GNU readline's `readline/readline.h` and `readline/history.h` under
  `fluidsynth`, where the SDK's `readline/readline.h` is libedit's), so an `-idirafter` route for
  package folders would hand a binding another library's header. Of the 23 `-D` names the packages
  here pass, none is a word the guard keeps, and none is `_REENTRANT` or `_FILE_OFFSET_BITS`.
- The refusal carries no `Fix` (`selfhost/cli/shadowed.hero:237-246`, the diagnostic built there).
- `shadowed_header` is held by `tests/harness/suite_special.hero`'s
  `a_library_header_in_a_named_folder_is_refused` (line 647, its comment from 638), and by no golden
  case annotated in its source: `grep -rln shadowed_header tests/golden | wc -l` prints 0.
- The specification names no diagnostic code: `grep -c 'error\[' spec/heroes-spec.md` prints 0.
- **Feature 974** (https://github.com/heroes-lang/heroes/issues/974, open on M-package-manager):
  *a bindings module is invisible to `heroes check`*, because `check` never runs clang; it points at
  panel 082's R3, which refused running the program inside `check` on a measured cost. Question 5
  meets it.
- **The fence's readings**, `./heroes-next measure` over drafts that change only the fence's `sqlite3`
  lines (the vendored tables; the real row needs `--refresh`):

  | the fence's `sqlite3` lines | claude-legacy | cl100k_base |
  |---|---|---|
  | A, as they stand | 8181 | 8327 |
  | B, `sqlite3_close(db: Db consumes) -> i64 when 0` | 8183 | 8330 |
  | C, `sqlite3_close_v2` in the `acquires` mark and the binding, no `when` | 8187 | 8333 |
  | D, `acquires sqlite3_close \| sqlite3_close_v2`, the close `when 0`, and a `sqlite3_close_v2` line | 8210 | 8357 |

  The real row of A, pinned by the last refresh: **10904**, `claude-opus-5`, 2026-11-08. D is the
  form the specification's own prose teaches at line 459 (the critic, finding 12).
- **What C moves beyond tokens** (finding 3): under C, `tests/harness/suite_special.hero:440`'s
  program, which builds the fence, fails `error[unknown_name]` on `sqlite3_close`;
  `tests/harness/suite_spec.hero`'s `fragments` pins the fence byte for byte; and both SQLite
  programs of `examples/` bind the close `when 0` since defect 2291.
- **design.md §4.19's copy** (`docs/design.md:2577-2582`) is not the fence: it binds
  `sqlite3_open(path: cstr, ...)` with no `lent` and no `constant SQLITE_OK`, and with `special`'s
  `main` it is refused today, `error[lend_kept]`, exit 1 (finding 2). It needs `lent` whichever
  reading wins.
- **The thesis contexts** (finding 4): the registered prediction (`tests/thesis/held-out/prediction.md:131`)
  names only the `spec` context, at `a4e09e061`; lines 113 to 119 list the two variant contexts
  (`tests/thesis/contexts/spec-with-one-worked-diagnostic.md`, `spec-without-productions.md`) as
  exploratory, and `tests/thesis/contexts/README.md` alone pins their sha256. Each holds the
  unmarked line once (`grep -c`).

## The questions

1. **The shape of item 1.** Refuse a file that stands in for a library header, as landed; or change
   where the library's headers are found (`-idirafter`, or `-iquote` with the group's include
   emitted quoted; `-isystem` is no route, above); or both. **And the environment**: whether the
   compiler should clear or count `CPATH`, `C_INCLUDE_PATH`, `CPLUS_INCLUDE_PATH`, `OBJC_INCLUDE_PATH`
   and `CCC_OVERRIDE_OPTIONS` for clang, or refuse a build where they are set. Which closes the class
   (a wrong value at exit 0 where the language promises an abort, design.md §1.12) with the fewest
   correct programs refused or changed, on the three platforms, measured; and what each reorder
   changes beyond the opening's 36 headers.
2. **What it refuses that is correct.** A program, a `--include` folder or a package that holds a
   header named like one the runtime's opening reads, for a reason of its own; counted over the tree
   and the packages here (above: none found), and argued for Linux and Windows.
3. **The class.** `shadowed_header` as its own code, or an existing one; whether item 2's refusal of
   a package's `-D` or `-U` is the same class as a header standing in, or stays `ffi_package`; where
   the environment's refusal, if any, belongs.
4. **The words.** Whether the message and note carry everything needed to fix the program without
   opening another file (design.md §4.17); whether a `Fix` is owed, and tagged what.
5. **`check` and the other four verbs disagree**: `check` exits 0 on a program `build`, `run`,
   `--emit-c` and `test` refuse at exit 1. Is that a gap to close for this refusal (a tool asking
   `check --json` is told nothing), feature 974's gap answered at last, or the line between the
   language and the C boundary, stated where; and whether a refusal `check` cannot see and the
   environment can bypass is the right line at all.
6. **Where it is held**: a golden case annotated in its source, and in which form (`unsupported`
   builds with exit 1 expected; a folder case), beside `special`'s check.
7. **Item 3, the specification's line**: A, B, C or D, priced on the real tokeniser and in what each
   moves; which teaches a model the binding a refused close needs, **and whether the lesson carries
   to a library with no `_v2`** (the reader test's second task); design.md §4.19's copy; and the two
   thesis contexts, by their real status above.
8. **design.md §4.19's text** for items 1 and 2, and for the environment if it is ruled on.

## Rules every seat keeps

- **Every number in your report comes from a command you ran while writing it**, the command beside
  it; a negative sentence (*X does not exist*, *no program does Y*) is run or written as a question.
  A figure in this brief is the coordinator's or the critic's: re-run it before you rest anything on
  it.
- **No paid run your brief does not name**: no `claude -p` session, API call or cloud run. One you
  find worth running goes in your report with its size, and the coordinator decides.
- **No credential printed**: never `gh auth token`; `gh auth status` masks it. Only `gh` reads
  (`issue view`, `issue list`, `search`), never a write.
- **No file outside `/Users/joseph/Temp/heroes/heroes-lang`**: no `/tmp`, no home directory.
- **A program that may not end** is built and its binary run under `timeout`, its output to
  `/dev/null` or through `head -c 65536` into a file; never `timeout heroes run`.
- **Write your report as you go** to `docs/panel/237-reports/<seat>.md` **in your own worktree**
  (the coordinator copies it into the sitting's lane), so a seat stopped early leaves what it had.
- Your report ends with: your verdict (approve, object, or veto with its condition), the design.md
  section it rests on, the cost or delta you measured, **one falsifiable prediction** with the
  milestone or run that scores it, and the condition that would change your verdict.
- No em dashes.
