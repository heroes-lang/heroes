# Panel 195, completeness critic, first pass (the briefs against the world)

Started 11:48:57 (`date`). No verdict. Copy: `<scratchpad>/195-critic/`, from
`git archive 23eb1169 | tar -x`, `round-b12/run-4254/seed-new.c` over
`seed/heroes.c` (sha1 2687685540fe...). Instruction counts only, no timing.

Findings are added below as they are run.

## Written 11:55:42 — the copy, and the facts that hold

Built 11:49-11:52: `heroes-seed` from `seed-new.c`, then `./heroes-seed build
selfhost/main.hero -o heroes` ("wrote heroes", exit 0).

Verified by command (each command run in `195-critic/`):

- `selfhost/ir/lower.hero:77` is `.constant_decl k =>`, the constant lowered
  through `build.begin(... kind: .constant_kind ...)` as a function (`sed -n 60,100p`).
- `HERO_STR_STATIC` at `runtime/heroes_runtime.h:198`; the lock-free assert and
  its read-only reason at :127-145; `HERO_RUNTIME_ABI 27` at :58.
- `hero_str_incref` :130, `hero_str_decref` :141-148, both return on a relaxed
  load `< 0` (`grep -n`, `sed -n 120,150p runtime/parts/str.c`).
- `hero_array_decref` :78-91, `fetch_sub ... > 1` then release, comment says
  "negative counts included"; `drop.c:84` is the `memcpy(&a->refcount, &next, ...)`.
- `cow.c:44` `if (a->refcount == 1) return;` then copies (`hero_array_unshare`),
  and `hero_array_push_owned` :78 tests `== 1` before writing in place.
- `HeroArrayHeader` is `{refcount, len, cap, elem}` (`heroes_runtime.h:473-479`), so
  ir12's `{{-1, 32, 32, &hero_desc_int}, ...}` initialiser matches its order;
  `hero_desc_int` is declared (:435).
- The constants: `grep -rE '^constant [A-Z_0-9]+: (\[|\{)' selfhost tests/harness | wc -l`
  = **66**; by type 61 `[str]`, 5 `[i64]`, 0 map. A wider grep over every `.hero`
  outside `archive/` (any indentation, any name case) finds **no map-typed
  constant anywhere**; ir12's own goldens add `[[i64]]`, `[Shape]` and an empty
  `[i64]` (`tests/golden/run/fixedbugs-382-...hero:24-30`).
- Spec § 4 line 115 says "a written body computes over literals and other
  constants"; `grep -n -i constant spec/heroes-spec.md` finds 8 lines and none
  states a cost. Verified as a reading.
- The reproducer on this copy's compiler (`heroes build`, default **-O0**,
  `selfhost/cli/verbs.hero:31`), `/usr/bin/time -l`: `k` **4,599,763,643**,
  `local` **116,461,213** (ratio 39.5). At `-O2` (`heroes build -O2`): `k2`
  **2,579,516,710**, `local2` **38,388,381** (ratio **67**). The brief's
  "about 39 times" holds at -O0 only; it does not say which level its
  "Measured" line is, and at -O2 the gap is 67x.
- Emitted C (`--emit-c`): `h_k_K(void)`, `hero_array_new(&hero_desc_int, 32)`
  then 32 `hero_array_push_owned`; the read site calls `h_k_K()` and indexes.

## Written 12:19:37 — framing facts found false, unverifiable or incomplete

(The session stopped on a rate limit between about 12:00 and 12:19 by the
coordinator's message; every command below was run between 11:49 and 12:19 in
`195-critic/`, and its output is quoted.)

### F1. The double-release "trade" is not what `00-shared.md` says it is (load-bearing for compiler-engineer item 2)

`00-shared.md`: *the guard turns a count driven negative by a release too many
from a loud failure into silence*. Run (`195-critic/ro/dbl.c`: `hero_array_new`,
`hero_array_decref` twice, against this copy's `runtime/runtime.c`, -O0, no sanitizer):

    32 elements:  word after free: 0 (0x0)
                  panic: the process is dying of SIGTRAP, and this runtime did not raise it. ...   exit 133
     4 elements:  word after free: 6 (0x6)
                  second decref returned / leak check passed                                    exit 0

So on this Mac (a) a release too many on a heap block does not drive a count
negative: the block is freed at 1 and the next decref reads freed memory (0 or 6
here); (b) today's "loud" is the allocator's double-free trap, in one size, and
**exit 0, silent, in another**; (c) neither value is negative, so ir12's guard
changes neither outcome. Under ASan both are `heap-use-after-free`.

And a program reaches it today: **defect 387** (open, `blocking`, filed 11:36 at
`f78c4f69`, which is on `lane-b12-ir12` but AFTER `23eb1169`, so it is **not in
the seats' copy** and no grep there finds it). Its reproducer on this copy's
compiler: `3004`, exit 0; `--sanitize`: `ERROR: AddressSanitizer: heap-use-after-free`.

What the guard really trades, stated as a question for the seats: a static block
is never freed, so an emitter bug that releases a constant's value once too many
becomes invisible **even under `--sanitize`**, where today (a fresh heap block per
read) ASan names it. `str` literals already live under exactly that trade
(`str.c:141-148`). The brief's question "what keeps a double release loud" should
be asked against that baseline, not against a loudness today's runtime does not have.

### F2. "written in read-only memory and then freed": the second half does not happen on Darwin arm64

`195-critic/ro/ro.c`: `static const struct { HeroArrayHeader h; int64_t e[4]; } K_static = {{-1, 4, 4, &hero_desc_int}, ...}`:

    nm -m ro | grep K_static  ->  (__DATA_CONST,__const) non-external _K_static
    ./ro   (today's hero_array_decref)  ->  "len 4", then exit 138, stderr empty
    ./ro i (today's hero_array_incref)  ->  "len 4", then exit 138, stderr empty

SIGBUS at the first write (the `fetch_sub`/`fetch_add` itself), with no message:
the outcome design.md §1.12 forbids. Nothing is freed. Today's **incref** faults
too; the brief names the guard's need only for `decref` in prose (ir12's C does
add it to both). Linux and Windows are unrun by me.

### F3. The other thread: a foreign-thread callback cannot read a constant today; a spawned thread can

compiler-engineer item 3 points at `selfhost/emit/callback_guard.hero` for "a
constant read by a callback on another thread". `runtime/parts/thread.c:87-99`:
`hero_thread_guard` panics at the callback's first instruction on any thread the
program did not start. The reachable concurrent read is **`hero_thread_spawn`**
(`runtime/hero_os.h:411`; `parts/spawn.c:221` claims the thread). Run
(`repro/spawnk.hero`: two spawned workers and `main` each read `K[at % 32]`
100,000 times): `1453125`, exit 0, and the same under `--sanitize`; the emitted
worker starts with `hero_thread_guard("spawnk.worker");` and calls
`h_spawnk_K()`. The seats should race **spawn**, not a C library's thread.

### F4. Shapes the language admits that neither brief lists

`selfhost/resolve/constant_body.hero:88-99` refuses only `.call`, `.method`,
`.try_expr`, `.hole`; it admits `.interp`, `.binary`, `.unary`, `.field`,
`.index`, `.if_expr`, `.match_expr`, `.name`, `.null_ptr`, `.map`. Run
(`repro/mapk.hero`, this copy's compiler):

    constant AGES: {str: i64} = {"ziggy": BASE + 2, "mars": 7}   -> 42
    constant PICK: [i64] = if BASE > 10 [BASE, BASE * 2] else []  -> 80
    constant WORDS: [str] = [f"a{BASE}b", "plain"]                -> a40b
    constant FROM: [i64] = [PICK[1], PICK[5]]                     -> panic: array index out of range, exit 134

So "a body is literals" is a property of today's 66, not of the language: a
static route needs a decidable test for it, and a computed body that **aborts**
(index, overflow, division) aborts today **at the read**. Evaluating it at
compile time adds a diagnostic (that leaves the soundness lane:
`.claude/skills/panel/SKILL.md:18-35`); evaluating it at program start aborts a
program that never reads it. And `repro/fnk.hero`:

    constant STEPS: [(function(i64) -> i64)] = [twice, thrice]   -> 25, exit 0

With `hero_os.h`'s spawn bound (`fnk2.hero`), the emitted C guards `twice` and
`thrice` **only because** the constant's IR holds `funcref heroes twice`
(`--dump-ir`). `callback_guard.hero`'s own falsifiable claim (its header) names
"a `constant` of function type ... whose address is folded at compile time" as
the day it dies. A static table that writes `&h_twice` into an initialiser
without the `funcref` drops the guard. Missing from both briefs: `[f64]`,
`[u8]`/`[i8]` (element size 1), `[str]` holding a NUL (`HERO_STR_MAGIC_NUL`), a
`null` pointer element, a function-valued element, a computed body, an `if`/`match` body.

### F5. The ABI question already has a written rule the briefs do not cite

`.claude/rules/generated-c.md` § The shape of the file: *"The stamp's job is
version skew, and it is **not** extended to cover behaviour"* (CL-007); and
`runtime/parts/drop.c:19-26`: a behaviour change leaves `HERO_RUNTIME_ABI` where
it is, *"What catches the behaviour change is the cache key"*. A guard-only change
does not move 27 under that rule, while emitted C that NEEDS the guard, linked
against a runtime without it, is F2's silent SIGBUS. The seats should rule
against that rule by name (or name the declaration, e.g. a new macro, that would
make the skew a compile error).

### F6. Defect 361's guard and the emitter's own words

`heroes_guard_open.h:9-27` and `guarded_names.hero` cover the words the runtime's
headers write. If the emitter writes the static struct itself (not via a runtime
macro), its member names (ir12's `h` and `e`) are the unit's own words after the
groups' headers: `h` is in `NAMES`, `e` is not (`guarded_names.hero:18-60`). A
runtime macro like `HERO_STR_STATIC` would put the words under the guard and add
them to `NAMES`, which `macro_guard`'s test then demands.

### F7. Times and commits that do not line up

- `00-shared.md` names the seats' base `23eb1169` as "lane ir12's branch";
  `git rev-parse lane-b12-ir12` is **96f3a588** (merge of `lane-round-b12`, which
  brings defect 387's filing). The copy is `23eb1169`'s tree, as the brief says.
- "lane ir12's compiler built **11:07** from its branch with `f7576a01`":
  `f7576a01`'s author and commit date are **11:33:57**. Unverifiable as stated;
  in substance it holds, since this copy's compiler from the commit reproduces the
  numbers (the 11:55 section: 4,599,763,643 vs 4,597,018,435; 116,461,213 vs 117,277,639).
- The author's-goal issue path cited in `00-shared.md` is on `lane-round-b12`
  (`b3738dc9`, 11:45:36), **not in the seats' copy** nor on `main`.
- The seed (`seed-new.c`) is pre-`f7576a01`: `k` built by `heroes-seed` reads
  **56,873,259,045**. A seat that stops after the first build step measures the
  old route.

### F8. CARRIED "19 read inside a loop by text": 19 only if names inside string literals count

My script (`195-critic/tools-critic/loops.py`: a reference indented under a
`for`/`while`, same-module bare or `module.NAME`) reproduces **19**, but 11 of
them are the name inside a string literal (`"    APPEND_ONLY: "`, `cards.AREAS`
in a message, ...). Stripping string literals: **8** (`ALLOCATORS`, `BUDGETS`,
`ENTRIES`, `FIELDS`, `LEVELS`, `NORMAL_ARM`, `SECTIONS`, `SLOW_APPENDS`),
`LEVELS` still the one in `selfhost/`. 48 more are read in a `for` header (once
per loop entry). The set ir12 counted is unknown to me; a text count is in any
case not a hotness count, since a read in a function called from a loop is as hot.

### Negative sentences written as facts

- `00-shared.md` "none a map": true of the tree (the 11:55 section), but the language admits a
  map constant (F4, run), so "no map constant exists" must not become "a map
  constant need not be served".
- "the route is sound only with the guard": true on Darwin by F2; Linux arm64 and
  Windows unrun.
- "Nothing more, `f7576a01` as the repair" is offered as a route, fine.
- ffi brief item 4 asks whether a binding's header "sees any new word": a
  question, fine.

### CARRIED numbers a seat will lean on

The whole route table (seven rows, -O0/-O2), the `[str]` pair 5,810,514,277 /
170,140,626 (the brief does not say "before" which compiler, nor the `[str]`
twin's source), "ir12 ran copy-on-write of the shared block clean under ASan and
UBSan", "19 in a loop" (F8), "61 `[str]` and 5 `[i64]`" (verified, the 11:55 section). Only the
`k`/`local` pair is re-measured here (-O0 and -O2).

### Routes nobody listed

1. **Element reads with no array at all**: lower `K[i]`, `K.len()` and `for x in K`
   on a literal-bodied constant to a `static const` C array of elements (for
   `[str]`, `HERO_STR_STATIC` literals, already count -1) with the bounds check;
   only a read of `K` as a whole value builds. No runtime change, no guard, no
   ABI question. 6 of F8's 8 in-loop sites are element reads or `for` headers.
   Unbuilt, a question.
2. **One process-wide copy built on the first read, published by an atomic
   compare-and-swap, marked by a sentinel count, in writable heap memory**:
   serves every shape of F4 (records, variants, maps, computed bodies, function
   values through the existing `funcref`), keeps an abort at the read, never
   writes a read-only page; still needs the incref/decref guard and an exit-time
   release for the leak check. Unbuilt, a question.
3. A `static` (not `const`) block: a missed guard decrements a count instead of
   SIGBUS. A variant of ir12's route, worth one line in a verdict.

### Instructions in the briefs, checked for safety

- `./heroes build selfhost/main.hero -o heroes` overwrites the running binary:
  run as `heroes-x` here, "wrote heroes-x", exit 0, 11:55-11:57. Safe on this Mac.
- Windows (`ssh win`, 11:58:25): answers, MINGW64 Git Bash, `clang version
  23.1.1`, 2 cores, `df -k /c` 30,279,160 KB free (> 10 GB), `/c/w/b12-b3738dc9-4531`
  present and four `heroes.exe` running: **the batch 12 leg is live**. No
  `/c/w/p195-*` yet. The ffi brief does not forbid building the compiler there;
  the 37 MB seed on two cores beside the leg is not light. Suggest: hand-written
  C plus `runtime/runtime.c` only, one clang at a time.
- Docker: `heroes-linux-arm64:latest` present; a container `romantic_sutherland`
  of that image is running (another session); `docker run --rm` makes its own,
  no conflict.
