# Panel 196, compiler-engineer

Work copy: `<scratchpad>/196-compiler-engineer/`, `git archive 39935f7c`, compiler
built from its seed at 20:55 (by `date`). Evidence copied in from the panel
worktree's untracked `docs/panel/196-evidence/` (read, not edited). Lines are
counted with `suite_layout.hero`'s `code_lines` (non-blank lines outside a
`test "` paragraph, comments counted), reimplemented in
`<scratchpad>/196-compiler-engineer-work/codelines.py`.

Written as I go, 20:55 to 21:20 and, after an API session limit stopped the
seat, 22:44 to 23:03 (by `date`). The spec-warden's, the historian's and the
ffi-pragmatist's reports were read at 22:44 and are cited where I engage them.

**In one paragraph.** S7 builds: `SHA256_Final` runs from one `.hero` file with
no header of the program's own (`1 1 1 32 186 173`, `--sanitize` clean), and so
do `pipe`, `gethostname` and the EVP out-count. S1 and S2 build too. The three
change no emission where unused (the frozen compiler's own source, emitted by
the S7 compiler, is `cmp`-identical to the seed) and no tracked file's `check`
outcome (a 2,740-file census). None adds a core construct; all of it is
checker admissions and emitter work at the extern call, 12 files and 385
lines with the guard and the default refusal. **Every static route leaves N the
binding's word**, measured on all three: an understated N corrupts at exit 0
or 134. A 16-byte guard on S7's buffer, built for 12 lines, turns that into a
named abort, `--sanitize` included. **Defect 396's own reproducer is closed by
no static route but the default refusal**, which verifies nothing
(`counted_by 1` builds the fault) and moves 59 files; the run-time guard on
scalar cells (S4, hand-built in C) is the route that stops it without refusing
a correct program.

## 0. Baseline on the frozen compiler (20:56 to 20:59)

Probes copied into `<copy>/ce/` beside `sha_shim.h` and `box.h`; `heroes check
--brief` and `heroes run`, frozen compiler:

| probe | check | run | output / first diagnostic |
|---|---|---|---|
| `fixed_param` | 1 | 1 | `ffi_type` at 9:32, *`u8[32]` cannot cross the FFI boundary* (one diagnostic) |
| `field_ptr` | 1 | 1 | `lent_shape` 9:27 and `field_lend_uncounted` 16:28 |
| `field_array` (shim) | 0 | 0 | `1 1 1 186 173` |
| `md_field` | 0 | 0 | `1 1 1 186 2531777658719584577` |
| `pipe_cell` | 0 | 0 | `0 3 4` (`after` 7 became 4) |
| `host_one` | 0 | 134 | `0 101 0`, then the false *null function pointer* panic |
| `host_cell` | 1 | 1 | `counted_by_shape` 3:47 |
| `evp_outcount` | 0 | 0 | `1 32 186 173` |
| `evp_outcount_512` | 0 | 134 | `1 1 64 -6286656575195475423` |
| `arr_ptr` | 1 | 1 | `bad_operand` 7:29, *`ptr` takes a fixed run of bytes* |

Every row agrees with the shared brief and the critic.

## 1. Where an `@` lend is read today (code_lines, frozen tree)

- **The checker's boundary rule**: `selfhost/check/ffi.hero` (282),
  `crosses_the_boundary` refuses `.array | .fixed` for every extern parameter
  (`ffi_signature`); `check/ffi_sweep.hero` (93) refuses a fixed array anywhere
  but a group record's field; `check/lend_extent.hero` (183) judges
  `counted_by` (a `ptr` or a record lent whole, naming a SIBLING, an integer).
- **The header probes**: `cli/pointee_wants.hero` (142) decides the asks
  (`numeric_out` for a scalar `@`, `opaque_out` for depth, `lend_ask` UNIT or
  WHOLE); `cli/pointee.hero` (243) runs clang; `cli/header_types.hero` (291)
  writes the two units: the dump is `extern __typeof__(f) hero_ty_f;` read
  through `-ast-dump=json`, the verdict `_Static_assert`s on `sizeof` and sign;
  `cli/pointee_ask.hero` (44) the records; `emit/ffi_pointee.hero` (279) and
  `emit/ffi_unit.hero` (123) read failures back onto the parameter.
- **The call**: `emit/inst.hero` renders an `.inout_arg` as `&<place>`;
  `emit/ops.hero`'s `guard_arguments` casts it `(void *)` where
  `emit/extern_probe.hero` (297) `is_cast_out` says so (numeric, `ptr`, `bool`);
  the probe spells the parameter `T * aN`. **Nothing anywhere reads how many
  `T` C writes through an `@` scalar**: the width and sign of ONE `T` is the
  whole question asked.
- **An element**: `ir/inout.hero` (213) copies `@xs[i]` into a temporary and
  stores it back after the call (`WriteBack`), so C writing past the element
  writes past a one-element C local, not into the array.
- **Route C**: `emit/lend_extent.hero` (184) writes two `_Static_assert`s for a
  constant count, one compare and a named abort for a run-time one, in
  `sizeof(*(P)0)` units; `emit/lend_count.hero` (202) finds the count, and only
  as a SIBLING argument (`counted_siblings`, -1 otherwise, and a -1 emits no
  check at all).

## 2. S1 built: `@md: u8[32]`, lent a place of that exact type

**Built** in the copy, `heroes-s1` (21:06). Four files, **48 insertions, 10
deletions** (`diff -ru`), no new module, no grammar change (§3's `Type`
already writes `u8[32]`, and `CParam` takes a `Type`):

| file | code_lines before -> after | what |
|---|---|---|
| `check/ffi.hero` | 282 -> 299 | `fixed_out`: an `@` parameter of a fixed array of numbers crosses |
| `check/ffi_sweep.hero` | 93 -> 100 | the sweep claims an extern's `@` parameter type |
| `cli/pointee_wants.hero` | 142 -> 150 | the width ask reads through one fixed array to its element |
| `emit/extern_probe.hero` | 297 -> 299 | cast `(void *)`, spelled `T *` in the probe |

`check/ffi.hero` lands one line under §11's 300; the landing would owe it a
seam or a DECIDED row.

**`fixed_param.hero` under S1**: `check` 0, `build` 0, `run` 0, printing
`1 1 1 173` (SHA-256("abc")'s last byte). The emitted C (`--emit-c`, line 169):
`t9 = SHA256_Final((void *)&h1_d.b, &h0_c);` the field's address and nothing
else; the probe line is `hero_ffi_probe_..._SHA256_Final(uint8_t * a0, struct
SHA256state_st * a1)`.

**The shapes beside it**, each run with `heroes-s1 check --brief` and `run`:

| shape | check | run | what |
|---|---|---|---|
| `u8[16]` field lent to `@md: u8[32]` | 1 | 1 | `type_mismatch` *expected `u8[32]`, found `u8[16]`* |
| a `[u8]` local lent | 1 | 1 | `type_mismatch` *found `[u8]`* |
| an element `@d.b[0]` | 1 | 1 | `type_mismatch` *found `u8`* |
| `@md: i8[32]` | 0 | 1 | `ffi_parameter_type`, **with `fix (guess): declare md as @md: u8`** |
| `@md: u32[8]` | 0 | 1 | `ffi_parameter_type`, the same fix |
| `md: u8[32]` by value | 1 | 1 | `ffi_type` (unchanged) |
| `@md: u8[16][2]` | 1 | 1 | `ffi_type` (unchanged) |
| `d = Digest32(...)`, then `@d.b` | 1 | 1 | `not_mutable` |
| `pipe(@fds: i32[2])`, record `{fds: i32[2], after}` | 0 | 0 | `0 1 7`: correct, `after` intact |
| **`pipe(@fds: i32[1])`, record `{fd: i32[1], after: 7}`** | 0 | 0 | **`after` prints 4**: the 396 fault under S1's word |
| a Heroes `function fill(@x: u8[32])` | 1 | 1 | `fixed_outside_a_group` (unchanged) |

Two findings. **The `guess` fix is the 396 fault**: the pointee reader proposes
`@md: u8`, dropping the extent; a landing owes `ffi_pointee.hero` the fixed
spelling (`@md: u8[32]`). **S1 moves the lie, it does not remove it**:
`@fds: i32[1]` against `int pipe(int [2])` builds and corrupts at exit 0, which
S5's redeclaration (below) would catch, since the header writes `[2]`.

`grep -rlE '@\w+: *[iuf][0-9]+\[[0-9]+\]' tests examples`: 0 files, so S1
changes no tracked program's outcome; no `selfhost/` binding changes.

**The compiler's own tests under S1** (a snapshot of the S1 tree,
`heroes-s1 test selfhost/main.hero`, ended 21:10): 1,308 tests, 1 failed,
*"no operand measures the spec, and an unreadable file is the tool failing"*,
which reads `STALE: the recorded count is for 7a1ea85e443d182b`. **The control**,
the frozen compiler on a pristine archive of `39935f7c` (ended 21:15): 1,308
tests, the same one failed. So S1 moves no self-test; the one red is the
frozen tree's stale spec pin, which the shared brief already names.

## 3. S2 built: `counted_by 32` and `counted_by SHA256_DIGEST_LENGTH`

**Built** on top of S1, `heroes-s2` (21:12). Three files, **80 insertions, 2
deletions**:

| file | code_lines before -> after | what |
|---|---|---|
| `parse/marks.hero` | 251 -> 254 | `counted_by` takes an `int_lit` (the grammar moves: `CParam`'s `"counted_by" ident` becomes `( ident \| integer )`) |
| `check/lend_extent.hero` | 183 -> 203 | `stated_extent`: a literal above zero, or a group `constant` of an integer type |
| `emit/lend_extent.hero` | 184 -> 227 | `stated_mark` and `stated_checks`: the two `_Static_assert`s route C already writes, with the signature's own text and span |

**Why the emitter had to move, and it is the strongest fact about S2**:
`lend_count.counted_siblings` answers -1 for a name that is no sibling, and
`lend_extent.assertions` emits **no check at all** for a -1. So S2 built
`check`-side alone (the spelling admitted, the emitter untouched) would accept
`counted_by 32` and check nothing, which is 396 wearing a word. The emitter
half is not optional.

Probes (`ce/s2_*.hero`, header `s2.h` declaring only structs; `heroes-s2`):

| shape | check | run | what |
|---|---|---|---|
| `md: ptr counted_by 32 lent`, `d.b.ptr()` on `u8[32]` | 0 | 0 | `1 1 186 173` |
| `counted_by SHA256_DIGEST_LENGTH`, the same | 0 | 0 | `1 1 186 173` |
| `counted_by 32` on a `u8[16]` field | 0 | 1 | `field_lend_extent` at 13:46 (the signature's `32`), **with `fix (guess): state the field's own length, 16`** |
| `counted_by SHA256_DIGEST_LENGTH` on `u8[16]` | 0 | 1 | the same, the fix replacing the constant with `16` |
| `counted_by 0` | 1 | 1 | `counted_by_shape`, worded *"`0` names no other parameter"* |
| `counted_by NOPE` | 1 | 1 | `counted_by_shape` (unchanged wording) |
| `@md: Digest32 counted_by 32` (S1', a record to `unsigned char *`) | 0 | 1 | `ffi_parameter_type`, *a different kind of thing* (today's refusal, unmoved) |
| `@md: u8 counted_by 32` | 1 | 1 | `counted_by_shape` (the scalar `@` still cannot carry a count) |
| `EVP_DigestFinal_ex(c, md: ptr counted_by EVP_MAX_MD_SIZE lent, @s: u32)`, SHA-512, `u8[32]` field | 0 | 1 | `field_lend_extent`: refused at build |
| the same into a `u8[64]` field | 0 | 0 | `1 1 1 64 7`: `after` intact |

Two findings beside it. **The `guess` fix on a stated extent writes the lie**:
it offers to rewrite the binding's `32` (or `SHA256_DIGEST_LENGTH`) to the
field's 16, which makes the binding say C writes 16 where it writes 32; a
landing owes `field_lend_errors.hero` a stated-extent message with no such fix
(about +20 lines, unbuilt). **S2 answers the EVP out-count twin by spelling, not
by refusal**: `counted_by EVP_MAX_MD_SIZE` is correct and checked, and the
critic's `counted_by s` (an out-count) still builds and corrupts; the compiler
cannot tell `unsigned int *s` in from out.

S2 does not reach the unmarked `@md: u8`, `pipe` or `gethostname`'s one-cell
lend; it needs S0's place (a group record field, so a hand-written header
struct) for `SHA256_Final`.

## 4. S7 built: `@md: [u8] counted_by N`, a buffer the emitted C owns

**Built** on top of S2, `heroes-s7` (21:18). **S7 builds, and `SHA256_Final`
runs from one `.hero` file with no header of the program's own**
(`ce/s7_sha.hero`, the group naming `openssl/sha.h` and nothing else):
`check` 0, `build` 0, `run` 0, printing `1 1 1 32 186 173`, `md.len()` 32 and
SHA-256("abc")'s first and last bytes; `run --sanitize` exit 0, 0 `ERROR`
lines.

The spelling is S2's, so the two are one rule: `counted_by` names a sibling C
reads the count from, or states the count itself (a literal or a group
constant), and on a `[T]` lent through `@` only the stated form is admitted.
What the emitter writes (`--emit-c`, lines 191 to 199):

```c
    uint8_t hero_buf_7_0[SHA256_DIGEST_LENGTH] = {0};
    { int64_t hero_n = hero_array_len(h1_md); if (hero_n > (int64_t)(SHA256_DIGEST_LENGTH)) hero_n = ...; for (...) hero_buf_7_0[hero_i] = *(const uint8_t *)hero_array_at(h1_md, hero_i); }
    t9 = SHA256_Final((void *)hero_buf_7_0, &h0_c);
    { HeroArrayHeader *hero_fresh = hero_array_new(&hero_desc_u8, (int64_t)(SHA256_DIGEST_LENGTH)); for (...) hero_array_push_owned(&hero_fresh, &hero_buf_7_0[hero_i]); hero_array_decref(h1_md); h1_md = hero_fresh; }
    }
```

C is handed the buffer and nothing else; the array is copied in (its first
`min(len, N)`, the rest zero) and replaced by exactly N after the call. No
runtime entry point is added (`hero_array_len`, `_at`, `_new`,
`_push_owned`, `_decref` are ABI 28's), so the runtime and its ABI do not move.

**Cost**, S2 to S7: 8 files, **208 insertions, 8 deletions**, one new module;
base to S1+S2+S7: 10 files, **330 insertions, 14 deletions**:

| file | code_lines base -> S7 | what |
|---|---|---|
| `emit/buffer_out.hero` | new, 100 | `buffers`, `before`, `after`: the buffer, copy in, copy out |
| `emit/ops.hero` | 284 -> 290 | the extern call routes its arguments through it |
| `emit/lend_count.hero` | 202 -> 220 | `array_param`: route C compares no extent for a buffer |
| `emit/lend_extent.hero` | 184 -> 227 | S2's two functions, and the skip |
| `emit/extern_probe.hero` | 297 -> **301** | `[T]` cast `(void *)`, spelled `T *` |
| `check/ffi.hero` | 282 -> **318** | `fixed_out` (S1), `array_out` (S7) |
| `check/lend_extent.hero` | 183 -> 222 | S2's `stated_extent`, S7's `array_param` and its refusal |
| `cli/pointee_wants.hero` | 142 -> 155 | the width ask through `T[N]` and `[T]` to the element |
| `check/ffi_sweep.hero`, `parse/marks.hero` | 93 -> 100, 251 -> 254 | S1, S2 |

Two files pass §11's 300 (`check/ffi.hero` 318, `emit/extern_probe.hero` 301);
the landing owes a seam (the `_out` predicates are one: what an `@` parameter
may be, a module of about 40 lines) or a DECIDED row. No pass outside
`check/`, `cli/` and `emit/` moved: **no lexer, no IR, no ownership, no
runtime line**. The lowering already gives an `@` array argument the place it
needs (`.inout_arg`), and an element `@xs[i]` its temporary (`ir/inout.hero`).

**The shapes beside it** (`heroes-s7`, `check --brief` and `run`):

| shape | check | run | what |
|---|---|---|---|
| `md` holding 40 nines, `t = md` before the call | 0 | 0 | `md.len()` 32, `md[31]` 173; **`t.len()` 40, `t[0]` 9**: no aliasing |
| a record field `@h.digest`, `Holder {digest: [u8], after: 7}` | 0 | 0 | `h.digest[0]` 186, `h.after` 7 |
| an element `@xs[1]`, `xs: [[u8]]` | 0 | 0 | `xs[1].len()` 32, `xs[1][0]` 186, `xs[0].len()` 0 |
| the same three, `run --sanitize` | — | 0 | 0 `ERROR` lines |
| `pipe(@fds: [i32] counted_by 2)` | 0 | 0 | `0 2 1 0`: two descriptors, one apart, both closed |
| `gethostname(@name: [i8] counted_by 256, namelen: u64)`, `namelen: 256` | 0 | 0 | `0 256 true` |
| `EVP_DigestFinal_ex(c, @md: [u8] counted_by EVP_MAX_MD_SIZE, @s: u32)`, SHA-512 | 0 | 0 | `s` 64, `md.len()` 64, the slice to `s` opens 221 (SHA-512("abc")[0], Python's `hashlib`); `--sanitize` 0, 0 `ERROR` |
| **`@md: [u8] counted_by 16`**, SHA-256 writing 32 | 0 | **134** | prints `16` and an intact `str`, then dies; **`--sanitize` exit 0, 0 `ERROR`** |
| **`@name: [i8] counted_by 4`, `namelen: 256`** | 0 | **134** | `0 4`, then the false *null function pointer* panic |
| `@md: [u8] counted_by n` (a sibling) | 1 | 1 | `ffi_type` and `counted_by_shape`: two messages for one mistake |
| `@md: [u8]`, no extent | 1 | 1 | `ffi_type` (unchanged) |
| `md: [u8] counted_by 32` by value | 1 | 1 | `ffi_type`, `counted_by_shape` |
| `@md: [i8] counted_by 32` against `unsigned char *` | 0 | 1 | `ffi_parameter_type`, with the same wrong `guess` fix as S1 (`@md: u8`) |
| `@md: [str] counted_by 32` | 1 | 1 | `ffi_type`, `counted_by_shape` |

**What S7 does not close, measured**: the extent is still the binding's word.
An understated constant (`counted_by 16` where C writes 32) overruns the
emitter's own buffer, at exit 134 here (the frame's guard, an inference: no
message names it) and **silent under `--sanitize`**, the store being inside
libcrypto as in 396. And where C takes a count by value, a constant extent
beside it ties nothing: `gethostname(@name: [i8] counted_by 4, namelen: u64)`
called with `namelen: 256` is `host_one` again. The robust form of S7 takes
the sibling too (`counted_by namelen`: a buffer of the run-time count, which
needs a heap block or the array grown in place, about +25 lines, **unbuilt**),
so that a binding CAN tie the two; the compiler cannot tell a count parameter
from any other integer, so nothing refuses the untied pair, and the guard below
is what catches it.

**What S1, S2 and S7 do to every program that does not use them, measured**:
the S7 compiler (S1 + S2 + S7) emitting the FROZEN compiler's own source
(`heroes-s7 build selfhost/main.hero --emit-c` in a pristine archive of
`39935f7c`, ended 22:48) is **byte-identical to `seed/heroes.c`** (`cmp` exit
0). So the emitter half changes no emission where no `[T]`, `T[N]` or stated
extent is written, the compiler's own 12 lends included. **The compiler's own
tests under S7** (a snapshot of the S7 tree, ended 22:51): 1,308 tests, the
same one failed (the stale spec pin), so S7 moves no self-test either.

## 5. The compiler's own seven lends (brief item 4)

`seed/heroes.c` holds **12 call sites** passing a cell's address cast
`(void *)&hN_...` (`grep -cE`): `hero_args_shown` 1, `hero_dir_at_shown` 1,
`hero_env_shown` 3, `hero_exe_path_shown` 1, `hero_file_read_shown` 2,
`hero_file_read_str` 1, `hero_run_go` 1, `hero_str_try_from_cstr` 1,
`hero_thread_spawn_sized` 1.

- **S1, S2, S7 change none of them**: no edit to a `selfhost/` binding, and the
  `cmp` above shows the emission of all 12 unchanged. The seed moves only
  because the compiler's source gains the routes' code; the fixpoint is the
  ordinary one (build with the old seed, regenerate, `cmp`), with no chain step,
  since no `selfhost/` file USES a new form.
- **S4 changes all 12** (each becomes a guarded cell), and with them every
  blessed emission that holds such a call: 42 files under `tests/emission` and
  `tests/golden` match `\(void \*\)&h[0-9]+_` (`grep -rlE`). Seed and
  fixpoint mechanical, the emission goldens re-blessed (reviewed, never
  regenerated blind).
- **The default refusal (item 6, below) breaks all 12 lends, in 7 signatures,
  and two more the census missed**: `selfhost/library_source.hero:153` and
  `:164`, `hero_file_read_str(path: str, @status: i64)` and
  `hero_str_try_from_cstr(p: cstr, @status: i64)`, the Heroes library's own
  bindings held as STRING LITERALS, which the shared brief's one-line regex
  (an indented `function` at the start of a line) cannot see. Measured: the
  refusal prototype WITHOUT marking them fails **every program**, `check`
  answering *internal error: a diagnostic landed inside the Heroes library, at
  its line 113: [ffi_one_cell] `@status` of `hero_file_read_str` ...*, exit 2
  on `md_scalar.hero`, `frexp`, all. So the live count is 34 + 2 = **36
  lends, 14 of them in `selfhost/`** (a correction to the census, CL-057), and
  the refusal must land in two commits, as `lent` did: the spelling first (so
  the seed parses `counted_by 1`), then the marks on the compiler's own 9
  signatures and the refusal.

## 6. S5 and S4: what each costs, and what was measured (brief item 3)

### S5, the header's own words

**Does the header probe see an array parameter's declared length today? No,
measured.** The dump unit the probe writes (`extern __typeof__(f) hero_ty_f;`,
`-ast-dump=json -ast-dump-filter=hero_ty_`, `cli/header_types.hero`), run by hand
on `pipe`, `uuid_generate`, `SHA256_Final`, `gethostname` and `ctime_r`
(`<work>/s5/dump.c`, Apple clang 21, exit 0, 6,359 bytes): the parameters read
`int *`, `unsigned char *`, `unsigned char *`, `char *`, `char *`. `int
pipe(int [2])` (`unistd.h:482`) arrives decayed; clang's own text dump of the
original declaration reads `ParmVarDecl ... 'int *'` too. **With
`-fbounds-safety`** (Apple clang only) the same `__typeof__` dump reads
`char *__single __counted_by(26)` for `ctime_r` and `char *__single
__counted_by(__namelen)` for `gethostname`; `-D__LIBC_STAGED_BOUNDS_SAFETY_ATTRIBUTES`
without the flag reads plain `char *` (`_bounds.h` expands `_LIBC_COUNT(x)` to
nothing otherwise, `_bounds.h:49`).

**The critic's redeclaration mechanism, re-run** (`<work>/s5/redecl.c`,
`-Werror=array-parameter`): `pipe(int fds[1])` and `uuid_generate(unsigned char
out[1])` are errors *with mismatched bound*; `SHA256_Final`, `gethostname`,
`ctime_r` silent. Two of my five.

**Cost, unbuilt, priced on the files that would carry it**: the redeclaration
needs the function's whole prototype rewritten at one parameter, from the
dump's `desugaredQualType` (`int (int *)`), in **its own unit**: a redeclaration
that does not match exactly (a Windows `__stdcall`, a variadic, an attribute
clang folds into the type) is `conflicting types`, a hard error that would sink
every other ask in a shared unit and read as the compiler's own fault (exit 2).
So: a third clang unit per cold build (cached as the other two are,
`cli/pointee.hero`), its writer in `cli/header_types.hero` (291, about +35,
past 300: a new module), a reader for `-Warray-parameter` lines mapped back to
the parameter by `#line` (`emit/ffi_pointee.hero`, about +40). **About 100
lines, three modules, one more clang run.** With S1 or S7 it compares the
binding's N against the header's bound, both ways; with an unmarked cell N is 1.
The `-fbounds-safety` reading is cheaper (a `__counted_by(` scan of a text the
dump already returns, about +40) and **Mac-only**: a binding refused here and
accepted on Linux, a verdict that depends on the platform for a program that is
wrong on both. OpenSSL, the case the defect is filed on, says nothing on any
platform (the critic: 0 files), so **S5 cannot close 396**; it is a net beside
the rule.

### S4, a run-time guard

**Built by hand in C, not in the compiler** (`<work>/s4/s4.c`, Apple clang 21,
`-O2`): the cell inside `struct { uint8_t cell; uint8_t canary[16]; }`, the
canary filled before the call and compared after it. `SHA256_Final` through a
plain one-byte cell: **exit 138** (SIGBUS); through the guarded cell: **exit
134 with the named panic** *C wrote past the one cell lent to `md` of
`SHA256_Final`*. Static instructions of the function at `-O2`: **11 plain, 38
guarded** (27 more per call site, the panic path included; not a timing).

**What it misses**: a write that lands bytes equal to the canary (2^-8 per
byte, so a 16-byte canary misses only a write that reproduces it); a pointer C
keeps and writes through after the call returns (the check has already run);
an over-READ (an input C reads past the cell, which the canary cannot see); and
the verdict is at run time, on the paths a run takes, never at `check`. It
stops 396 after the fact: the 31 bytes are written, and the abort comes before
the program reads them.

**Cost, unbuilt**: an emitter module of the shape `emit/buffer_out.hero` already
is (the cell copied into a guard local, the canary written and compared, the
cell copied back), **about 60 to 80 lines**, `emit/ops.hero` +3; no checker and
no grammar line. It changes the emission of every scalar `@` extern call: the
compiler's own 12 (the seed moves), 42 blessed emission files.

## 7. S0: where N bytes live (brief item 5)

- **A local fixed array** (`md: u8[32] @ ...`): today `fixed_outside_a_group`
  (`check/ffi_sweep.hero`), and the emitter gives `T[N]` no storage on purpose:
  `emit/ctype.hero:384` answers *C has no assignable array*, and
  `emit/gate.hero`'s `check_fixed_flow` (panel 081 R3) refuses a fixed array
  named or stored. Admitting it makes `T[N]` a first-class value (copy, `==`,
  storage of its own), and the type kind is matched at **257 `.fixed` sites in
  84 `selfhost/` files, 121 of them in `emit/` and `ir/`** (`grep -rn`), each a
  question about storage the emitter has never answered. That is a core
  construct the checker, the lowering and the backend must all carry: **several
  hundred lines, unbuilt**, and Principle 0 reads 0 for it (no compiler need).
- **A `[u8]` lent through `.ptr()` with a run-time length check**: today
  `bad_operand` (`check/lend_types.hero`). It needs the array unshared before C
  writes (the address is into a refcounted heap block: without the unshare a
  copy made before the call changes, §3's no aliasing at exit 0), the address
  `hero_array_at_mut(.., 0)` (which aborts on an empty array, so the program
  must size it first, and `repeat` takes only a `str`: `[0].repeat(32)` is `bad_operand`, measured), and route C's
  run-time compare against `hero_array_len` instead of `sizeof`. **About 60 to
  100 lines across `check/lend_types.hero`, `emit/field_lend.hero` and
  `emit/lend_extent.hero`, unbuilt.**
- **S7 makes both unnecessary for C's out-buffers**: the array is the place, its
  length is set by the copy-out, and the bytes C writes land in a buffer whose
  size the emitter chose. S0 stays a question for a C function that KEEPS the
  buffer (a stream's `setvbuf`), which S7 cannot serve (its buffer dies with the
  call) and which no route here reaches.

## 8. The default when a binding says nothing (brief item 6), prototyped

**Built** on top of S7, `heroes-r` (22:53): an unmarked scalar `@` on an extern
(`int`, float or `bool`, no `counted_by`) is refused, `error[ffi_one_cell]`
on the parameter's name; **the marked spelling of one element is
`counted_by 1`**, S2's stated extent on a cell, and any other stated number on
a cell is refused, naming S7's form. Four files, **48 insertions, 3
deletions** (`check/lend_extent.hero` 222 -> 248, `ffi_errors.hero` 212 ->
223, `check/ffi.hero` +1, `library_source.hero` the library's two marks). It is
`check`-side: no header is asked, since the refusal is of the binding's
silence, not of a header's word.

| probe | check | run | what |
|---|---|---|---|
| `md_scalar` (`@md: u8`) | 1 | 1 | `ffi_one_cell` 10:28 |
| `pipe_cell` (`@fds: i32`) | 1 | 1 | `ffi_one_cell` 6:20 |
| `host_one` (`@name: i8`, a count beside it) | 1 | 1 | `ffi_one_cell` 7:27 |
| `@md: u8 counted_by 1` | 0 | 0 | **`1 1 186`: the fault, now under a word the binding wrote** |
| `@md: u8 counted_by 32` | 1 | 1 | `counted_by_shape`, *a cell holds one ... lend an array: `@md: [T] counted_by N`* |
| `frexp(x: f64, @e: i32 counted_by 1)` | 0 | 0 | `0.5 4` |
| the compiler's own source | 1 | — | 12 `ffi_one_cell` at the 7 lines the census names |

**The census, frozen compiler against `heroes-r`, `check --brief` on all 2,740
tracked `.hero` files outside `archive/` and `site/`** (`git ls-tree` of
`39935f7c`, `xargs -P 8`, 22:54 to about 22:57): **59 files change, 54 from
exit 0 to 1 and 0 the other way**, and in every one of the 59 the only change
is the added `ffi_one_cell` (283 of them: a module's lends count once per root
that reaches it). By tree: `tests/harness/` 30 (every root reaching
`shell.hero` or `cases.hero`), `docs/panel/` 8, `tests/golden/run` 8,
`tests/golden/check` 4 (their diagnostics change, exit already 1),
`tests/golden/fixedbugs` 4, `tests/golden/unsupported` 2, `selfhost/` 3 roots
(`main.hero`, `modules.hero`, `rename_fit.hero`). **The same census reads
S1, S2 and S7 as changing no tracked file's `check` outcome**, since they ride
in `heroes-r` and no row differs by anything else.

**What it would cost to land**: the checker (about 50 lines, built); 36 lends
re-marked in 26 signatures, 14 of the lends the compiler's own (the seed moves
in two commits, the spelling first); 18 golden cases re-annotated or rewritten
(a `fixedbugs/` case pinning a header refusal must keep reaching it, so it
gains `counted_by 1`); 30 net roots rebuilt through 5 marked lines of
`tests/harness/`. **What it buys, measured**: `md_scalar`, `pipe_cell` and
`host_one` stop at `check`. **What it does not buy**: `counted_by 1` on
`SHA256_Final` builds and corrupts exactly as before. The refusal turns an
assertion nobody wrote into one somebody wrote; it verifies nothing.

## 9. The three shapes beside 396, route by route (brief item 5)

Each run here unless marked; `pipe` and `gethostname` on this Mac, OpenSSL
4.0.3.

| | `pipe(int [2])` | `gethostname(char *, size_t)` | `EVP_DigestFinal_ex` out-count |
|---|---|---|---|
| today | `@fds: i32` runs, `after` 4 | `@name: i8` 134 | `counted_by s`: 512 overruns, 134 |
| S1 | `@fds: i32[2]` on a header-struct field: correct (`after` 7); `i32[1]`: `after` 4 at exit 0 | `@name: i8[16]`, `namelen: 64`: **134**, `after` 0 (the count beside it is tied to nothing) | `@md: u8[64]` correct by its N; a smaller N is the same lie |
| S2 | **not reachable**: `.ptr()` on `i32[2]` is `bad_operand` (bytes only) | today's route C already serves it: `ptr counted_by namelen` on a byte field | `counted_by EVP_MAX_MD_SIZE`: `u8[32]` refused at build, `u8[64]` runs, `after` 7 |
| S7 | `@fds: [i32] counted_by 2`: `0 2 1 0`, correct | `counted_by 256`, `namelen: 256` correct; **`counted_by 4`, `namelen: 256`: 134** | `@md: [u8] counted_by EVP_MAX_MD_SIZE`: `s` 64, the slice opens 221, `--sanitize` 0 |
| default refusal | refused (`ffi_one_cell`) | refused | untouched (the out-count is a `counted_by` already) |
| S4 (by hand) | would abort after the call (unrun on `pipe`) | would abort (unrun) | untouched (no cell) |
| S5 | `int [2]` caught by the redeclaration (measured) | `_LIBC_COUNT` seen only under `-fbounds-safety` (Mac) | nothing (OpenSSL says nothing) |

**The pattern the table measures**: every static route turns N into the
binding's word, and the word can be wrong; S7 is the only one that also gives
the program a place for the bytes and the compiler a buffer of its own size.
Where C takes a count by value beside the buffer (`gethostname`, `read`),
S7's constant is the wrong form: the count must BE the extent (route C's
sibling, extended to an `@` array), or the program can state two numbers that
disagree.

## 10. S7 with S4's guard on its own buffer, built

The one mechanism in this sitting that sees the binding's number being wrong
is a canary, and S7 is the one route where the emitter already owns both the
buffer and a block around the call, so the guard costs it almost nothing.
**Built** in `emit/buffer_out.hero` (`heroes-s7g`, 22:59, on the S7 +
default-refusal tree): the buffer becomes `struct { T b[N]; unsigned char
g[16]; }`, the 16 bytes set to `0xA5` before the call and compared after it,
with a named `hero_panic`. **12 insertions, 4 deletions; the module 100 -> 108
code_lines.**

| probe (`heroes-s7g run`) | exit | output |
|---|---|---|
| `SHA256_Final(@md: [u8] counted_by SHA256_DIGEST_LENGTH)` | 0 | `32 186 173` |
| the three places (copy, field, element) | 0 | unchanged from §4 |
| `pipe(@fds: [i32] counted_by 2)` | 0 | `2 1 0` |
| EVP SHA-512 into `counted_by EVP_MAX_MD_SIZE` (`@s: u32 counted_by 1`) | 0 | `64 64 221` |
| **`counted_by 16`, SHA-256 writing 32** | **134** | *panic: `SHA256_Final` wrote past the 16 elements lent to `md`, the extent its declaration states* |
| the same, `run --sanitize` | **134** | the same panic (ASan alone said nothing, §4) |
| **`gethostname(@name: [i8] counted_by 4)`, `namelen: 256`** | **134** | *panic: `gethostname` wrote past the 4 elements lent to `name` ...* (was the false *null function pointer* panic) |

What it still misses is S4's list (§6): an overrun whose bytes over the guard
happen to equal the pattern, and a pointer C keeps. The ffi-pragmatist's guard
page (a `PROT_NONE` page the buffer ends against) misses neither the first nor
any overrun length; it moves the cost into the runtime (a guarded allocation
per platform, mmap or VirtualAlloc, and an ABI bump), about 60 to 100 lines of
C by my estimate, **unbuilt here**. A real landing would
size the guard to the element and put `file:line` in the message as route C's
run-time compare does (`emit/lend_extent.hero`'s `compared`).

## 11. Verdicts, one per route

The ceiling's question first, for all of them: **no route on the table adds a
Part 5 core construct except S0's local fixed array.** S1, S2 and S7 are
checker admissions plus emitter work at the extern call, the home
`guard_arguments`, the null-`cstr` guard and the `owned` cell already share;
no route touches the lexer, `ir/` or the ownership pass, and the built ones
leave the runtime and its ABI alone. Everything built fits one person: the
whole of S1 + S2 + S7 + its guard + the default refusal is 12 files, 385
insertions and 16 deletions (`diff -ruN` against the frozen `selfhost/`).

### U, the ruling: an unmarked `@` is one element
- `verdict`: approve
- `section`: §1.7 (no compiler line), §4.19
- `implementation_cost`: 0 `selfhost/` lines; a spec sentence
- `needed_for_self_hosting`: no (it describes the compiler's 14 lends)
- `argument`: It states what every live lend already keeps and costs the compiler nothing. **Alone it closes nothing**: `md_scalar.hero` still exits 0 with the digest in its frame, so U is the ruling a guard enforces (S4 below) and S7 gives a way out of; without them it is a sentence about a fault the compiler still accepts.
- `prediction`: under U alone, `heroes run` of `docs/panel/194-evidence/new-defects/single-cell/md_scalar.hero` still exits 0, at the landing.
- `condition`: none; it is the minimum.

### S7, `@md: [u8] counted_by N`, an emitter-owned buffer, with its guard
- `verdict`: **approve**, conditioned
- `section`: §1.12 (*the boundary is complete*: `SHA256_Final` is unbindable without hand-written C today, and the lease entered on §1.12 by the same argument, design.md §4.19's fourth case); §1.7 and Part 5 (no core construct: §4.8's copy in, copy out with the extent the binding's)
- `implementation_cost`: **built**: S2 + S7 + guard over S1, 9 files, about 300 insertions; `emit/buffer_out.hero` new (108 code_lines), `emit/ops.hero` +6, `emit/lend_count.hero` +18, `check/ffi.hero` and `emit/extern_probe.hero` past 300 (a seam owed); no lexer, IR, ownership or runtime line (canary form)
- `needed_for_self_hosting`: no
- `argument`: The only route that builds and runs `SHA256_Final` from one file (measured `1 1 1 32 186 173`), and also `pipe`, `gethostname` and the EVP out-count. It emits nothing new where unused (`cmp` 0 on the frozen compiler's source). Strongest reason it is wrong: N is still the binding's word, so an understated N overruns the emitter's buffer, silent under `--sanitize` (measured); unguarded, S7 merely moves 396 into a buffer the compiler owns. The 16-byte guard turns that into a named abort (measured, `--sanitize` too) for 12 more lines. Second: 2N runtime calls per call for the copies.
- `prediction`: the landing of U + S2 + S7 + guard reads **250 to 450 `selfhost/` insertions with no file under `selfhost/ir/`, `selfhost/lexer.hero` or `runtime/` changed (canary form)**; its compiler emits the parent's `selfhost/main.hero` **byte-identical to the parent's `seed/heroes.c`**; and a `check` census of the tracked `.hero` files moves **0** outcomes. Checkable at the landing commit in M-buildable-structs.
- `condition`: **veto, on soundness (§1.12; the brief's veto), if S7 lands without a guard** (an unguarded understated N is 396 in the emitter's own frame, silent under `--sanitize`, measured); object if the sibling form (`counted_by namelen`, unbuilt) is left out, since a constant beside a by-value count is `host_one` again (measured 134); object if `ffi_pointee`'s `guess` fix still proposes `@md: u8` for an array (it writes the fault, measured on S1 and S7). Lifted to plain approve by a reader test showing it used.

### S2, `counted_by` a constant or a literal
- `verdict`: approve, as S7's spelling of a stated extent (one rule)
- `section`: §1.7 (one word, two forms, one checker function and one emitter function), §4.19
- `implementation_cost`: **built**: 80 insertions, 3 files; the constant spelling needs no parser line, the literal moves `CParam` (spec line 447) and the `grammar` suite (+4 parser lines); a stated-extent message owed in `field_lend_errors.hero` (about +20, unbuilt)
- `needed_for_self_hosting`: no
- `argument`: It checks a constant extent at build on a field (measured refusal on `u8[16]`) and gives the EVP out-count its honest binding (`EVP_MAX_MD_SIZE`, measured). **Strongest reason it is wrong, and it is the one I most want the synthesis to keep**: built `check`-side alone it checks nothing, because `lend_count.counted_siblings` answers -1 and `lend_extent.assertions` emits no assertion for a -1; and its `guess` fix rewrites the binding's 32 to the field's 16, writing the lie.
- `prediction`: in the landing, a golden case with `counted_by 32` over a `u8[16]` field exits 1 at `build` on all three CI legs; a build that admits the spelling without the emitter half exits 0 on it (the check this prediction exists to catch).
- `condition`: object if the emitter half is not in the same commit as the spelling, or if any `Fix` offers to rewrite a stated extent.

### S1, `@md: u8[32]`
- `verdict`: object
- `section`: §1.7 (*does it remove a special case?* It adds a third spelling of one buffer)
- `implementation_cost`: **built**: 48 insertions, 10 deletions, 4 files, no new module, no grammar change
- `needed_for_self_hosting`: no
- `argument`: Cheap and correct where it reaches (the field's address and nothing else, measured), but its place is a group record's field, so a hand-written header struct; `pipe(@fds: i32[1])` corrupts at exit 0 under its word (measured); `gethostname` with `i8[16]` and `namelen: 64` exits 134 (measured); its `guess` fix proposes `@md: u8`, the fault itself. Dominated by S7, which needs no struct.
- `prediction`: an S1 compiler binds `SHA256_Final` from one file in 0 ways: every one-file attempt reads `type_mismatch`, `fixed_outside_a_group` or `ffi_unknown_tag`.
- `condition`: approve if S7 is refused and S1 lands with its `guess` fix repaired.

### S4, a run-time guard on an unmarked scalar cell
- `verdict`: **approve** as a backend check, not a promise in the spec
- `section`: §1.12 (*check rather than assume ... one predictable branch per FFI argument, and it is paid deliberately*); CLAUDE.md § Precedence (robustness outranks speed)
- `implementation_cost`: **unbuilt in the compiler**; hand-built in C: 11 to 38 static instructions per call site at `-O2`, `SHA256_Final` through a one-byte cell 138 plain against 134 with a named panic; an emitter module of `buffer_out.hero`'s shape, about 60 to 80 lines; it changes the compiler's 12 call sites (seed mechanical) and 42 blessed emission files
- `needed_for_self_hosting`: no
- `argument`: **It is the only route that stops defect 396's own reproducer without refusing a correct program**: U + S2 + S7 leave `md_scalar.hero` at exit 0, and the default refusal refuses the correct `frexp` (the spec-warden's readers). Strongest reason against: a per-call cost on every scalar `@`, and a detection after the store; but the abort comes before anything reads the frame, and the guard page (the ffi-pragmatist's) removes the pattern miss at the runtime's cost.
- `prediction`: with S4 landed, `heroes run md_scalar.hero` exits 134 with a panic naming `SHA256_Final`'s `md`, and the compiler's own tests read their parent's count.
- `condition`: object if it copies a record lend (the ffi-pragmatist's `z_stream` veto, which I join), or if the spec promises more than the guard catches.

### The default refusal (S3 widened), `counted_by 1` the mark of one
- `verdict`: object
- `section`: §1.7 (cost without a check), §1.12 (*it does not suspend Principle 0*)
- `implementation_cost`: **built**: 48 insertions, 4 files; the census moves **59 files, 54 from exit 0 to 1**; 36 lends in 26 signatures re-marked, 14 of them the compiler's own, the seed in two commits
- `needed_for_self_hosting`: no
- `argument`: It catches `md_scalar`, `pipe_cell` and `host_one` at `check` (measured), and verifies nothing: `@md: u8 counted_by 1` builds and corrupts exactly as before (measured `1 1 186`). It writes a word on 14 compiler lends that each store one value (44 stores), and refuses the commonest correct first try (`frexp`). S4 catches the same three at run time with no word and no refused program.
- `prediction`: landed, the 59-file census above reproduces within 2 files on the landing's parent.
- `condition`: approve if S4 is refused and a reader test shows the frozen arm writing the one-cell fault at least 3 of 10 times.

### S5, the header's own words
- `verdict`: approve as a complement, in its redeclaration form only; object to the `-fbounds-safety` form
- `section`: §4.19 (*clang checks ... against that header*), §1.12
- `implementation_cost`: unbuilt; about 100 lines in three modules and a third clang unit per cold build (§6); the `-fbounds-safety` reading about 40 lines and Mac-only
- `needed_for_self_hosting`: no
- `argument`: The probe does not see `int [2]` today (measured: the dump reads `int *`); the redeclaration does (`pipe`, `uuid_generate`), and with S1 or S7 it compares N both ways. It cannot touch OpenSSL. The `-fbounds-safety` form makes one binding's verdict depend on the platform.
- `prediction`: a prototype refuses `critic/pipe_cell.hero` and none of the 36 live lends.
- `condition`: object if its unit shares the check unit (a `conflicting types` there sinks every other ask as the compiler's fault).

### S6, a sentence alone; S1', a one-field record to `T *`; S3 narrow; Nothing
- `verdict`: object to each
- `section`: §1.12 (S6 and Nothing leave hand-written C as the answer); §1.7 (S1' relaxes today's pointee refusal for a form S7 covers; S3 narrow misses `pipe` and refuses 163's correct `char *`)
- `implementation_cost`: S6 and Nothing 0 lines; S1' and S3 narrow unbuilt
- `needed_for_self_hosting`: no
- `argument`: none of them changes what `md_scalar.hero` does, and S6 writes down that a C library needs C by hand.
- `prediction`: under any of them `md_scalar.hero` exits 0 at the landing.
- `condition`: none beyond the routes above.

### S0
- `verdict`: **veto** on a local fixed array; object to the in-place `[u8].ptr()` until it is built
- `section`: Part 5 and §1.7 (a fixed array as a first-class value is a core construct the checker, the lowering and the backend must all carry); Principle 0
- `implementation_cost`: unbuilt; the local fixed array reaches 257 `.fixed` match sites in 84 files, several hundred lines; the in-place lend about 60 to 100 lines with an unshare
- `needed_for_self_hosting`: no
- `argument`: S7 already gives the bytes a home with no new value kind. The in-place lend is what the ffi-pragmatist asks for count-carrying buffers (`read`, `recv`), and it is faster than S7's copy, but it hands C an address inside a refcounted block, so §3 holds only if the unshare is right, which nothing here has measured.
- `prediction`: none for the veto; for the in-place lend, a prototype keeps a copy taken before the call unchanged (as S7's `t` did) or it is wrong.
- `condition`: the veto lifts if a measured compiler need for a local `T[N]` appears; the objection lifts on a built prototype passing that case and route C's count check against `hero_array_len`.

## 12. Found beside the routes

- **The census of live scalar lends is 36, not 34**: the Heroes library's own
  two bindings, `selfhost/library_source.hero:153` and `:164`, are string
  literals the shared brief's regex cannot see, and a refusal that forgets them
  fails every program with an internal error (measured, §5).
- **`emit/ffi_pointee.hero`'s `guess` fix drops an extent**: on `@md: i8[32]`,
  `u32[8]` and `[i8] counted_by 32` it proposes `@md: u8`, which is 396's own
  binding. A prototype finding (no such form exists on the frozen tree); owed by
  S1's or S7's landing.
- **S2's `field_lend_extent` fix rewrites a stated extent**: owed by S2's
  landing, never a `certain` fix and better no fix.
- **Two messages for one mistake** on `@md: [u8] counted_by n` and on a
  by-value `md: [u8] counted_by 32` (`ffi_type` and `counted_by_shape` on one
  parameter): owed by S7's landing.
- **S7 copies element by element**, one `hero_array_at` and one
  `hero_array_push_owned` per element; a runtime entry copying N bytes at once
  would make it one `memcpy` each way, at the price of an ABI bump. Not a
  measurement of speed: a count of calls in the emitted C.

Nothing in the repository or any worktree was edited; no paid run; no timing
(the instruction counts are static counts of `-O2` assembly). My copy:
`<scratchpad>/196-compiler-engineer/` (compilers `heroes-s1`, `heroes-s2`,
`heroes-s7`, `heroes-r`, `heroes-s7g`; probes under `ce/`); diffs and the
census under `<scratchpad>/196-compiler-engineer-work/` (`s1.diff`,
`s2.diff`, `s7.diff`, `r.diff`, `census.tsv`).
