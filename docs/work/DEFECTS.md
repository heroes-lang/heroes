# DEFECTS — the compiler defects that are still open

Read by whoever attacks a defect. Every item here is a **measured** failure of
the compiler on a program — a crash, a wrong answer at exit 0, a silence where
a message is owed — with its reproducer, its cause where known, and what is
owed. It exists because the author said so on 2026-09-03: *"non mi piace la
cartella defect … se ti rimane ancora qualcosa aperto in defect alla fine fai
un solo file chiamato DEFECTS.md all'interno di work, così tutto è ordinato"*.
Seven files under `docs/defects/` became this list and six entries in
`docs/work/DONE.md` that evening; the directory is gone.

**Only open defects live here.** The moment one is repaired its entry is ticked,
gains a *The repair* section with the measurements that prove it, and moves to
`docs/work/DONE.md` — the record — as every other list in this directory does
(CLAUDE.md §3). A defect that stays here after its fix is the shape §3 was
amended to prevent.

**One notation: `- [ ]`.** A finding written as a bare bullet is invisible to
every count in this project. The body of an entry is indented under its line;
it may be long, because a defect's reproducer, cause and measurements are the
entry, not decoration.

Format: `- [ ] **NNN — <title>** | <date, found by> | <status> | <where it came from> | <severity>`, then the body.

- [ ] **006 — A type parameter the checker accepted and the emitter could not name** | Date: 2026-09-03, M-corpus-depth step 5. **Found by writing a program, not by reading the compiler** — which is the reason that milestone exists. | **Status: OPEN.** Worked around in the program that found it (`examples/interpreter/run/value.hero`, four lines instead of one generic) and filed as work with a home in `docs/work/SCHEDULED.md`. Nothing is fixed here. | moved here 2026-09-03 from `docs/defects/006-a-type-parameter-with-nowhere-to-come-from.md` by author instruction — the directory is gone, its text is below, unedited except that its `## ` headings became bold leads | Severity: **★★★** — `heroes check` says exit 0 and `heroes build` aborts at **exit 134** with `assert failed: false`, naming no file, no line and no expression. It is the shape panel 082 R3 is about (*check accepts ⇒ build succeeds* is false) with a second failure on top of it: when the compiler does give up, it says nothing a reader can act on.


  Date: 2026-09-03, M-corpus-depth step 5. **Found by writing a program, not by
  reading the compiler** — which is the reason that milestone exists.

  **Status: OPEN.** Worked around in the program that found it
  (`examples/interpreter/run/value.hero`, four lines instead of one generic) and
  filed as work with a home in `docs/work/SCHEDULED.md`. Nothing is fixed here.

  Severity: **★★★** — `heroes check` says exit 0 and `heroes build` aborts at
  **exit 134** with `assert failed: false`, naming no file, no line and no
  expression. It is the shape panel 082 R3 is about (*check accepts ⇒ build
  succeeds* is false) with a second failure on top of it: when the compiler does
  give up, it says nothing a reader can act on.

  **The program.**

  Eighteen lines, reduced from `examples/interpreter/`'s value module while it was
  being written:

  ```
  variant Value
      number
          v: i64
      text
          s: str

  function wanted<A>(what: str) -> A?
      return fail("type", what)

  function number_of(v: Value) -> i64?
      return match v
          .number n => ok(n.v)
          .text _ => wanted(what: "a number")

  function main()
      print(number_of(.number(v: 7)).must())
      print(number_of(.text(s: "x")).is_err())
  ```

  `wanted`'s type parameter `A` appears **only in its return type**. There is no
  argument to infer it from; the only thing that could say what `A` is here is
  the context the call sits in — `number_of`'s declared `i64?`.

  **What each tool says, measured 2026-09-03 on the author's Mac with the.**
  compiler built from `seed/heroes.c`

  | command | exit | what it says |
  |---|---|---|
  | `heroes check` | **0** | nothing at all |
  | `heroes build --dump-ir` | **0** | the IR, carrying `??` where the type should be |
  | `heroes build --emit-c` | — | `assert failed: false` |
  | `heroes build` | **134** | `assert failed: false` |

  The IR dump is where the defect is visible rather than merely fatal:

  ```
  slots  $s0: Value · $r0: i64? · n: Value.number · $own4: i64? · $own5: ??
         $t10: ?? = call heroes wanted($t9)
         $t15: ?? = load $own5
         $t16: ?? = load $r0
  ```

  `??` is a type the lowering never resolved. Four slots and three temporaries
  carry it, and the dump prints at exit 0 — so the pipeline is willing to hand a
  half-typed program to the next stage.

  **The second shape, and why it matters more than the first.**

  Move the same call from a `match` arm to a `return` and the failure changes
  completely:

  ```
  function number_of(n: i64) -> i64?
      if n > 0
          return ok(n)

      return wanted(what: "a number")
  ```

  ```
  internal error: the lowered program is not well formed
    number_of (after lowering): bb1: returns a value of the wrong type
  error: the verifier refused
  ```

  Exit **2**, which CLAUDE.md §7 says is the right code for *the compiler is
  wrong*, from the instrument built to catch exactly this. So the verifier
  already knows this program is malformed — it just does not look at the shape
  the `match` arm produces. **The verifier is not blind to the defect; it is
  blind to one of its two spellings**, and the one it misses is the one that
  reaches the emitter.

  **What is not yet decided, and why nothing was fixed here.**

  Whether a type parameter may be inferred from the **return context** at all is
  a language question and therefore a panel path (CLAUDE.md §4). The spec says
  generics are *"always inferred, never written at the call site"* (§ Functions
  and calls) and does not say inference reads only the arguments. Two rulings are
  available and they are opposite:

  - **refuse it** — a type parameter must appear in the parameter list, so
    `wanted<A>(what: str) -> A?` is `error[...]` at *check*, with a diagnostic
    naming the parameter that has nowhere to come from. This is the smaller
    change and it makes the two failures above impossible.
  - **support it** — infer from the enclosing return type, which is what the
    checker already half does (it accepted this program), and repair the lowering
    so the instantiation carries a resolved type.

  Either way **two repairs are owed regardless of the ruling**, and they are the
  reason this file exists rather than a note in a commit:

  1. the verifier must catch the `match`-arm shape as it catches the `return`
     shape, so the worst outcome is exit 2 rather than exit 134;
  2. `assert failed: false` must become an `internal error:` naming the function
     and the node, as the verifier's own message does. An internal assertion that
     prints the word `false` and nothing else is a message that costs its reader
     an hour.

  **How it was found.**

  `examples/interpreter/run/value.hero` needed two refusals that differ only in a
  noun — *expected a number, found a string* and *expected a string, found a
  number* — so they were written once, generically, with the type parameter in
  the return position. That is the natural way to write it, and it is what a
  language with return-position inference invites. The module's own tests then
  reported `assert failed: false` with no test name, which is what sent the hunt
  into the compiler rather than into the program: `heroes run` does not execute
  test blocks, so a message that appeared under `heroes run` could not be coming
  from the program's asserts.
