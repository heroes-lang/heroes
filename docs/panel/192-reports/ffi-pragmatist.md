# Panel 192, the ffi-pragmatist: Q4 (a NUL at the C boundary, the runtime's doors) and Q5 (`args()` on Windows)

Started 2026-10-04 18:16:20 (from `date`). Written as I go; a section that
says *in progress* is not finished.

**My copy**: `<scratchpad>/192-ffi-pragmatist/`, from `git -C <trunk>
archive 4c3524fb | tar -x`. Seed sha256 begins `c79ffd5ad005c301`,
35,206,983 bytes. Compiler built from it 18:16:35 to 18:16:39:
`shasum -a 1 heroes` reads `8084f018f53875362048cc0230d23207976d8b00`. All
three match `00-facts.md`'s base. Programs and probes live in
`<scratchpad>/192-ffi-pragmatist-work/`; each route is its own copy,
`<scratchpad>/192-ffi-pragmatist-r<X>/`, from the same archive.

## Status

- Q4: in progress (measured facts below; routes being built).
- Q5: waits on the Windows box (held by batch 10's leg, then lane
  b11-windows). The Mac and Linux arm64 halves are done here first.

## Q4, measured on the base (18:26 to 18:30)

### F4 reproduces

`192-ffi-pragmatist-work/f4-base/`, F4's four programs copied and built with
my base compiler, each `build` exit 0:
- `nulc` prints `1`, exit 0;
- `rf` prints `3`, `1`, exit 0;
- `qi-read` prints `3`, `false`, `the file named a`, exit 0;
- `qi-write` prints `11`, `false`, exit 0, and writes `out-a` holding
  `written`.

### Where a program's `str` gets a NUL: one more door than the critic's

`192-ffi-pragmatist-work/sources/sources.hero` with a header of my own,
`nulsrc.h`, whose C hands back the bytes `61 00 62`:

| constructor | what the program holds |
|---|---|
| `nul_three().validated()` (a `cstr` over `a\0b`) | length 1: `strlen` |
| `[97, 0, 98].validated_bytes()` | length 1: `memchr`, to the first zero |
| `nul_owned()`, `-> cstr owned free` | length 1 |
| `nul_str()`, `-> str`, whose C is `return hero_str_from_bytes("a\0b", 3);` | **length 3** |

So **C itself is a third door**, beside a raw literal and `read_file`: a
binding's own C returning `str` through `hero_str_from_bytes` with an
explicit length, which design.md §4.20 names as the constructor that makes
§4.19's ladder step 3 writable. The critic's *"after Q1, a program's NUL
comes from `read_file`"* is false by this door.

**The constructors are a closed set, by the code.** `runtime.c` includes
every part into one translation unit (`:117` to `:200`), so every heap `str`
is born in the static `hero_str_alloc` (`str.c:46`), whose callers are 11
lines: `str.c` 155 (`repeat`), 165 (`concat`), 225 (`slice`), 306
(`from_bytes`), 347 (`try_from_cstr`), 387 (`try_from_bytes`); `f64.c` 222,
236, 256, 268 (number to text); `text.c:120` (`join`). The two static forms
are the literal macro `HERO_STR_STATIC` (`heroes_runtime.h:186`, length
`sizeof(text) - 1`, which is how a raw NUL in a literal is kept) and
`hero_empty_block` (`str.c:24`). Of these, the ones that copy bytes from
outside a `str` with a length the caller states are `from_bytes` alone;
`try_from_cstr` and `try_from_bytes` stop at the first zero; `repeat`,
`concat`, `slice`, `join` copy only bytes already in a `str`; the numbers
write digits. **So the doors through which a NUL enters a `str` are three:
the literal (compile time), and at run time every caller of
`hero_str_from_bytes` with a length not from `strlen`**: `hero_file_read`
(`os.c:814`), `hero_bytes_shown` (`os.c:649`, `:703`, the compiler's shown
read of a file and of the environment), `chars()` (`text.c:87`, a piece of
a `str` that already holds it), `hero_run_win_command_line` (`run.c:210`,
Windows, words already `strlen`ed), and any binding's C.

### The doors, enumerated at the parameter rather than by vocabulary

A grep of 120 call names (POSIX, C and Win32 calls that take a name),
comment lines dropped, prints 75 lines in six parts; mapped to their
enclosing functions (a script over the grep), every one sits under a
`hero_os.h` door or a static helper of one. **And it still missed one**:
`hero_fs_deny_delete` (`replace.c:737`) reaches the system through
`acl_get_file` and `acl_set_file`, which no vocabulary of mine named. So the
enumeration that is complete by construction is the door's parameter list:
a C function receives a name only through a parameter or a global, and the
globals here are `hero_run_words` (filled by `hero_run_arg`) and `hero_argv`.

**`hero_os.h` declares 27 doors whose 36 name parameters are
`const char *`**, and one that takes the `str`:

| door | name parameters | reaches |
|---|---|---|
| `hero_file_read`, `hero_file_read_shown` | `path` | `fopen` (`os.c:747`) |
| `hero_env_shown` | `name` | `getenv` |
| `hero_file_write` | `path` | `fopen` (`os.c:857`) |
| `hero_fs_exists`, `hero_fs_is_directory` | `path` | `stat`, `GetFileAttributesA` |
| `hero_fs_mkdir_all` | `path` | `mkdir`, `_mkdir` |
| `hero_fs_remove` | `path` | `remove`, `RemoveDirectoryA` |
| `hero_fs_newer_than` | `path`, `reference` | `stat`, `GetFileAttributesExA` |
| `hero_fs_rename` | `from`, `to` | `rename`, `MoveFileExA` |
| `hero_fs_landing` | `path` | `lstat`, `readlink`, `CreateFileA`, `FindFirstFileA` |
| `hero_fs_kind`, `hero_fs_links`, `hero_fs_writable` | `path` | `lstat`, `stat`, `open`, `CreateFileA` |
| `hero_file_stage` | `staged`, `like` | `open`, `stat`, `unlink`, `CreateFileA`, `DeleteFileA`, `Get`/`SetFileAttributesA` |
| `hero_fs_replace` | `staged`, `path` | `rename`, `open` (the directory), `MoveFileExA` |
| `hero_file_unstage` | `staged` | `unlink`, `DeleteFileA` |
| `hero_fs_mode`, `hero_fs_set_mode` | `path` | `stat`, `chmod`, `Get`/`SetFileAttributesA` |
| `hero_fs_link`, `hero_fs_symlink` | two each | `link`, `symlink`, `CreateHardLinkA`, `CreateSymbolicLinkA` |
| `hero_fs_deny_delete` | `path` | `acl_get_file`, `acl_set_file` (macOS) |
| `hero_fs_flagged`, `hero_fs_set_flag` | `path` | `stat`, `open`, `chflags`, `Get`/`SetFileAttributesA` |
| `hero_dir_scan`, `hero_dir_remove_tree` | `root`, `path` | `opendir`, `FindFirstFileA`, then `hero_fs_remove` |
| `hero_run_go` | `program`, `in_path`, `out_path`, `err_path` | `execvp`, `open` (`run.c:378` to `:411`), `CreateFileA`, `CreateProcessA` |
| **`hero_run_arg`** | **`HeroStr word`** | `execvp`'s words, `CreateProcessA`'s line |

A program can reach every one: an `extern "hero_os.h"` group in a program's
own module is accepted (the compiler's own tests declare `hero_file_read`
that way, `check/contracts.hero:324`), and the prelude reaches two,
`read_file` and `write_file` (`library_source.hero:208`, `:222`).

## Resumed 2026-10-04 21:07, paused by the author at 21:26, resumed 2026-10-05 00:39 (each from `date`)

Appended at 00:40; what follows was measured between 21:07 and 21:26 and is
written down only now. From here this report is appended to, never
rewritten.

### Three routes built, each its own copy

Each copy is `git archive 4c3524fb`, with the base compiler (sha1
`8084f018f5387536`) copied in as `heroes-base`, which built the route's
compiler from `selfhost/` inside the copy (so the route's runtime is the one
beside it):

- **rA**, `<scratchpad>/192-ffi-pragmatist-rA/`: a scan at the lend. A new
  runtime function `hero_str_lend` (`memchr` over the `str`'s length, a
  panic naming the byte), emitted at `selfhost/emit/inst.hero:316` instead of
  `hero_str_cstr`; the same check at the top of `hero_str_held`, the lease.
  `hero_str_cstr` itself is untouched, so its four runtime callers that pass
  a length (`os.c:864`, `:913`, `replace.c:440`, `:507`) stay correct.
  Built 21:11:45 to 21:13:17, exit 0, compiler sha1 `3cde0484ab92a3d5`.
- **rB**, `<scratchpad>/192-ffi-pragmatist-rB/`: a fact kept per `str`. A
  second value of the header's existing magic word, `HERO_STR_MAGIC_NUL`
  (`"HEROSNUL"`), so `HeroStrHeader`'s layout does not move. Set where a
  block is made: `hero_str_from_bytes` notes a zero inside the UTF-8 walk it
  already makes; `concat` ORs its two inputs' facts; `repeat` inherits;
  `slice` keeps the fact only when the source holds one and a `memchr` of
  the slice finds it; `join` ORs its parts and, with two parts or more, its
  separator; the literal macro `HERO_STR_STATIC` computes it at C compile
  time with `__builtin_strlen(text) + 1 == sizeof(text)` (clang folds it in
  a static initializer under `-pedantic` with no warning: my `fold.c`,
  `192-ffi-pragmatist-work/fold/`, printed both magic values). The lend
  `hero_str_lend` and the lease ask the fact: one load, one compare.
  `hero_str_hdr_checked` accepts both values. Built 21:14:45 to 21:16:29,
  exit 0, sha1 `bd33ae46479e56ba`.
- **rD**, `<scratchpad>/192-ffi-pragmatist-rD/`: the NUL refused in the walk
  every constructor already makes, as the shared brief describes it.
  `hero_str_from_bytes` aborts on a zero (*the bytes hold a NUL, which no
  str holds*); `hero_file_read`'s pre-check answers a new status
  `HERO_OS_HOLDS_NUL` (5, `hero_os.h`), which the prelude's `read_file`
  turns into `not_text` with the message *the bytes of <path> hold a NUL
  byte, which a str does not hold* (the existing *are not UTF-8* would be a
  false sentence for a NUL). `.cstr()` untouched. Built 21:18:40 to
  21:20:21, exit 0, sha1 `975a913555312239`.

### F4's programs under each route (21:13 to 21:21)

| program | base | rA | rB | rD |
|---|---|---|---|---|
| `nulc` (raw NUL literal, `strlen`) | `1`, exit 0 | panic at the lend, *byte 1 of 3*, 134 | the same, 134 | **the compiler aborts building it**, 134 |
| `rf` (`read_file`, `strlen`) | `3`, `1`, exit 0 | `3`, panic at the lend, 134 | the same, 134 | `read_file` fails `not_text`, `.must()` panics with it, 134 |
| `qi-read` | `3`, `false`, `the file named a` | `3`, panic at the prelude's `path.cstr()`, 134 | the same | fails at its first `read_file`, 134 |
| `qi-write` | `11`, `false`, writes `out-a` | `11`, panic at the lend (*byte 5 of 11*), 134, no file | the same | fails at its first `read_file`, 134, no file |
| `qi-read-reads` (mine: `qi-read` reading the failure, no `.must()`) | | | | `true`, `not_text: the bytes of nul.txt hold a NUL byte, which a str does not hold`, exit 0 |

Under rA and rB no other file is opened, but **`read_file` and
`write_file` reach neither `.ok` nor `.err`**: the prelude lends
`path.cstr()` (`library_source.hero:208`, `:222`) before the door can
answer, so the lend's check runs first, in the emitted call at
`inst.hero:316`'s site. That is below the floor for the two doors whose
types promise a failure (`read_file -> str?`, `write_file -> ()?`), the
shape `os.c:803-808` records as defect 002.

**Route (d) as briefed crashes the compiler on a source file holding a
NUL**: `heroes check` of `nulc.hero` and of a file holding a NUL in a
comment (`nulcomment.hero`) reads exit 0 on the base and **exit 134,
`panic: hero_str_from_bytes: the bytes hold a NUL, which no str holds`**,
on rD. The compiler reads its sources through `hero_file_read_shown`, whose
`hero_bytes_shown` builds through `hero_str_from_bytes` (`os.c:649`), and it
calls `chars()` on slices of its source to count columns (`source.hero:268`,
`text_lines.hero:163`, `diag_render.hero:112`), each piece built through
`hero_str_from_bytes` (`text.c:87`). So route (d) cannot land without the
compiler's own read deciding what a NUL in a `.hero` file is, which is the
compiler-engineer's Q1 and Q2, and its `not_text` message says *not UTF-8*
(`selfhost/not_text.hero:65`), false for a NUL.

### Route (b)'s fact against route (a)'s scan, at the shapes beside (21:2x)

`192-ffi-pragmatist-work/shapes/shapes.hero`: one case per run, each
lending one `str` to `strlen`, the NUL taken from `nul.txt`. On all 14
cases rA and rB agree with each other, and both refuse exactly the 7 where
the base's C reads fewer bytes than the `str` holds:

| case | base: `len`, C reads | rA, rB |
|---|---|---|
| `slice-before` (0..1), `slice-after` (2..3) | 1, 1 | allowed |
| `slice-across` (0..2) | 2, **1** | refused, 134 |
| `concat-clean`, `number`, `empty`, `chars-a`, `join-sep-one` | equal | allowed |
| `concat-nul` | 4, **2** | refused |
| `fstring` `f"<{held}>"` | 5, **2** | refused |
| `join-part`, `join-sep-two` | 5, **3** and 5, **2** | refused |
| `repeat(held, 2)` | 6, **1** | refused |
| `chars-nul` (`held.chars()[1]`) | 1, **0** | refused |

And where a NUL is correct today, `whole.hero`: `print` of the text writes
`61 00 62 0a` and `write_file` writes `61 00 62`, identically on base, rA
and rB, because neither lends.

### Route (d) against the net's own tests (21:20 to 21:22)

`heroes test tests/harness/main.hero` in the base copy and in rD, side by
side: **base 260 tests, 1 failed; rD 260 tests, 2 failed**. The one both
share is `"a dead citation is caught, and the anchored rule ignores what it
must"` (`assert failed: !excused["site/dist/nosuch.html"].is_err()`), the
copy's environment. **The one rD adds is defect 317's test**, `"fixedbugs:
a child that writes bytes that are not UTF-8 ran, and its refusal says so"`,
`panic: hero_str_from_bytes: the bytes hold a NUL, which no str holds`: its
case `printf 'a.hero\000caf\351.hero\000b.hero'` is *a name among NULs, as
`git ls-files -z` lists them*.

That is not an accident of one test. **The net's harness reads NUL-separated
listings as a `str` by design**: `shell.project_files`
(`tests/harness/shell.hero:608`) runs `git ls-files ... -z`, captures the
child's stdout with `hero_file_read_shown` (`captured`, `:359`), and splits
it with `strings.split_on(text: ran.out, byte: 0)` (`:626`);
`suite_records.hero:820` walks the project through it; and the comment at
`shell.hero:30-32` states the premise, *a Heroes `str` carries that byte
through `read_file` and byte indexing without trouble*. **So a gated program
of this repository holds a `str` with NULs today, deliberately, and route
(d) breaks it.** `project_files` itself needs `.git` and could not run in a
copy (`.claude/rules/platforms.md` § Measuring in a copy); the shared
mechanism is what defect 317's test exercised.

### Route (c)'s price: the lends a gate builds, by the compiler's lexer (00:40)

`heroes lex <file> --dump-tokens` over every `.hero` file of the six trees
in my copy, counting the token run `.` `cstr` `(` `)` and `.` `lease` `(`
`)` (`192-ffi-pragmatist-work/lexcount.py`, output `lexcount.txt`):

| tree | files | `.cstr()` | in files | `.lease()` |
|---|---|---|---|---|
| `selfhost/` | 412 | 37 | 4 (`cli/files.hero` 18, `cli/process.hero` 15, `module/reading.hero` 3, one more) | 0 |
| `examples/` | 120 | 6 | 2 | 3 |
| `tests/golden/` | 1,235 | 68 | 50 | 18 |
| `tests/harness/` | 36 | 23 | 2 (`shell.hero` 22) | 0 |
| `docs/panel/` (no gate) | 210 | 26 | 16 | 28 |
| `archive/` (no gate) | 1 | 2 | 1 | 0 |

228 files the lexer refuses (exit 1, lexing diagnostics cases) were grepped
instead: two hold `.cstr()`, one in a gated tree
(`check/fixedbugs-172-a-result-the-header-gives-64-bits.hero`). So **a
fallible lend rewrites 137 lends** in gated code (134 by the lexer, 1 by
grep, and the prelude's 2 written as text at `library_source.hero:208`,
`:222`) **and 21 leases**, each gaining a `?` or a `.must()`, and every
function holding a `?` returning a `T?`. F7's 215 counted lines, comments
and docs included.

### The run-time prices, base against each route (00:42 to 00:45)

`192-ffi-pragmatist-work/price/`: seven probes built by each route's
compiler at `-O0` (`build`'s default) and `-O2` (`run`'s), each run three
times, **instructions retired** by `/usr/bin/time -l`, the median kept
(spread under 2 million everywhere but `read_big`, under 30 million there).
`lend_long`: one `str` of 1 MiB lent 1,000 times to a C function of my own
that reads one byte (`probe.h`); `lend_long_strlen`: the same lends to
`strlen`; `lend_short`: 1,000 short `str`s lent 1,000 times each;
`read_big`: `read_file` of a 64 MiB text file, four times; `concat` and
`slice`: a million each. Three runtime variants were added after the first
table, built with route (b)'s or (d)'s compiler over a runtime named by
`HEROES_RUNTIME` (`192-ffi-pragmatist-work/rt/`): **rB2**, route (b) with the
fact in the magic word's low bit (`HERO_STR_MAGIC | 1`, one constant and a
bit test) and the NUL found in `from_bytes` by one `memchr` after the
unchanged walk; **rD2a**, route (d) with the NUL found by `memchr` instead
of a compare folded into the walk; **rD2b**, rD2a with `hero_file_read`'s
second walk removed (the judged bytes copied, not judged again).

At `-O2`, millions of instructions, and the change against the base:

| probe | base | rA | rB | rB2 | rD | rD2a | rD2b |
|---|---|---|---|---|---|---|---|
| `startup` | 15.8 | +0.3% | 0.0% | +0.4% | +0.1% | +0.3% | 0.0% |
| `lend_long` | 42.5 | **+1,079.9%** | +0.1% | 0.0% | 0.0% | +0.2% | 0.0% |
| `lend_long_strlen` | 304.8 | **+150.6%** | +0.1% | 0.0% | 0.0% | 0.0% | +0.1% |
| `lend_short` | 92.1 | **+65.0%** | +21.5% | **+5.9%** | -0.2% | -0.2% | -0.2% |
| `read_big` | 3,447.2 | -0.1% | +31.1% | **+3.2%** | +62.1% | +6.6% | **-43.6%** |
| `concat` | 612.6 | 0.0% | +4.5% | **+1.1%** | -0.1% | -0.1% | -0.1% |
| `slice` | 590.2 | 0.0% | +4.2% | **+0.8%** | -0.1% | -0.1% | 0.0% |

At `-O0` the same order: rA +708.2% (`lend_long`), +140.4%, +23.4%; rB2
+8.7% (`lend_short`), +1.3% (`read_big`), +5.8% (`concat`), +2.9%
(`slice`); rD +77.9% (`read_big`), rD2a +2.8%, rD2b -47.4%.

What the numbers say, each a reading of the table:
- **route (a)'s scan is about 0.44 instructions per byte lent**: 459
  million for 1 GiB lent, at either level; against `strlen`, which reads
  the string itself, it more than doubles the call; on a million short
  lends it costs about 60 instructions each at `-O2`. design.md §4.20 calls
  the free lend *the single highest-return decision in the string design*,
  and this is that decision's price;
- **route (b)'s fact costs about 5 instructions a lend in its low-bit
  form**, flat in the string's length, which is §1.12's *one predictable
  branch per FFI argument, paid deliberately*; the second-value form I
  built first (rB) cost about 20, from a second 64-bit constant in every
  `incref` and `decref`;
- **the zero test folded into the scalar UTF-8 walk is the expensive way to
  find a NUL**, about 4 instructions a byte a walk (rD, rB); a separate
  `memchr` is about 0.42 (rB2, rD2a). The brief's *one compare inside an
  existing scan* is measured here at ten times the cost of a second,
  vectorised scan;
- **`hero_file_read` walks its bytes twice today** (`os.c:809` and inside
  `hero_str_from_bytes`, the critic's reading), and removing the second
  walk (rD2b) more than pays for any route's NUL test: -43.6% on
  `read_big`. That saving belongs to no route; it is open to the base.

### The route I build and recommend: rBE (00:46 to 00:58)

`<scratchpad>/192-ffi-pragmatist-rBE/`, a fresh `git archive 4c3524fb`:
- **(b), the fact, in its low-bit form** (rB2's runtime): `HERO_STR_MAGIC_NUL
  = HERO_STR_MAGIC | 1`, so `HeroStrHeader` keeps its two fields and its
  size; the lend `hero_str_lend` (emitted at `inst.hero:316`) and the lease
  `hero_str_held` refuse a block whose bit is set, naming the byte;
  `hero_str_cstr` and its four length-passing callers are untouched;
- **(e) for the two doors whose types promise a failure**: two functions
  added to `hero_os.h`, `hero_file_read_str(HeroStr path, int64_t *status)`
  and `hero_file_write_str(HeroStr path, HeroStr text)`, which ask the
  name's BYTES (`memchr`, beside a system call), not the fact, and answer a
  new status `HERO_OS_BAD_NAME` (5) before any `fopen`; the prelude's
  `read_file` and `write_file` call them with the `str` and turn the status
  into `read_failed` / `write_failed`, *could not read (write) a path
  holding a NUL byte, which names no file*. The old two doors stay, so
  nothing else that declares them moves;
- **the runtime's printer by length**: `failure.c`'s three `%s` lines
  (`:116`, `:135`, `:145`) become one `fwrite` of the joined pieces, each
  program-made text by its `len` (`%.*s` stops at a NUL too).

One thing the build found: a call to a function with two `str` parameters
must label them (`needs_label`), and the prelude is checked only when a
program is compiled, so my first rBE compiler built and then refused every
program with *internal error: a diagnostic landed inside the Heroes
library, at its line 187*. The repair is `hero_file_write_str(path: path,
text: text)`. **A route (e) door taking a name and a text costs its callers
a label each.** Built 00:49:17 to 00:50:39, sha1 `b2c65f60c5136164`; built
again by itself (`heroes2`, 00:51:11 to 00:52:48), which carries
`hero_str_lend` for its own 37 lends.

**F4 under rBE** (`192-ffi-pragmatist-work/f4-rBE/`):
- `nulc`, `rf`: stop at the lend, *byte 1 of 3*, exit 134 (a binding's lend);
- **`qi-read`: `3`, `true`, `<failed>`, exit 0. `qi-write`: `11`, `true`,
  exit 0, and no `out-a` is written.** Read as failures (my `-reads`
  variants): `read_failed: could not read a path holding a NUL byte, which
  names no file`, `write_failed: could not write a path ...`;
- an `assert` whose side holds `a<NUL>b` prints `left:  61 00 62` under
  rBE, `left:  a` on the base.

**Every door, with a name holding a NUL** (`192-ffi-pragmatist-work/doors/`,
`doors.hero` and `drive.py`): one program, the door named by its argument,
the name `"v-" + door + "-" + <nul.txt>` so C reads `v-<door>-a`, a victim
the driver made in a fresh folder, the folder's names, kinds, modes, flags,
ACL lines and digests recorded before and after. 29 probes: the 27 doors,
`hero_run_go`'s program parameter apart from its output path, and
`hero_run_arg`.

| door | base, 00:57 | rBE |
|---|---|---|
| `read_file` (prelude) | `ok: victim of read_file`: **another file's text** | `err: read_failed: ...`, exit 0, nothing touched |
| `write_file` (prelude) | `ok`, **`v-write_file-a` overwritten** | `err: write_failed: ...`, exit 0, nothing touched |
| `hero_file_read_shown` | reads the victim | stops at the lend, 134 |
| `hero_env_shown` | **reads `V_ENV_a`**, another variable | stops at the lend |
| `exists`, `is_directory`, `kind`, `links`, `writable`, `landing`, `mode`, `flagged`, `newer_than` | answer about the victim (`1`, `1`, `1`, `1`, `1`, `v-landing-a`, `420`, `0`, `0`) | stop at the lend |
| `mkdir_all` | **creates `v-mkdir_all-a/`** | stops at the lend |
| `remove` | **deletes `v-remove-a`** | stops at the lend |
| `rename` | **renames `v-rename-a`** | stops at the lend |
| `file_stage`, `fs_replace`, `file_unstage` | **creates, replaces, deletes** the victim | stop at the lend |
| `set_mode`, `set_flag`, `deny_delete` | **chmod 0600, sets a flag, sets an ACL** on the victim | stop at the lend |
| `link`, `symlink` | **create `v-link-a`, `v-symlink-a`** | stop at the lend |
| `dir_scan` | lists the victim directory (`1`, `inside`) | stops at the lend |
| `dir_remove_tree` | **deletes the victim directory and its file** | stops at the lend |
| `run_go`, the output path | **overwrites `v-run_go-a`** | stops at the lend |
| `run_go`, the program | **runs `./v-run_go_program-a`**, another program | stops at the lend |
| `run_arg` | panic, *an argument contains a NUL byte* | the same |

So on the base 28 of 29 act on what the program never named, and under
rBE none does: 2 answer a failure, 26 stop at the lend, 1 keeps its panic.

**The net's own tests under rBE's self-built compiler**: 260 tests, 1
failed, the same environmental test as the base; defect 317's test passes,
because a `str` may still hold a NUL and only the lend refuses one.

### rBE against the compiler's own tests, its fixpoint, and the corpus's bindings (00:55 to 01:07)

- **The compiler's own tests**, `heroes2 test selfhost/main.hero` in the
  rBE copy: **1,213 tests, all passed, exit 0**; the base copy's own
  compiler, the same command: 1,213, all passed.
- **The fixpoint, on the emitted C**: `heroes2` and `heroes3` (rBE's
  compiler built by `heroes2`) each `build selfhost/main.hero --emit-c`:
  35,213,202 bytes both, `cmp` silent (the base seed is 35,206,983). The
  binaries differ at byte 1,449 of the Mach-O header, which is why the
  emitted C is what is compared. It holds 37 `hero_str_lend(` calls, the
  compiler's own 37 lends as the lexer counted them.
- **The price on a whole program**: `heroes check selfhost/main.hero`,
  three runs each, instructions retired: the base compiler built by itself
  the same way (`heroes-self`) reads a median of **81,348.8 million**, rBE's
  `heroes2` **81,925.9 million, +0.71%**. (The seed-built base read
  81,367.9 million; the first comparison against it was not like for like
  and is not used.)
- **The corpus's bindings**: the 20 `examples/` folders holding an
  `extern`, built by the base compiler and by rBE's `heroes2`: 19 build at
  exit 0 under both (`raylib`, `sdl` and `curl` among them, clang holding
  every declaration against its real header) and `gallery` has no
  `main.hero`, so its two FFI programs, `08-ffi.hero` and `13-lease.hero`,
  were built instead, exit 0 under both. Run (not `raylib` and `sdl`, which
  open windows, nor `curl`, which goes to the network): **all 18 print the
  same bytes and exit 0 under both, and the 16 with a `main.expected` match
  it under rBE** (`tally` fed its `main.stdin`).

### The C a binding needs where a NUL is correct (00:59 to 01:00)

SQLite takes text with its length, so `a<NUL>b` is a correct value there.
`192-ffi-pragmatist-work/sqlnul/`, against the real `sqlite3.h` and
`-lsqlite3`, `select length(cast(? as blob))`:

- **the shipped binding's own way**, `examples/ledger/db/sqlite.hero:354`'s
  lease with `length: -1`: **on the base SQLite holds 1 byte of 3, exit 0,
  silently**; under rBE the lease stops, *`.lease()` copies for C a str
  holding a NUL byte (byte 1 of 3)*, exit 134. Defect 245's shape is in the
  shipped example's code path today;
- **what a binding writes under rBE to carry the NUL whole**: one `static
  inline` in a header of its own taking the `str`, which clang checks
  against `sqlite3_bind_text`'s real prototype:

  ```c
  static inline int bind_text_whole(sqlite3_stmt *statement, int column, HeroStr text) {
      return sqlite3_bind_text(statement, column, text.ptr, (int)text.len, SQLITE_TRANSIENT);
  }
  ```

  declared `function bind_text_whole(statement: CStmt, column: i32, text:
  str) -> i64`: **`str len 3, sqlite holds 3`, exit 0, on base and rBE
  alike**. That is the cost rBE puts on a binding that means to pass a NUL
  with its length: a four-line shim, the same shape as the runtime's own
  `hero_file_write(path, HeroStr text)`.

### Linux arm64, base and rBE (01:08:56 to 01:11:05, one container)

`docker ps -q` empty first; `docker run --rm --name ffi192-arm64` over
`heroes-linux-arm64` (clang 22.1.8, aarch64), my base copy, my rBE copy and
my work folder mounted read-only and copied inside, the base compiler built
from the seed there (sha1 `0214bbc1f7ff140d`) and rBE's compiler built by it
from rBE's `selfhost/` (exit 0, `fe354e7c3a1b9733`); script
`192-ffi-pragmatist-work/linux/leg.sh`, output `leg-out.txt`. My container
exited at 01:11:05; a second one, `cool_varahamihira`, was running at that
moment, not mine (lane b11-windows may use Docker), and I did not touch it.

The same as this Mac, case for case:
- **base**: `nulc` `1`, `rf` `3` `1`, `qi-read` reads `the file named a`,
  `qi-write` writes `out-a`; the 14 shapes truncate in C exactly as here;
  `print` and `write_file` carry `61 00 62`; the `assert` printer shows
  `left:  a`; SQLite through the shim holds 3, through the ledger's lease
  1; `args()` aborts and `args_checked()` answers `not_text` on `x<FF>y` and
  on the WTF-8 of `x<D800>y`; **27 of the 29 door probes act on or answer
  about the truncated name** (`deny_delete` answers 4, unsupported on
  Linux; `run_arg` panics);
- **rBE**: `nulc` and `rf` stop at the lend; `qi-read` `3`, `true`,
  `<failed>`; `qi-write` `11`, `true`, no `out-a`; the same 7 shapes refused
  and 7 allowed; the `assert` printer shows `a \0 b` whole; the SQLite shim
  holds 3 and the lease stops at *byte 1 of 3*; the arguments as on the
  base; **`read_file` and `write_file` answer their failures and the other
  26 doors stop at the lend, touching nothing**.

One flaw of mine: the F4 lines' exit column in `leg-out.txt` read the wrong
pipeline (`PIPESTATUS` after an `echo`) and is not used; the shapes' and
doors' exits are right.

### `HERO_RUNTIME_ABI` (01:12)

rBE adds three functions (`hero_str_lend`, `hero_file_read_str`,
`hero_file_write_str`), two constants (`HERO_STR_MAGIC_NUL`,
`HERO_OS_BAD_NAME`) and a second accepted value of the magic word;
`HeroStrHeader` and `HeroStr` keep their layout. Both mismatched pairs, run
(`192-ffi-pragmatist-work/abi/`, `rf.hero`):
- **rBE's compiler over the base runtime** (`HEROES_RUNTIME`): exit 2,
  `call to undeclared function 'hero_str_lend'`. Loud, as panel 089's
  *adding a function is self-guarding* says;
- **the base compiler over rBE's runtime**: builds, exit 0, and `rf`
  prints `3`, `1`: **the guard is absent in silence.** That is the old
  behaviour, not a new defect, but it is a pair that runs without the
  promise, the silence the stamp was written against
  (`selfhost/cli/toolchain.hero:76-78`, *a decoy runtime ... the stamp
  turns that silence into `use of undeclared identifier`*).

So my route **moves the stamp to 27** in the robust reading, at the cost
panel 089 named (the seed asserts the number it was built with, so a bump
lands in the order the project has used for each of its moves since 15),
and holds it at 26 in the conservative one, which leaves that one pair
silent.

## Q4, the answers the brief asks for (written 01:14)

**What each route costs, in one place** (every number above, by its
command):

| | lends a gate builds (137, and 21 leases) | the emitter | a binding's author | run time | F4's `qi-` programs |
|---|---|---|---|---|---|
| (a) scan at the lend | unchanged | `inst.hero:316` names `hero_str_lend` | nothing; a NUL passed with its length now aborts, so a 4-line `str` shim | 0.44 instructions a byte lent: +1,080% on a long lend, +65% on short ones | abort at the prelude's lend: below the floor |
| (b) the fact, low bit | unchanged | the same one line | the same | 5 instructions a lend; `concat` +1.1%, `slice` +0.8%, `from_bytes` +0.42 a byte; **the compiler checking itself +0.71%** | the same as (a), alone |
| (c) a fallible lend | **137 lends and 21 leases rewritten**, each enclosing function a `T?` | a new fallible lend, its check (a)'s or (b)'s | a `?` or `.must()` at every lend | (a)'s or (b)'s | a failure through the prelude's `?` |
| (d) refused at construction | unchanged | none | a shim returning text with a NUL aborts; NUL data must stay in a `u8[N]` field | `memchr` +0.42 a byte; the fold measured +4 | a true failure at the first `read_file` |
| (e) doors take the `str` | unchanged | none | unchanged; **245 stays open for every binding** | one `memchr` a door | a failure at the door |

And what (d) costs that no table holds: **the compiler aborts (134) on any
`.hero` file holding a NUL** until its own read decides what one is (Q1,
Q2), and **the net's own tests go red** at defect 317's NUL listing,
because the harness reads `git ls-files -z` as a `str` by design.

**What each needs at the seven call lines** (`grep -rn 'hero_str_cstr('
runtime/`, 9 lines: the declaration, the definition, 7 lines of 10 calls):
- the four that pass a length, `os.c:864`, `:913`, `replace.c:440`, `:507`:
  **nothing under any route**. They keep `hero_str_cstr`, which no route
  touches; a route that put its check inside it would make `write_file`
  abort on a correct program (the critic's reading, and `whole.hero`
  writes `61 00 62` under rA, rB and rBE because it does not);
- the three that print through `%s`, `failure.c:116`, `:135`, `:145`: under
  (a), (b), (c) and (e) a `str` may still hold a NUL, so **each must write
  by length** (`%.*s` stops at the NUL too); rBE does, one `fwrite` of the
  joined pieces, and the `assert` side prints `61 00 62` on this Mac and on
  Linux arm64 where the base prints `a`. Under (d) they are exact unedited,
  no `str` holding one.

**Which failure each door answers, and whether the spec says it** (`grep -c`
over `spec/heroes-spec.md`): under rBE `read_file` answers `read_failed` and
`write_file` `write_failed` (a new status, `HERO_OS_BAD_NAME`, behind
existing codes); **neither code is in the spec today** (0 each, as
`file_not_found` is 0); the spec's failure codes are `null_cstr` and
`not_text` (`:384-385`) and `missing_key`. The other 25 doors answer no
failure: the lend stops first, exit 134, naming the byte. `hero_run_arg`
answers its panic. `file_not_found` would also be true of a NUL path (no
system holds such a name) and is the conservative alternative; I chose
`read_failed` because the program's next move on *not found*, writing the
file, would fail too.

**`hero_run_arg`'s panic: precedent or not?** **Precedent for the doors
that are bindings, and not for the two whose types promise a failure.**
`hero_os.h` is a C header like any other, a program reaches its 25 other
doors only through an `extern` group of its own, and at a binding the
boundary's answer to a value C cannot be handed is an abort that names it,
as for a null `cstr` (`ops.hero:254-270`, §1.12). `read_file -> str?` and
`write_file -> ()?` promise `.err`, and an abort there is the shape
`os.c:803-808` records as defect 002: *a program handling both `.ok` and
`.err` reached neither arm*.

**Which check runs first, and in what code.** On the base, none. Under
(a) and (b) alone, **the lend's**, in the call `hero_str_lend(path)` the
emitter writes at the prelude's `path.cstr()` (`inst.hero:316`'s site,
`library_source.hero:208`, `:222`), so it aborts before `hero_file_read`
runs. Under rBE the prelude no longer lends the path: **the door's `memchr`
runs first**, in C, in `hero_file_read_str` and `hero_file_write_str`
(`os.c`), before any `fopen`, and it asks the bytes rather than the fact,
so a constructor whose fact were ever wrong would still not open another
file. For the other 25 doors, the lend's check runs first.

**A shape beside, with this repair's cause, filed apart rather than
widened here** (`.claude/rules/verification.md` § Bounded discovery):
`validated_bytes()` over a `[u8]` the program built stops at the first
zero, so `[97, 0, 98]` gives a `str` of length 1 at exit 0 (measured,
`sources.hero`); the spec gives that rule to *a field of bytes* (`:384`)
and says nothing of a `[u8]`, where the bytes after the zero are data.
It is the mirror of 245 on the way in.

## Verdicts

### Q4: `.cstr()` and the doors

- **verdict**: **approve rBE** (route (b) with the fact in the magic word's
  low bit, the lease checked as the lend is, route (e) for `read_file` and
  `write_file`, and the runtime's printer writing by length);
  **object** to (a) alone, to (c), and to (d) as briefed; (e) alone does not
  close 245. No veto: no route measured here moves a layout across the
  boundary.
- **section**: design.md §4.20 (*`.cstr()` is free with zero copies, the
  single highest-return decision in the string design*), §1.12 (*one
  predictable branch per FFI argument, paid deliberately*), §1.11 (*FFI
  ergonomics rank alongside comprehension*), §4.3's freeze (`:1016-1018`).
  design.md does not cover a NUL that arrives without an escape (a file, a
  binding's C); that silence is this sitting's.
- **experiment**: four routes built and run (rA, rB, rD, rBE, plus three
  runtime variants); F4, 14 shapes, 29 door probes, SQLite through the
  shipped lease and through a `str` shim compiled against the real
  `sqlite3.h`, 20 binding examples, the compiler's own tests (1,213), the
  net's own (260), the fixpoint on the emitted C, all on this Mac, and F4,
  shapes, doors, SQLite and arguments again on Linux arm64. clang accepted
  every binding under rBE; the only refusal was mine (`needs_label`).
- **argument**: (a) prices §4.20's free lend at 0.44 instructions a byte,
  +1,080% on a long lend. (c) rewrites 137 lends and 21 leases. (d) aborts
  the compiler on a NUL in a source and breaks the net's own NUL listing.
  rBE keeps every byte a program holds, refuses exactly the lends where C
  would read fewer bytes than the `str` holds (7 of 14 shapes, both
  platforms), costs 5 instructions a lend and +0.71% on the compiler, and
  answers a failure at the two doors that promise one. On the base, 28 of
  29 doors touched another file; under rBE, none.
- **prediction**: with rBE landed, every one of the 20 `examples/` binding
  folders builds and runs unchanged on all three platforms, the full net
  reads the base's counts, and on the Windows box the 29 door probes touch
  nothing; and the ledger's own `bind_text` (`sqlite.hero:354`) aborts on a
  NUL-holding `str` where today SQLite stores 1 byte of 3.
- **condition**: a gated program, or a corpus binding, that passes a
  NUL-holding `str` through `.cstr()` or `.lease()` with an explicit
  length and is correct today (rBE would abort it); or the fact measured
  above 2% on a real program; or Q1 and Q2 making the compiler refuse a NUL
  in a source **and** the harness reading its `-z` listings as bytes, which
  removes (d)'s two measured breakages and would make (d), a free lend
  with no fact to carry, the better route.

### Q5: `args()` on Windows

**Waiting for the box** (unreachable at 00:38, the coordinator's message;
not tried again by me). What is run: on this Mac and on Linux arm64, base
and rBE alike, `args()` aborts (134, *not well-formed UTF-8*) on `x<FF>y`
and on `78 ed a0 80 79`, the WTF-8 of `x<D800>y`; `args_checked()` answers
`not_text` for both and `ok` for `good`; an argument holding a NUL cannot
be handed to a program (`subprocess` refuses it, *embedded null byte*), so
no route of Q4 is reached through an argument.

What the lane does, read and not built (`.claude/worktrees/lane-b11-windows`,
`d5133e26`, `dc7eed87`): the UTF-8 manifest in the runtime's object, the
start refused where `GetACP()` is not 65001, wide doors behind
`hero_win_wide` and `hero_win_name_bytes` (`runtime/parts/codepage.c:133`,
`:168`, `:203`); **`args()` still reads the narrow `hero_argv`**
(`hero_args_at`, `hero_str_from_cstr`), so under the manifest a lone
surrogate still arrives as U+FFFD (F9, carried, unrun by me).

My provisional verdict, owed the box's runs: **route 4, the wide `argv`
converted by the lane's own `hero_win_name_bytes`**, so a lone surrogate
reaches `args()` as its WTF-8 bytes and the existing checks answer as they
already do here: `args()` aborts, `args_checked()` answers `not_text`, and
`:324-325` becomes true on Windows with no new sentence. The manifest
alone turns `x<D800>y` into `x<U+FFFD>y`, valid text naming another file,
Q-i's shape on the way in. **It composes with c-dirwide**: the lane owns
the conversion, rBE sits in front of every door (at the lend and at the
two `str` doors), and the lane's `hero_win_char_at` reads a name to its
NUL, which rBE guarantees never arrives; of my files the lane changes only
`hero_os.h` (8 lines) and `os.c` (57), both by additions. Owed on the box:
the four conversions over `x<D800>y`, base and route, and rBE's F4,
shapes and doors.

**Corrected 01:15 (from `date`), two sentences above, left as written:**
- *"On the base, 28 of 29 doors touched another file"* (the Q4 argument):
  of the 28, 15 changed or made another file, directory, link, mode, flag
  or ACL, ran another program or read another variable's or file's
  contents, and 13 answered a question about another file (`exists`,
  `kind`, `mode` and the like). Both are another file; *touched* is true of
  the first 15 only.
- *"the compiler aborts (134) on any `.hero` file holding a NUL"* (the cost
  table): measured on two, a NUL in a string literal and a NUL in a
  comment; *any* is an inference from the mechanism (the shown read builds
  every source through `hero_str_from_bytes`).

## Status at 01:15

- **Q4: done** on this Mac and Linux arm64; its Windows leg (rBE's F4,
  shapes and doors) is owed to the box.
- **Q5: waiting for the box**, which was unreachable at 00:38. Everything
  Q5 can do off the box is above. I stop here, as asked, until the
  coordinator says the box is free.
- **Cost of this seat so far**: no paid run; Docker one container,
  `ffi192-arm64`, 01:08:56 to 01:11:05, removed on exit (`--rm`); at most
  three processes at once; nothing deleted. Copies:
  `<scratchpad>/192-ffi-pragmatist{,-rA,-rB,-rD,-rBE}/`, work in
  `<scratchpad>/192-ffi-pragmatist-work/`.

**Corrected 01:16, the correction of 01:15**: its split *15 and 13* is
wrong, counted again from the base's door table on this Mac. **19** changed,
made, read the contents of, listed or ran what the program never named
(`read_file`, `write_file`, `hero_file_read_shown`, `hero_env_shown`,
`mkdir_all`, `remove`, `rename`, `file_stage`, `fs_replace`,
`file_unstage`, `set_mode`, `link`, `symlink`, `deny_delete`, `set_flag`,
`dir_scan`, `dir_remove_tree`, `run_go`'s output path, `run_go`'s program),
and **9** answered a question about it (`exists`, `is_directory`,
`newer_than`, `landing`, `kind`, `links`, `writable`, `mode`, `flagged`):
28 in all, as the table says.

**Corrected 01:15:02, two times above**: the notes headed *Corrected 01:15*
and *Corrected 01:16* were written before the clock was read; `date`, run
in the same commands, read **01:14:43** and **01:14:54**. Likewise
*Status at 01:15* was written at 01:14:43.
