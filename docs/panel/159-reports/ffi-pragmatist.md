# Panel 159 — ffi-pragmatist report

- `verdict`: **object** — on R3's framing, not on the refusal it documents.
  No veto: nothing here touches the C ABI.
- `section`: design.md §1.11 (the founding constraint), §4.19 (FFI).
  **The objection's ground is a silence design.md does NOT cover** and I say so
  explicitly below.

---

## First: the brief's claim, checked

The brief says `sort`'s direction and `xs[i] @ v` "have no C boundary in them as
far as the coordinator can see", and asks me to say if that is wrong.

**Half right. R1 is clean. R2 is wrong, and it is wrong in the direction that
costs a binding author most.**

### R1 (`sort`'s direction) — no C boundary. CONFIRMED by compiling.

```
extern "sqlite3.h" link "sqlite3"
    record Db tag sqlite3
    function sqlite3_open(path: cstr, @out: Db acquires sqlite3_close) -> i64
    function sqlite3_close(db: Db consumes) -> i64
function main()
    dbs: [Db] @ []
    print(dbs.sort().len())
```

```
error[unordered_element]: `sort` orders arrays of numbers, `str` and `bool` — `[Db]` is none of those
```

A foreign handle, a `ptr` or an extern record **cannot reach `sort` at all**, so
its direction cannot silently reorder anything a binding produced. The
coordinator's claim holds. `heroes build` exit 1, measured
(`scratchpad/ffi159/f/sorthandles.hero`).

### R2 (`xs[i] @ v`) — the claim is WRONG. It is on my boundary twice.

Two forms, both compiled today, **neither in `spec/heroes-spec.md`**:

```
# 1. a C out-parameter written STRAIGHT INTO an array element
dbs: [Db] @ []
dbs @ dbs.push(nullptr)
_ = sqlite3_open(":memory:".cstr(), @dbs[i])      # build exit 0
```
```
# 2. `xs[i] @ v` with a FOREIGN HANDLE element
d: Db @ nullptr
_ = sqlite3_open(":memory:".cstr(), @d)
dbs[0] @ d                                        # build exit 0
_ = sqlite3_close(dbs[0])                         # build exit 0, run exit 0
```

And the acquire/consume ledger **tracks through the array element**: form 1 with
no close runs to `exit 134`, `panic: 1 C handle(s) never given back`. So the
language already implements the N-connection-pool shape correctly — and the
document shows a binding author neither half of it.

§4.19's own ladder needs this at step 3: a prepared-statement cache is
`[Statement]` filled through `@stmts[i]` out-parameters. The spec gives
`advance(@l)` (§5, a bare ident) and `m[k] @ v` (§10, a map) and nothing else.
The grammar already derives both — §5 `Place = ident { "." ident | "[" Expression "]" }`
and §5 `Arg = [ ident ":" ] [ "@" ] Expression` — so R2's price is prose, not
semantics. **The llm-ergonomist built a map of taken keys because the prose
stopped short; a binding author builds a parallel `{i64: Db}` for the same
reason, and a map of handles is a leak the ledger will find at 134 in
production.**

---

## The one that is mine: `main` and the process boundary

### What a Heroes program's exit status carries today — MEASURED

Built and run in a copy (`heroes 0.2.0`, the tree's own binary + `runtime/`),
statuses captured directly, never through a pipe.

| program | exit | stdout | stderr |
|---|---|---|---|
| `print("ok")` | **0** | `ok` | — |
| `xs[7]` out of range | **134** | — | `panic: array index out of range` |
| `exit(code: 3)` | **3** | `about to exit` | — |
| `exit(code: 200)` | **200** | — | — |
| `exit(code: 256)` | **0** | — | — |
| `exit(code: -1)` | **255** | — | — |
| `exit(code: 4294967296)` | **0** | — | — |
| `match step() … .err e => print(e.msg)` | **0** | `failed: …` | — |
| same + `exit(code: 1)` | **1** | `failed: …` | — |
| `step().must()` on a failure | **134** | — | `panic: .must() on an error: nope: …` |

`heroes run` forwards the child's status faithfully: 3 → 3, 134 → 134, 0 → 0.

### The generated `main` — read, not reasoned about

`selfhost/emit/decls.hero:268-279` emits exactly this and nothing else:

```c
int main(int argc, char **argv) {
    hero_args_set(argc, argv);
    h_<module>_main();
    hero_runtime_check_leaks();
    return 0;
}
```

`return 0` is a **literal**. There is no path from a Heroes value to the
process's status except `hero_exit` (`runtime/parts/os.c:337`, `exit((int)code)`).
So the refusal's sentence is exact about `main`'s *return*, and the runtime has
no machinery it is declining to use.

### The collision with `.claude/rules/cli-surface.md` — MEASURED, and real

That file's contract is *"exit 0 clean, exit 1 the input has diagnostics, exit 2
the tool could not run"*, and it is the **compiler's**. But `heroes run` is the
compiler:

```
heroes run p/e1.hero    → exit 1, stdout "program failed",        stderr EMPTY
heroes run p/bad.hero   → exit 1, stdout empty, stderr "error[unknown_name]: …"
heroes run p/e2.hero    → exit 2, stdout "program failed harder", stderr EMPTY
```

A CI leg reading only the status cannot tell **"this program reported failure"**
from **"this program does not compile"**. Both are 1. `exit(code: 2)` collides
with *the tool could not run*. **Nothing in `spec/heroes-spec.md`,
`docs/design/design.md` or `cli-surface.md` states a rule for a compiled
program's status.** The spec's only word is §11's `exit(code: i64) (ends the
program)` — one parenthesis, no status semantics, no truncation.

**I say this explicitly, as the brief requires: design.md does not cover this.**
Grepped `exit status` / `exit code` in `spec/heroes-spec.md`: zero hits.

### The sweep the brief asked for — 18 of 20 binding programs cannot fail

`examples/` holds **55** directories; **21** `.hero` files carry a top-level
`extern`, across **20** binding programs. Of those 20, **exactly 2 call `exit`
at all** — `examples/ledger` (3) and `examples/tally` (1). The other **18 always
exit 0**.

And the flagship proves the cost. `examples/sqlite/main.hero:99-110` — the §4.19
ladder's own example, the one a reader meets first — handles a failed
`sqlite3_open` by printing and returning. I compiled the shipped file unchanged
and a one-token variant pointing at an unopenable path:

```
./sq.bin      exit=0   stdout: "rows: 3" / "longest: 6"
./sqfail.bin  exit=0   stdout: "cannot open the database"
```

**Identical status, and the notice is on stdout**, so `2>/dev/null` does not
separate them either. That is `error[main_returns]`'s sentence enacted: *a
program reports failure by what it prints*. A shell reads a success.

### What a `main -> ()?` shim would cost — I wrote it and clang took it

Taking the real emitted C, retyping `h_handled_main` to return the `()?` struct
(`{ int64_t tag; union { HeroFailure err; } as; }`), and hand-writing the shim:

```c
int main(int argc, char **argv) {
    hero_args_set(argc, argv);
    h_0opt_a8ea2 r = h_handled_main();
    if (r.tag != 0) {
        hero_write_err(r.as.err.msg);
        h_0opt_a8ea2_release(&r);
        hero_runtime_check_leaks();
        return 1;
    }
    h_0opt_a8ea2_release(&r);
    hero_runtime_check_leaks();
    return 0;
}
```

```
clang -std=gnu11 -I runtime -o mainres.bin mainres.c runtime/runtime.c   → exit 0
./mainres.bin  → exit 1, stderr "the step did not work"
```

**`int main(int argc, char **argv)` is unchanged. Nothing crosses the C boundary
differently. No ABI question exists here, which is why I do not veto** — the
refusal is a policy, and the panel may keep it.

### The cost that makes it more than policy — MEASURED

`exit(code:)` is the **only** way a Heroes program reports failure to a process
today, and `runtime/hero_os.h:105` says it **bypasses
`hero_runtime_check_leaks()`** by design. On the FFI that is not a footnote:

```
acquire a Db, never close it, fall off the end   → exit 134,
    "panic: 1 C handle(s) never given back — every call marked `acquires` owes one marked `consumes`"
acquire a Db, never close it, exit(code: 1)      → exit 1, SILENT, stderr empty
```

So telling a binding author *report failure with `exit(code:)`* tells them to
**switch off §4.19's own handle ledger on exactly the path where a handle is
most likely to be stranded** — the error path. The fallible-`main` shim above
does not have that hole: it runs the gate before it returns 1.

---

- `experiment`: eleven Heroes programs built with the tree's `heroes 0.2.0` in an
  isolated copy, plus three hand-written C shims. Real `sqlite3.h` / `-lsqlite3`
  bindings for the handle cases. **clang accepted the `main -> ()?` shim at exit
  0** and it ran to exit 1 with the message on stderr. `[Db].sort()` was
  **refused** at `heroes build` exit 1 (`error[unordered_element]`);
  `@dbs[i]` and `dbs[0] @ d` were **accepted** at exit 0.
- `argument`: R1 is clean; **R2 is not** — a C out-parameter into an array
  element and `xs[i] @ v` over foreign handles both compile, the handle ledger
  tracks them, and §4.19's ladder needs both at step 3, so R2's sentence is FFI
  documentation and should be priced as such. On R3 I do not contest the
  refusal; I contest writing it **alone**. The reader's real silence is not *may
  `main` be `-> ()?`* but *what does my program's exit status carry* — and the
  answer, measured, is that 18 of 20 shipped bindings cannot report failure at
  all, the flagship SQLite example exits 0 on a failed open, and the one escape,
  `exit(code:)`, silently disables the handle-leak gate. (119 words)
- `prediction`: **falsifiable, and on a named binding.** Write §4.19's ladder
  step 3 (a prepared-statement cache, `[Statement]` filled through `@stmts[i]`)
  and it needs **no shim and no `map`** under R2 stated as `xs[i] @ v` plus
  `f(@xs[i])` — I compiled the `[Db]` form of exactly this at exit 0. And:
  **`examples/sqlite/main.hero` will still exit 0 on a failed `sqlite3_open`
  after this sitting unless R3's sentence carries the status rule**, because the
  file has no `exit` call and R3 as briefed adds none. Instrument:
  `sed 's/":memory:"/"\/nonexistent-dir-xyz\/db.sqlite"/'` then run and read
  `$?`.
- `condition`: I move to **approve** if R3's sentence says what a *program's*
  status carries — at minimum *"a program exits 0 unless it calls `exit(code:)`;
  an abort is not 0; the status is the low 8 bits"* — and R2's sentence names the
  `@` out-parameter into an array element, not only `xs[i] @ v`. I move to
  **veto** only if a proposal changes `int main(int argc, char **argv)`,
  which nothing here does.

---

## UNRUN

- **Windows exit-status width.** `hero_exit` is `exit((int)code)` under a comment
  reading *"A shell reads the low 8 bits anyway"* (`runtime/parts/os.c:333-336`)
  — a POSIX premise, `.claude/rules/module-shape.md`'s "premise about the world"
  shape. `cmd.exe`'s `%ERRORLEVEL%` reads a full 32-bit DWORD, so I predict
  `exit(code: 256)` is **0 here and 256 on Windows**, i.e. the same binding
  forwarding the same C error code reports differently per platform. UNRUN — it
  needs the Windows box (`.claude/rules/platforms.md`; only the author starts
  it). Command that settles it: build `exit(code: 256)` there and read
  `echo %ERRORLEVEL%`.
- **Linux leg.** Same probes under `--sanitize` on the Linux container, where
  LeakSanitizer exists (CL-055), to see whether `exit(code:)` also suppresses
  **LSan** on a handle path and not merely `hero_runtime_check_leaks()`. UNRUN.
  Command: `docs/ref/environment/linux/` image, then
  `heroes build f/exitleak.hero --sanitize -o f/exitleak.bin && ./f/exitleak.bin`.
- **Whether `exit` accepts a positional argument.** Every one of the 11 example
  files calling it writes `exit(1)`, while `spec/heroes-spec.md:313` and
  `selfhost/library_source.hero:259` both declare `function exit(code: i64)`.
  Both spellings appear to compile; I ran only `exit(code: n)` deliberately, so
  the call-site rule is a question for the llm-ergonomist rather than a claim of
  mine.
- **A count I got wrong and corrected by re-running** (CL-057): my first sweep
  grepped `exit(code:` and reported **0** example files. The corpus writes
  `exit(1)`. The real count is **11** files, and the binding census above is the
  corrected one.

## Files

Probes and logs: `/private/tmp/claude-501/-Users-joseph-Temp-heroes-lang/bf49271c-9c48-4701-9a34-ec92d4cc09af/scratchpad/ffi159/`
(`p/` process-boundary probes, `f/` FFI probes, `ex/` the shipped SQLite example
and its failing-open variant, `p/mainres.c` the hand-written fallible-`main`
shim). The repository working tree was not modified.
