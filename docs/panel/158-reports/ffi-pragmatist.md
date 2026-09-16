# Panel 158 — ffi-pragmatist report

Seat: FFI pragmatist. Mandate: design.md §1.11 (everything comes from C) and
§4.19 (what may cross). Veto on ABI breakage or categorically harder bindings.
Method: compile the C a real binding needs, run it, capture exit codes directly.

- **verdict**: **object** — the shared brief's *consistency, not safety* footing
  SURVIVES my half, on twelve shapes and two platforms; §1.12 is **not**
  reached. I object to the brief's option set, not to its footing.
- **section**: design.md §1.11, §4.19. §1.12 tested and NOT reached.
- **veto**: **NOT cast.** The ground for one does not exist and I say so rather
  than manufacture it (see § The veto has no ground).

---

## 1. The veto has no ground, and that is the finding that should decide R1

A fallible **of any depth has no C ABI at all.** Run, not inferred:

```
$ ./heroes check w/t3_extern_fallible.hero          # function fabs(x: f64) -> f64?
error[ffi_type]: `f64?` cannot cross the FFI boundary, and it is an `extern`'s
result — a C header can declare a number, `bool`, `str`, `ptr`, `cstr`, a
function type as a parameter, and a `record` declared in this same group (§4.19)
CHECK=1
$ ./heroes check w/t3b_extern_param.hero            # function fabs(x: f64?) -> f64
error[ffi_type]: `f64?` … and it is an `extern`'s parameter …
CHECK=1
```

`T?` is not a parameter, not a result, not an extern-record field. `T??` is
therefore **Heroes-internal representation only**. No repair in the brief — not
even option 3, which looks the most expensive from this seat — can change a
single byte of any binding, any header verification, or any calling convention.
**The FFI cost of all four options is zero, measured.** That removes the seat
that would normally object loudest, and the sitting should know it.

## 2. The C I wrote, and clang accepted it

A real-shaped opaque-handle library, the sqlite3/curl shape with nothing in the
way. `scratchpad/lib/myhandle.h` + `myhandle.c`:

```c
typedef struct myres myres;
myres *myres_open(long id);
long myres_close(myres *r);
long myres_id(myres *r);
long myres_live(void);
```

`clang -c -fPIC` + `ar rcs libmyhandle.a` → exit 0, on macOS arm64 and inside the
Linux x86-64 container. Bound with the shipped idiom:

```
extern "myhandle.h" link "myhandle"
    record Res tag myres
    function myres_open(id: i64) -> Res acquires myres_close
    function myres_close(r: Res consumes) -> i64
```

And **the shipped SQLite binding**, §4.19 ladder step 3, copied word for word
out of `examples/sqlite/main.hero` with a `Stmt??` interposed on the Heroes side
(`{str: Stmt?}` cache, read back and peeled twice). Real `-lsqlite3`, both
platforms.

## 3. Every shape beside the brief's, with its exit code

Leak gate is `hero_runtime_check_leaks()`, emitted into `main`
(`selfhost/emit/decls.hero:261,277`) — a leak is exit 134, not silence.
macOS = Apple clang 21.0.0 (measured), arm64; `--sanitize` adds
`-fsanitize=address,undefined` and Darwin arm64 has **no LSan**
(`.claude/rules/c-boundary.md` § The instruments).
Linux = `heroes-linux` container, **LSan live and proved live** (§6).

| shape | check | build | run mac | run mac `--sanitize` | run linux `--sanitize` |
|---|---|---|---|---|---|
| `{str: str?}` → `str??`, allocating payload (brief's row 3, **re-measured**) | 0 | 0 | 0 | 0 | 0 |
| two-level `?` chain, allocating payload at BOTH levels, all three paths | 0 | 0 | 0 | 0 | 0 |
| three-level `str???` via `wrap<T>` twice, inner `fail` with built message | 0 | 0 | 0 | 0 | 0 |
| **handle** `Res??` via `{str: Res?}`, `acquires`, released | — | 0 | 0 | 0 | 0 |
| **handle** `Res??`, **NOT** released | — | 0 | **134** | **134** | **134** |
| **handle** `Res???` (generic twice), released | — | 0 | 0 | — | 0 |
| **handle** `Res???`, **NOT** released | — | 0 | **134** | — | **134** |
| `find` over `[Res?]` → `Res??`, 2 handles + 1 `fail`, all discharged | 0 | 0 | 0 | — | 0 |
| **SQLite** `Stmt??`, real `-lsqlite3`, finalized | — | 0 | 0 | — | 0 |
| **SQLite** `Stmt??`, statement DROPPED inside the nested fallible | — | 0 | **134** | — | **134** |
| `cstr??` (lend stashed in `ok()`) | **0** | **1** | — | — | **1** |
| `ptr??` | **0** | **1** | — | — | **1** |

The 134s are the right answer, not a failure:

```
panic: 1 C handle(s) never given back — every call marked `acquires` owes one
marked `consumes`, and this program is missing that many. The first is at 0x…
```

**The obligation counter sees through two levels and through three, on both
platforms, and through `find`.** The structural reason is in the emitted C:
`hero_handle_acquired(t4);` is emitted **immediately after the call**, before any
wrapping (`w/t4.c:242-244`). The obligation is keyed to the CALL, so the depth of
the fallible it is later put into cannot reach it.

## 4. The emitted C, read rather than trusted

The brief's line *"`?` peels the outer level"* is true and the code is better
than that. `str??` is a plain nested C struct — no box, no indirection:

```c
typedef struct h_0opt_f87774a  { int64_t tag; union { HeroStr ok;          HeroFailure err; } as; } h_0opt_f87774a;
typedef struct h_0opt_72520b2c { int64_t tag; union { h_0opt_f87774a ok;   HeroFailure err; } as; } h_0opt_72520b2c;
```

and for the handle case the payload is the **raw C pointer**, `myres * ok;`.

The release is recursive and tag-directed:

```c
HERO_TU_LOCAL void h_0opt_72520b2c_release(h_0opt_72520b2c *v) {
    if (v->tag == INT64_C(0)) { h_0opt_f87774a_release(&v->as.ok); }
    else                      { hero_failure_release(&v->as.err); }
}
```

**The brief's worry — "is the inner fallible's payload freed on the error path of
the outer one?" — is answered by the shape of the code, not by luck.** The
emitter does not free per level: it frees per **owning slot**, and every exit
block releases every slot. `bb2`, the outer error path of the two-level chain:

```c
bb2:
    t9 = t8.as.err;  hero_failure_retain(&t9);
    t10 = (h_0opt_f87774a){.tag = INT64_C(1), .as.err = t9};
    …
    h_0opt_72520b2c_release(&h2_f0);     /* the WHOLE two-level value, recursively */
    h_0opt_f87774a_release(&h3_outer);
    h_0opt_f87774a_release(&h4_f1);
    hero_str_decref(h5_inner);
    …
    return t10;
```

So nesting depth is invisible to the release discipline. A leak of the shape the
brief feared would need a slot the emitter does not declare, and it declares one
per binding. `--emit-c` twice is byte-identical (`cmp` exit 0).

## 5. Header verification is untouched, and a wrong signature is still a compile error

The thesis at the boundary — *a wrong FFI signature is a COMPILE error* — holds
with a `Stmt??` in the program. Both run against the real `sqlite3.h`:

```
$ sed 's/-> i64/-> f64/' on sqlite3_column_int           BUILD=1
error[ffi_return_type]: `sqlite3_column_int` does not return `f64` — that is
what `sqlite3.h` says, and clang read it

$ sed 's/tag sqlite3_stmt/tag sqlite3_stm/'              BUILD=1
error[ffi_unknown_name]: `sqlite3.h` declares no `sqlite3_stm` — clang read the
header and could not find it
```

And the probe set is the shipped one. `examples/sqlite/main.hero` emits **14**
`sqlite3`-bearing verification lines; my `Stmt??` version emits **15**. The extra
one is

```c
_Static_assert(__builtin_classify_type(*(sqlite3_stmt * *)0) != 13, "heroes-ffi-union Stmt ");
```

**and I isolated where it comes from rather than guessing.** A third version with
a FLAT `{str: Stmt}` cache — same binding, no nesting — emits the same **15**,
and `diff` against the nested version is the module mangle prefix and nothing
else. So the extra assert is the **container's**, not the nesting's, and
**nesting adds exactly zero to the verification set**. Putting a handle in any
container strengthens header checking; going two levels deep costs nothing
either way.

## 6. The control, because a silent leg proves nothing

LeakSanitizer does not exist on Darwin arm64
(`.claude/rules/c-boundary.md` § The instruments), so the Linux leg carries the
weight — and a green Linux leg is worthless unless the detector is on. I built
one on purpose: `myleak_stray()` mallocs 512 bytes and drops them.

```
CONTROL BUILD=0
CONTROL RUN=1
==24==ERROR: LeakSanitizer: detected memory leaks
Direct leak of 512 byte(s) in 1 object(s) allocated from:
    #1 … in myleak_stray
    #2 … in h_tctl_main /w/_probe/tctl.hero:7:10
SUMMARY: AddressSanitizer: 512 byte(s) leaked in 1 allocation(s).
```

**The detector is live.** The seven green Linux rows above are therefore real.

---

## Verdicts

### R1 — which repair

**Option 3 (make written `T??` legal, delete the parse refusal), and I object to
the option set as priced.**

- The FFI cost of every option is **zero** (§1). Option 3 is not expensive from
  this seat, and the brief should stop treating it as the risky one.
- **Option 1 is measured near-useless and I object to it on the numbers.** I
  counted map-of-fallible declarations across `examples/`, `selfhost/` and
  `tests/`: **4 occurrences, all four in `tests/golden/check/`, zero in
  `examples/`, zero in `selfhost/`.** So it breaks 0 programs *and* protects 0
  programs, and it leaves the generic door and the `find` door wide open — I ran
  both and both produce `Res??` and `Res???` at exit 0. Closing one of three
  doors on a value the language already represents correctly is a special case
  bought for nothing (§1.7 runs against it).
- **Option 2 stays vetoed and the FFI makes it worse, not better.** A binding's
  natural shape is *"the connection for this name, or the error that stopped it
  opening"* — `{str: Db?}`. Flattening collapses *never opened* into *open
  failed*, and in a C binding the second carries an `rc` the program must act on.
- Option 4 leaves `heroes check` accepting programs whose type the compiler
  refuses to parse, which is the §4.17 failure the brief names.

**The fifth option nobody listed, and it is mine**: option 3 **plus** moving the
container-element refusal from the emitter to the checker. See R3.

### R2 — `nested_fallible` absent from `is_thesis_rule`

**Not a defect, and for a reason the brief does not give.** Measured:
`check --permissive` on `function take(x: i64??)` still exits **1** with
`error[nested_fallible]`. It cannot be dropped because it is raised by the
**parser** (`selfhost/parse/type.hero:51-58`), not the checker, and
`--permissive` filters checker diagnostics. Adding the code to `is_thesis_rule`
would change nothing.

The FFI-relevant half the brief did not ask: **no `ffi_*` code is in
`is_thesis_rule` either**, and I confirmed `check --permissive` still exits 1 on
`error[cstr_escapes]`. That is correct and must stay correct: Part 11's control
arm is *§1 switched off*, not *the C boundary switched off*. A permissive run
that let a lend escape would segfault, and §1.12 outranks the experiment.

### R3 — printing an unspellable type

**Not acceptable, and there is a second instance of the same disease at the C
boundary that the brief has not noticed.** Measured:

```
error[type_mismatch]: expected `i64?`, found `i64??`
  fix (guess): `.must()` — abort on the error case, giving `i64?`
```

Under option 3 this message becomes spellable and correct, which is the clean
repair.

But the same *check says yes, build says no* shape is already live on my
boundary, and it will be **widened by one depth** if option 3 lands. Measured
today, at depth one:

```
function take(x: cstr?) -> i64        CHECK=0   BUILD=1
unsupported[pointer_element]: `cstr` as the element of a `T?` is not emitted yet
```

Also measured: `ok(("a"+"b").cstr())` is accepted by `cstr_escapes` at CHECK=0,
because `ok(…)` is syntactically a call and the rule says *a lend may stand only
as an argument of a call* — while the bare form `c: cstr = (…).cstr()` is
refused at CHECK=1. **The wrapper defeats the check-stage guard and only the
emitter catches it.** I confirmed this is the general container rule and not a
nesting-specific hole — `[cstr]` behaves identically — so nesting creates no new
class. But under option 3 a user may **write** `x: cstr??` in a signature, and
the refusal then lands at the emitter on a module boundary rather than at the
call site that made the lend. That is the cost I want on the record, and the
fifth option above is what pays it: teach the checker the `pointer_element`
refusal so `check` and `build` agree.

### R4 — does *consistency, not safety* change the answer

**It survives my half, and I ran the shapes designed to break it.** Twelve
shapes, two platforms, LSan proved live, real `-lsqlite3`, `acquires`
obligations at two and three levels, `find`, and a `str` payload at every level.
Nothing leaked, nothing corrupted, nothing gave a wrong answer at exit 0. The two
programs that *should* fail did fail, loudly, at 134, with the right count.

**§1.12 is not reached, and I say so plainly as the brief asked.** So the
sitting is decided on §1.7 and §4.17, and CLAUDE.md § Precedence rank 3 does not
enter. Panel 155 R3's twin is a fair comparison on the ground — but the ground
is not the whole argument: the twin left a correct program refused, while this
one leaves the compiler **printing a type it will not read back**, which is a
§4.17 failure the twin does not have.

---

## Prediction (falsifiable)

**If R1 adopts option 3, `examples/sqlite/` — step 3 of §4.19's ladder — needs no
shim, no wrapper and no binding edit, and its emitted C is unchanged.** Because
`error[ffi_type]` refuses a fallible at the boundary at every depth, the extern
group is untouched by anything the parser decides about `??`. Falsified by any
diff in the `sqlite3`-bearing `_Static_assert` / `hero_ffi_probe_*` lines of
`heroes build examples/sqlite/main.hero --emit-c` before and after the change.
Same claim for `examples/curl/` and `examples/ledger/db/sqlite.hero`.

## Condition — what flips me to veto

A repair that makes a fallible **crossable at the FFI boundary** at any depth:
removing or widening `error[ffi_type]` so `T?` or `T??` may be an extern
parameter, an extern result, or an extern-record field. That is an ABI change —
a tagged struct where a C header declares a scalar — and it is the marshalling
the Lua lesson refuses. Nothing in the four options does this; if a fifth is
proposed that does, I veto it on the spot.

Second, weaker condition: if the sitting adopts option 3 **without** the checker
half of my fifth option, my verdict stays `object` rather than becoming
`approve`, because `check` 0 / `build` 1 on a written `cstr??` parameter is a
diagnostic landing one module away from the mistake.

---

## UNRUN

- **Windows.** No `ffi_type`, `pointer_element`, handle-counter or leak-gate
  measurement was taken on the Windows box. Settled by, on that machine:
  `heroes build <the handle probe> --include … --library … -o t && t` per
  `docs/ref/environment/windows/WINDOWS-MACHINE.md`.
- **The full net.** Forbidden by the brief. No suite was run; nothing here is a
  claim about whether the tree is green.
- **raylib** (`examples/raylib/`), the by-value-struct binding: not built, since
  the container omits raylib on purpose. My §1 claim covers it by the
  `ffi_type` refusal, but it is unrun as a program. Settled by
  `heroes build examples/raylib/main.hero` on a box with raylib installed.
- **A `T??` inside an `extern record` field.** I ran the parameter and result
  positions and both are `error[ffi_type]`; I did **not** run a group-local
  `record` with a fallible field. Settled by a four-line program with
  `record R tag foo` carrying `x: i64?`.
- **Timing.** I took no `/usr/bin/time -p` figure for the compiler; a clock was
  never the question and a parallel container run would have invalidated it
  anyway (CL-025).

## Where the artifacts are

Programs, the C library, emitted C and logs:
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-lang/bf49271c-9c48-4701-9a34-ec92d4cc09af/scratchpad/`
(`w/` the Heroes programs and emitted C, `lib/` the C library, `tree/` the build
copy, `tree/_probe/` the container-side staging). The repository working tree was
not modified; `tree/` is a `cp -r` with `target` and `build` removed.
