# Panel 237, what every seat reads first

The sitting rules on three things batch 24 landed or held on 2026-11-08 (`proposal.md` beside this
file), on decision 2342 (https://github.com/heroes-lang/heroes/issues/2342). **Full panel**:
compiler-engineer, ffi-pragmatist, spec-warden, historian, the llm-ergonomist as a blind seat, and
a completeness critic over these briefs before any seat starts and over the reports after.

## The tree, frozen

- Every seat measures the tree at **`163912031`** (`git rev-parse --short HEAD` in lane `p237`, the
  merge of `p236-land` into `b24-round`), which holds 2333's landing (`96d42c250`) and 2322's
  repair reverted (`ebdc8a28c`; the repair itself is `37fb22678`).
- Each seat works in **its own detached worktree**, `.claude/worktrees/237-<seat>/`, made by the
  coordinator at that commit, and nowhere else: never in `p237`, the trunk, a lane or another seat's
  worktree. A `build/` it makes is its own. (The `/panel` skill names a scratchpad copy; this
  project's contract forbids a file outside the repository's root, so the worktrees stand under
  `.claude/worktrees/` as panels 235 and 236 did, a deviation the synthesis records.)
- **Build your own compiler** in your worktree: `clang -I runtime seed/heroes.c runtime/runtime.c -o
  heroes` (seconds), then `./heroes build selfhost/main.hero -o heroes-next` **in its own command**
  (about two minutes on this machine today, beside other work); judge with `./heroes-next`. At this
  commit `./heroes-next --version`'s second line read `sources 0d7a6741345899d8` in lane `p237`.
- The machine is shared with three lanes of batch 24. **Measure a cost in instructions**
  (`/usr/bin/time -l` prints *instructions retired*), never in seconds.

## What was measured while this brief was written (lane `p237`, `heroes-next` above)

- A folder holding `main.hero` (an `i64` sum past the top) and a `stdint.h` of one line,
  `#include_next <stdint.h>`: `heroes build` and `heroes run` exit 1 with

  ```
  error[shadowed_header]: `stdint.h` stands in for the C library's `<stdint.h>`, which the C this compiler writes reads: a folder this build names is searched before the system's, so its file is read in the library's place
    at main.hero:1:1
  ```

  and the note *rename the file, or move it out of `.`: the folder is this program's own, one
  `--include` names, or one a package's `pkg-config` answers, and every `#include <stdint.h>` of the
  build reads what it holds*; `--emit-c` exits 1 with the same; **`heroes check` exits 0**. The same
  program with no `stdint.h` aborts, `panic: integer overflow`, exit 134.
- The refusal carries no `Fix` (`selfhost/cli/shadowed.hero:237-246`, the diagnostic built there).
- `shadowed_header` is held by `tests/harness/suite_special.hero`'s
  `a_library_header_in_a_named_folder_is_refused` (line 647, its comment from 638), and by no golden
  case annotated in its source: `grep -rln shadowed_header tests/golden | wc -l` prints 0.
- The specification names no diagnostic code: `grep -c 'error\[' spec/heroes-spec.md` prints 0.
- The token count, `./heroes-next measure`, vendored tables (the real row needs `--refresh`, below):

  | the fence's two `sqlite3` lines | claude-legacy | cl100k_base |
  |---|---|---|
  | A, as they stand | 8181 | 8327 |
  | B, `sqlite3_close(db: Db consumes) -> i64 when 0` | 8183 | 8330 |
  | C, `sqlite3_close_v2` in both the `acquires` mark and the binding, no `when` | 8187 | 8333 |

  The real row of A, pinned by the last refresh: **10904**, `claude-opus-5`, 2026-11-08.
- design.md §4.19's example (`docs/design.md:2577-2582`) binds `sqlite3_close(db: Db consumes) ->
  i64` too. The two thesis contexts that copy the specification
  (`tests/thesis/contexts/spec-with-one-worked-diagnostic.md`, `spec-without-productions.md`) each
  hold the unmarked line once (`grep -c`), and `tests/thesis/contexts/README.md` pins their sha256
  as the frozen inputs of a registered measurement.

## The questions

1. **The shape of item 1.** Refuse a file that stands in for a library header, as landed; or change
   where the library's headers are found so the program's folders never come first for them (clang's
   `-iquote`, `-idirafter`, `-isystem` orders, or another route a seat finds); or both. Which closes
   the class (a wrong value at exit 0 where the language promises an abort, design.md §1.12) with the
   fewest correct programs refused, on the three platforms, measured.
2. **What it refuses that is correct.** A program, a `--include` folder or a package that holds a
   header named like one the runtime's opening reads, for a reason of its own; counted over the tree
   (`examples/`, `tests/golden/`) and over the packages this machine's `pkg-config` lists.
3. **The class.** `shadowed_header` as its own code, or an existing one; whether item 2's refusal of
   a package's `-D` or `-U` is the same class as a header standing in, or stays `ffi_package`.
4. **The words.** Whether the message and note carry everything needed to fix the program without
   opening another file (design.md §4.17); whether a `Fix` is owed, and tagged what.
5. **`check` and `build` disagree**: `check` exits 0 on a program `build` refuses at exit 1. Is that
   a gap to close for this refusal (a tool that asks `check --json` is told nothing), or the line
   between the language and the C boundary, stated where.
6. **Where it is held**: a golden case annotated in its source, and in which form (`unsupported`
   builds with exit 1 expected; a folder case), beside `special`'s check.
7. **Item 3, the specification's line**: B, C, or A as it stands, priced on the real tokeniser;
   which teaches a model the binding a refused close needs; design.md §4.19's copy; and the two
   thesis contexts, frozen or not.
8. **design.md §4.19's text** for items 1 and 2, the sentences the landing owes.

## Rules every seat keeps

- **Every number in your report comes from a command you ran while writing it**, the command beside
  it; a negative sentence (*X does not exist*, *no program does Y*) is run or written as a question.
  A figure in this brief is the coordinator's: re-run it before you rest anything on it.
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
