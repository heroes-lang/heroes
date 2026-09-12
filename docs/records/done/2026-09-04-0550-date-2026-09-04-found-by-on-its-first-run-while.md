- [x] **009 — A record whose every field is unhashable emits C that clang warns about** | Date: 2026-09-04, found by `examples/ledger/` on its first run, while binding SQLite at program scale | REPAIRED the same hour, at the cause | `selfhost/emit/structural.hero::hash_body` · `tests/harness/suite_warnings.hero` (the rule it broke) | **Severity: the emitted C failed the corpus's own zero-warning rule, and six lines were enough to make it.**

  **The reproducer, whole:**

  ```
  record Handle
      at: ptr

  function main()
      h = Handle(at: nullptr)
      print(h.at == nullptr)
  ```

  `heroes run` on that answered `minptr.c:60:28: warning: unused variable 'v' [-Wunused-variable]` — and the program was otherwise correct, printing `true`.

  **The cause, read off the emitter.** `hash_body` opened every record's hash with `const <T> *v = elem;` unconditionally, then folded one term per field. A `ptr` has no structural hash, so `one_hash` writes `(hero_unreachable(), UINT64_C(0))` for it — a term that reads nothing. A record whose fields are ALL of that kind therefore declares `v` and never touches it. Nothing in the corpus had such a record until `examples/ledger/` needed two of them: SQLite's database and statement handles are `ptr`s, and a `ptr` may not be the element of a `T?` (`unsupported[pointer_element]`), so the binding wraps each in a one-field record — which is exactly what the diagnostic's own note tells you to do.

  **Why it matters more than a warning.** `tests/harness/suite_warnings.hero` holds every corpus program to **zero** clang warnings, both verbs, and it is one of the five suites that read `examples/`. So this was not cosmetic: it would have made the new program fail the net, and the shape it needs — a record around a foreign handle — is the first thing anybody writes when binding a C library. §4.19 asks for an FFI that is *complete and bug-proof*.

  **The repair.** `hash_body` asks, per field, whether that field contributes anything the hash reads — `descriptors.hash_call`'s own answer, through a new `hashes_structurally` — and declares `v` only when at least one does. `(void)elem;` was the alternative and is worse: it says *somebody might have read this* where the truth is that nothing can, and the `partial` arm above it already spells the deliberate-silence case. Measured after: the six-line program emits **zero warnings** and still prints `true`; the compiler's own tests and the net are in the same commit's verification.

  **THE FIRST REPAIR WAS WRONG, AND THE INSTRUMENT CAUGHT IT IN ONE BLESSING.** It asked, per field, whether `descriptors.hash_call` answers for that field's type — a predicate about what the loop *would* emit rather than about what it *did*. A **fixed array** field answers no there, because the loop handles one element by element through `fixed_element` instead; so `record VrStereoConfig`, whose only field is a `Matrix[2]`, lost its declaration while its lines went on saying `&v->projection[0]`. `tests/emission/run-ffi-a-c-array-member.c` showed it the moment the traces were re-blessed — one removed line, in a function whose next line reads the removed name — and `heroes run tests/golden/run/ffi-a-c-array-member.hero` was `error: use of undeclared identifier 'v'`. **That is CLAUDE.md §11's own shape, in the repair for a defect rather than in the defect**: a narrowing rested on a premise about the world (which field kinds produce a term) instead of on a fact about the value in hand. The second repair collects the lines it is about to write and declares `v` only if one of them says `&v->`, which is a fact about the lines and cannot expire. Both programs are green afterwards: the six-line reproducer emits zero warnings and the `Matrix[2]` golden runs.

  **What it says about the corpus, which is the reason to write it down.** M-corpus-depth's plan named `ledger/` for a measured gap — *FFI and "real program" are disjoint sets*, every `extern` program in the corpus being 65 to 192 lines and single-module. The gap was real and this is what was behind it: the shapes a binding needs at program scale had never been emitted, so nothing had ever checked them. Two of them broke on the first run — this, and `ptr` inside a `T?`.
