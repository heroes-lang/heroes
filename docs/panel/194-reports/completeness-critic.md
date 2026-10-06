# Panel 194, completeness critic, second pass (after the seats)

Started 2026-10-06 18:08:41 (`date`). No verdict. Copy:
`<scratchpad>/194-critic2/`, made 18:08:50 by `git archive 7a26a0a6 | tar -x`
(from the lane-panel-194 worktree, read-only), committed seed SHA-256
`fc9751a29a1ecb8a`, equal to the shared brief's. The first pass's copy
(`194-critic/`, `44f3b802`) is kept as it was.

(written as each command runs)

## 1. The nine blind programs, compiled and run (18:09:59 to 18:11:49)

**Compilers.** L and C: the frozen compiler, `194-critic2/heroes`, built
18:08:56 to 18:09:01 from the committed seed, exit 0. M: the
compiler-engineer's R1 as finally built, `ck/selfhost` copied to
`194-critic2/r1/selfhost` (`diff -rq` equal, 13 files differ from the frozen
`selfhost/`, his *+304/-79 in 13 files*), built by the frozen compiler,
`heroes build selfhost/main.hero -o heroes-r1`, exit 0 (18:09:15 to 18:10:49).
Each program is the first fenced block under `# program` in
`/tmp/b194-<t>-<v>/report.md` (equal to the copies in `p194/reports/blind/`),
extracted to `194-critic2/judge/<t>-<v>/prog.hero`.

**What the folders held** (`diff` against the frozen `spec/heroes-spec.md`):
L the frozen spec; M line 370 with 178's sentence **verbatim, with no union
clause** (the ergonomist brief says *R1's sentence with the union clause the
spec-warden prices*: the folders do not); C line 370 with *C's `char` is
`i8`.*; the three tasks' `spec.md` equal per variant, `brief.md` and
`headers.txt` equal across variants (`cmp`). Each session: `claude-opus-5-5`
(with a haiku helper), 5 turns, 0 permission denials, 0.22 to 0.41 USD,
**2.99 USD for the nine** (`run.json`), under the 18 USD bound.

**t2's shim cannot link as the readers had it.** `shim.h` only *declares*
`connect_un`. With it, the t2-L program (once `lent` is removed) stops at
`ffi_missing_link`: *the linker cannot find `connect_un`* (exit 1,
`judge/t2-L/declonly/`). So I judged t2 with
`judge/shim-def.h`, the same declaration as a `static inline` that calls
`connect`, and a listener on `/tmp/demo.sock` (`judge/listener.py`) so that
`connect_un` returning 0 proves the path's bytes arrived.

| program | compiler | `check` | `run` | what it prints / says | class |
|---|---|---|---|---|---|
| t1-L | frozen | 1 | 1 | `lent_shape` (`lent` on `@buf`), `fixed_outside_a_group` (`zero: i8[256] = [...]`, a local) | refused; see below |
| t1-C | frozen | 1 | 1 | `fixed_outside_a_group` (`constant ZEROS: i8[256]`) | refused; see below |
| t1-M | R1 | 0 | 0 | `Darwin`, `arm64` | **correct first try** |
| t2-L | frozen | 1 | 1 | `lent_shape` on `@addr: SockaddrUn lent`; with `lent` removed: `check` 0, `run` 0, `socket returned 3`, `connect_un returned 0` | refused, message names the repair |
| t2-C | frozen | 1 | 1 | the same `lent_shape`; with `lent` removed: `socket: 3`, `connect_un: 0` | refused, message names the repair |
| t2-M | R1 | 1 | 1 | the same `lent_shape`; with `lent` removed: `socket: 3`, `connect_un: 0` | refused, message names the repair |
| t3-L | frozen | 0 | 0 | `init: 0`, three rounds `lock 0 ... unlock 0`, `counter: 3` | **correct first try** |
| t3-C | frozen | 0 | 0 | the bytes before and after init, `init 0`, three rounds 0, `counter: 3` | **correct first try** |
| t3-M | R1 | 0 | 0 | `init 0`, the bytes, three rounds 0 with the counter 1, 2, 3 | **correct first try** |

Under the frozen compiler t1-M and t3-M stop at `unknown_name: zero` (exit 1),
as expected of a form that does not exist there.

**t1-L and t1-C's message points at a dead end.** `fixed_outside_a_group`'s
note says *use `[T]` here*. Followed literally (`zero: [i8] = [...]`,
`constant ZEROS: [i8]`, `judge/t1-*/fix1/`): `check` 1, five `type_mismatch`,
*expected `i8[256]`, found `[i8]`*. The working repair, the literal written
inside each field (pass 1: `194-critic/w/blind1/fix_i8.hero`, run 0, `Darwin`),
is named nowhere. So these two are **refused, with a message that does not name
the repair**: two turns at least.

| variant | correct first try | refused, message names the repair | refused, message misleads | silently wrong | `u8` for C's `char` |
|---|---|---|---|---|---|
| **L** (no sentence) | 1 (t3) | 1 (t2) | 1 (t1) | **0** | **0** of 3 |
| **C** (`char` is `i8`) | 1 (t3) | 1 (t2) | 1 (t1) | **0** | **0** of 3 |
| **M** (178's sentence) | 2 (t1, t3) | 1 (t2) | 0 | **0** | **0** of 3 |

`grep -n u8 judge/t*/prog.hero`: `u8` appears only for `sun_len` and
`sun_family`, which are `unsigned char` and `sa_family_t` (unsigned 8-bit):
right. Every `char` field in the nine is `i8`.

**What the nine readers found that no route addresses.**
- **4 of 9 wrote `lent` on an `@` record parameter** (t1-L, t2-L, t2-C,
  t2-M), refused `lent_shape`. The spec's grammar admits it
  (`CParam = [ "@" ] ident ":" Type [ "counted_by" ident ] [ "lent" ]`, line
  443) and its prose (*a parameter is taken to keep what it is handed unless
  declared `lent`*, line 396) invites it; no sentence says an `@` parameter
  takes no `lent`. It cost the only turn every t2 reader lost, under all three
  variants.
- **2 of 3 t1 readers without `rest: zero` bound a fixed array outside a
  group** to avoid 1,280 zeros (a local, a constant): 178's *11 of 50* class,
  alive, and its note now sends the reader to a type the field refuses.

**How far nine readers carry anything.** One reader per cell, three per
variant. What they can carry: **existence** (a reader under M wrote
`Utsname(rest: zero)` and `Mutex(__sig: 0, rest: zero)` unprompted, and both
ran; a reader under L and C reached the fixed-array-outside-a-group wall that
M removes; no silently wrong program under any variant). What they cannot
carry: **a rate**. 178's `u8` finding was 22 of 50 overall (1 of 10 under its
I, 6 of 10 under each R1 variant); here 0 of 9, and **0 of 3 under L and 0 of
3 under C**. With 3 per variant, 0 of 3 is compatible with any true rate up
to 63% (one-sided 95% bound, 1 − 0.05^(1/3) = 0.632), and 178's 44% (22 of 50)
sits inside it: the C sentence's effect on `u8` is **not measurable here**,
and 178's figure is neither confirmed nor refuted. Two readers wrote that
they took *plain `char` is signed on Darwin* from their own knowledge (t1-L
choice point 1, t1-M `context`); whether 178's readers ran on the same model
is a question (178's critic wrote only *the same model family as this
critic*), so a change of model is one unrun explanation of 22 of 50 against 0
of 9.

**Against the CARRIED rate the spec-warden leans on** (its § *§1.2, from
178's reader test*, and its row *R1 without K: object*, 18 of 30 against 1 of
10). Two-sided Fisher exact, computed here (`python3`, hypergeometric): today
**0 of 9 against 178's 22 of 50, p = 0.020**; today's M **0 of 3 against 178's
R1 variants 18 of 30, p = 0.083**; L 0 of 3 against I 1 of 10, p = 1.0. So the
`u8` effect that makes K a condition of R1 **did not reproduce** in today's
nine, at a level that is significant overall and not within one variant. The
populations differ in ways nobody controlled: 178's readers wrote three tasks
in one session (a reader counted once), these wrote one; the headers here
spell `char` beside every field; the model is today's. **What would settle it**
is the reader test the spec-warden itself registers (N >= 10 per arm), run with
**R1 + K together and R1 alone** as arms: no session of this sitting saw R1 and
K together, and no session saw R1's honest wording (below, § 4).

## 2. The seats' disagreements, reproduced where a command reaches them (18:12 to 18:20)

### 2a. The spec-warden: 178's sentence is false under panel 186's union rule. **Reproduced through the R1 compiler.**

`194-critic2/disc/union/small.hero` (the compiler-engineer's `un.h`: `SA` a
struct with an anonymous `union { char c; double d; }`, `UD` a union type),
`SA(kind: 1, c: 7, rest: zero)` and `UD(c: 7, rest: zero)`, run by
`r1/heroes-r1`: **run 0, prints `7 3.5e-323 0 7 3.5e-323` and `false false`**
for `d == 0.0`. `d` is a field the construction does not name, and it is not
zero: 178's *every field it does not name is zero* is false on the first
program that names a union's small member, and § 13 lets a program read any
member (*names one or more of each union, reads any*). The seat showed it in C
(`w/un/u.c`); this is the pipeline. The M readers of this sitting were shown
that sentence verbatim (§ 1).

The compiler-engineer's union table, re-run with the same compiler on his
programs (copied to `disc/union/`): `none.hero` run 0 `1 0.0 0 0.0`;
`one.hero` run 0 `1 2.5 0 1.5`; `two.hero` run 1 `ffi_union_field`;
`norest.hero` run 1 `missing_fields` for `x`. **All four equal his.**

### 2d. The compiler-engineer's interim U. **Reproduced, with one consequence no seat drew.**

`u/selfhost` copied from his tree (5 files differ from the frozen), built by
the frozen compiler, `u/heroes-u` (18:14:05 to 18:15:38, exit 0). In
`disc/u/`:

| program | frozen | U |
|---|---|---|
| `wide.hero`, `fill_ints(int *)`, `n: 16` over a `u8[16]` field | run 0, `4702111234474983745` (`after` overwritten) | **run 1**, `ffi_parameter_type`, *`int *` points at `int`, which C counts in units wider than a byte* |
| `wide4.hero`, the same with `n: 4`, a **correct** call (16 bytes) | **run 0, prints `7`** | **run 1, the same refusal**: the price he names |
| `narrow.hero`, `char *` and `void *` | run 0, `7 65` | run 0, `7 65` |
| the ffi-pragmatist's `wide-units.hero`, `mbstowcs(wchar_t *)` | run 134 after `16` and `438086664293` | **run 1**, the same refusal: U closes the ffi-pragmatist's case too |
| the 17 tracked `.hero` declaring `counted_by` (`grep -rl counted_by tests/golden examples`, the same list as his) | 3 exit 0, 14 exit 1 | **17 of 17 the same exit and stderr** (`cmp`, the `tu-` hash masked) |

**Every claim of his row reproduces.** Two things beside it:

- **U's note is false for the case it fires on**: it tells the author *declare
  `dst: ptr`*, which the declaration already is (`disc/u/mb/u.out`). He said so
  (*a landing writes its own*); landed as built it is a false message, which
  `.claude/rules/verification.md` § Bounded discovery classes `blocking`.
- **U is route C′ narrowed, and two seats object to C′.** The lane defines C′
  as *refusing `counted_by` on a lend whose header pointee is not `void`*
  (`docs/panel/194-evidence/092-routes.md`, § What neither route closes, 1); U
  refuses where the pointee is wider than a byte (`void` and one-byte types
  pass). The ffi-pragmatist **objects** to C′ (*`poll` on more than one
  descriptor has no binding under C′*) and the historian **objects** to C′
  (*every precedent counts typed pointers in elements*). Neither read U; the
  compiler-engineer did not read their C′ rows. `wide4.hero` is the measured
  form of their objection: a correct program refused. **Not a contradiction in
  fact, a contradiction in verdict**: U as an interim until C lands is one
  ruling, U as the rule is the C′ two seats refuse; the synthesis has to say
  which, and with what end date.

### 2b. The compiler-engineer: veto on R1 built `check`-side, approval of the `build`-side one. **Reproduced.**

`cs/selfhost` copied from his tree (20 files differ from the frozen), built by
the frozen compiler, `cs/heroes-cs` (18:15:38 to 18:17:12, exit 0). Same
programs, two compilers (`disc/union/`):

| program | `build`-side (`r1/heroes-r1`) | `check`-side (`cs/heroes-cs`) |
|---|---|---|
| `none.hero`, a union none of whose members is named | run 0, `1 0.0 0 0.0` | **run 1, `ffi_union_field`, *names `c` and `d`***: two fields the program never wrote |
| `one.hero`, `d` named | run 0, `1 2.5 0 1.5` | **run 1**, the same |
| `small.hero` (mine), `c` named | run 0, `7 3.5e-323 ...` | **run 1**, the same |
| `two.hero`, `c` and `d` named | run 1, `ffi_union_field` | run 1, `ffi_union_field` |
| `norest.hero`, no words | run 1, `missing_fields` for `x` | run 1, the same |
| `kinds.hero`, `uname_rest.hero` (his) | run 0, equal outputs | run 0, equal outputs |

Layout, `code_lines` replicated in awk on `suite_layout.hero`'s rule:
`ir/flatten.hero` frozen **1150**, build-side **1150**, check-side **1158**
against DECIDED 1150 (`suite_layout.hero:450`); `ast.hero` 550 / **517** /
513 (DECIDED 550, the seam move in both); `grammar_expr.hero` 1073 / 1082 /
1082 (1085); `check/walk.hero` and `print/fmt.hero` untouched in both. **His
numbers reproduce, his corrected 517 included.** The veto's ground is
measured: the check-side route gives a false message (*names `c` and `d`*)
on correct programs, which § Bounded discovery classes `blocking` if it
landed.

### 2c. The ffi-pragmatist: veto on 092's route B, approval of route C.

(2c is written below § 3, once the 092 prototype compiler is built.)

## 3. The four new defects, reproduced on the frozen tree (18:14:23 to 18:16:00)

Each run by the frozen compiler (`194-critic2/heroes`, `7a26a0a6`'s seed), the
probes copied from the seats' copies into `194-critic2/def/`. Each `grep`ped
for in this copy's `issues/` with the words below; **none found** (a search,
not proof). Classes read from `.claude/rules/verification.md` § Bounded
discovery: `blocking` is *a wrong value, a crash, a memory fault, an exit 2
where the author can be told, a false message, ..., a correct program refused,
a wrong one accepted*, and it is never deferred.

| # | defect (finder) | reproducer, frozen compiler | result | class, and the words of the rule it meets |
|---|---|---|---|---|
| 1 | **A scalar lent through `@` to a pointer C writes an array through** (ffi-pragmatist) | `def/sha/md_scalar.hero`: `function SHA256_Final(@md: u8, @c: Sha256Ctx)`, `m: u8 @ 0` | `check` 0, `run` 0, prints `1 1 1 186` (the digest's first byte, right): silent | **`blocking`**, *a memory fault*: see the two runs below |
| | | `def/sha/md_guard.c` (his C proof, `clang -lcrypto`, Darwin) | `m=186 guard bytes changed past the one lent: 31 of 63` | |
| | | **`def/sha/md_field.hero` (mine)**: the same lend from a field, `@b.m` of `record Box tag box { m: u8, rest: u8[7], after: i64 }` with `after: 7` | **`check` 0, `run` 0, prints `after` = `2531777658719584577`**, which is bytes 8 to 15 of SHA-256("abc") read as a little-endian `i64` (`python3 hashlib`: equal) | the corruption made visible in the program's own record at exit 0; the seat's canary had landed in unprinted slots |
| 2 | **§ 13's field lend counts bytes, C counts its pointee's unit** (both the ffi-pragmatist and the compiler-engineer, two reproducers of one cause) | `def/wide/wide-units.hero`: `mbstowcs(dst: ptr counted_by n lent, ...)` over a `u8[16]` field, `n: 16` | `check` 0, `run` **134** after `16` and `438086664293` (`after` overwritten) | **`blocking`**, *a memory fault* at `check` 0 |
| | | `def/unit/wide.hero`: `fill_ints(int *)`, `n: 16` | `check` 0, **`run` 0**, prints `4702111234474983745` (`0x4141414141414141`) | the same, silent at exit 0 |
| 3 | **A handle built with no arguments** (compiler-engineer) | `def/handle/handle_empty.hero`: `record Opaque tag opaque` (an incomplete C struct), `h = Opaque()` | `check` 0, **`run` 134**, *panic: entered unreachable code — this is a compiler bug* | **`blocking`**, a crash at `check` 0 that the compiler calls its own bug. § 13 (`sed -n 376,384p`) defines a handle as *C's pointer to that type* with `nullptr` its null, and says nothing of `T()` for one: the seat's *a handle is never built (spec § 13)* is an inference from that silence, not a sentence. Whichever way it is ruled (refused at `check`, or `nullptr`), today's exit is wrong |
| 4 | **A named function with an `@` parameter taken as a value** (compiler-engineer) | `def/fnref/inout_value.hero` (`_ = bump`), `inout_called.hero` (`f = bump`, `f(@x)`) | `check` 0, **`run` 2**, *internal error: compiling the generated C failed*, *incompatible function pointer types ... `void (*)(long long)` from `void (int64_t *)`* | **`blocking`**, *an exit 2 where the author can be told*: § 3's function type `(function(A) -> B)` has no `@` (`spec/heroes-spec.md:73`), so the value has no type to be told in, and `check` should refuse it |

Defect 1 is **092's gap 2 at a scalar** (*a typed pointer, no count*, the
lane's § What neither route closes, 2): its cause is not a record, and none
of routes A to E reaches it. Whether it is filed as its own number or as a
shape of 092 is the coordinator's; by § Bounded discovery's own bound (*only
a shape with the repair's own cause stays in the item*) it has 092's cause
only if 092's repair would reach it, and no proposed one does, so it reads as
its own item. Defect 2 is the same unit gap 092's routes A and B carry
(`a-typed-pointer-record-count`); route C or the interim U closes it (§ 2d).

### 2c (continued). The ffi-pragmatist: veto on route B, approval of route C. **The veto reproduced; C's approval rests on C written by hand, reproduced as such.**

The lane's `docs/panel/194-evidence/092/prototype.diff` applied to a fresh
copy of the frozen `selfhost/` (`patch -p1`, 8 files, exit 0), built by the
frozen compiler, `proto/heroes-proto` (18:17:12 to 18:18:45, exit 0). His
programs copied to `disc/c092/`, the lane's to `disc/c092/ev/` (4,096 bytes
of `A` on standard input):

| program | frozen | prototype (A and B) |
|---|---|---|
| `b-wrong-type.hero`: a 48-byte `addrinfo` through `h.ptr()` to `struct pfd *`, `nfds: 6` | 1, `bad_operand` | **run 0, prints `6 65536`**: C wrote `revents` into the record |
| `a-wrong-type.hero`: the same through `@` | 1, `counted_by_shape` | 1, `ffi_parameter_type`: the pointee check holds |
| `gso-today.hero`: real `getsockopt(SO_LINGER)` into `struct linger` | **run 0, `0 0 8`, correct** | **1, `ffi_parameter_type`**: a correct binding refused |
| `gso-a.hero`: the same with `counted_by len` | 1, `counted_by_shape` | 1, `counted_by_shape` (*`len` is not an integer C takes by value*): no checked spelling |
| `today-void-count-fits` (correct) | run 0, `4 1094795585` | **1**, refused |
| `a-void-count-fits`, `b-void-count-fits` | 1 | run 0 |
| `a-typed-pointer-record-count`, `b-typed-pointer-record-count` (`nfds: 2` against an 8-byte record) | 1 | **run 0, `2 1`, silent**, both routes |
| `today-read-into-addrinfo` | **138** after `4096 1094795585` | 1, refused |

**The veto's ground reproduces**: B lets a wrong record type through `ptr()`
silently, where `@` refuses it. So do his two objections to A as built
(`gso-today` refused; no spelling for a count through a cell) and the unit
hole both routes leave (`*-typed-pointer-record-count`).

**Route C is unbuilt in Heroes.** What reproduces is his C: `routes_c.c`, the
guard each route would emit written by hand, compiled under the emitter's
flags at `-O0` on Darwin, Linux arm64 and Linux x86-64 (`heroes-linux-arm64`,
`heroes-linux` without `--platform`): 0 diagnostics on each, **output
byte-identical on the three legs** (`cmp`), and C refuses before C runs in all
three shapes (`read` 4096, `poll` 2, `mbstowcs` 16) where A/B admit two of them
(2 and 48 bytes past). That is the arithmetic of the guard, not the
mechanism: the lane's own sentence on C is *the pointee check ... hands the
round a `-D` per counted record lend naming the unit*, and no seat built it.
The historian also approves route C, and **approves B as the remedy** (its
table: *Zig `asBytes`; Swift `withUnsafeMutableBytes(of:)`*), which
`b-wrong-type` measures against. Whether those precedents let a byte view
reach a parameter typed `struct pfd *` without a cast the author writes is a
question I did not run; here `ptr` reaches it with nothing written. **Approve
B (historian, advisory) against veto B (ffi-pragmatist): the veto is the
measured one.**

## 4. What the sitting missed, and what settles it (18:19 to 18:24)

### 4a. The census's Linux columns, recounted: **33 and 27 public, not 29 and 23.** (A route nobody built: the spec-warden found the fault and left the recount.)

`disc/census/mktu.sh` (178's 35 headers, each kept if it compiles alone, then
`clang -Xclang -ast-dump`) and `disc/census/count.py`, which names a record by
its own name, else **the typedef whose `Record 0x...` line points at it**, else
its enclosing record's name, and attributes each array field to the innermost
enclosing `RecordDecl` by depth (178's awk gave an anonymous record's fields
to the previous named one). Run on Darwin (`-I/opt/homebrew/include`) and in
both images (no header missing on Linux):

| leg | records with an array longer than 8 | public (named, no leading `_`) | the shared brief and ROADMAP row 63 |
|---|---|---|---|
| Darwin arm64 | 62 | **42** | 62 / 42: equal |
| Linux arm64 | **39** | **33** | 34 / 29 |
| Linux x86-64 | **34** | **27** | 29 / 23 |

On Linux arm64 the recount drops four names the old rule invented (`cmsghdr`,
`sched_param`, `sigval`, `timespec`, each a neighbour credited with an
anonymous record's array) and adds eight it hid: `cpu_set_t`, `fd_set`,
`mcontext_t`, `pthread_mutex_t`, `pthread_cond_t`, `pthread_rwlock_t`,
`pthread_barrier_t`, `siginfo_t` (29 − 4 + 8 = 33). The spec-warden's
*likely an undercount* is right in sign, by 4 on each Linux leg. It moves no
verdict (the thesis argument's rows were named records), but the number the
ROADMAP carries for the milestone is wrong, and **glibc's mutex, cond, rwlock
and barrier are census rows on Linux**, which the zero-validity table (§ 2 of
the ffi-pragmatist) is about.

### 4b. No reader saw the text the seats approve

The spec-warden approves R1 in its honest wording (beta +55, or gamma +42,
both with a union clause) **only composed with K**; the ffi-pragmatist's
condition (b) and the compiler-engineer's amendments say the same union rule.
The nine sessions saw L, 178's sentence verbatim (shown false in § 2a), and K
alone. **No session saw R1 + K, and none saw a union clause**; none of the
three tasks holds a union, so the clause's cost to a reader is unmeasured
(Darwin's `struct sigaction` holds `union __sigaction_u`, `sys/signal.h:270-288`
in the SDK: a task that could carry it; not run). The spec-warden's own registered test
(N >= 10 per arm) is the instrument; its arms should be **L, R1-honest, R1-honest
+ K**, with one task holding a union.

### 4c. Two findings the readers made that no route addresses

- **`lent` on an `@` record parameter: 4 of 9 readers, every t2 reader under
  every variant** (§ 1). Spec line 396 (*a parameter is taken to keep what
  it is handed unless declared `lent`*) and the grammar at line 443 admit it;
  no sentence excludes it. Today's `lent_shape` carries no fix (no `fix` line
  in `judge/t2-L/check.txt`). Routes, unpriced and unbuilt: a `certain` fix on
  `lent_shape` deleting the word (0 spec tokens, the word reads nothing), or a
  clause in the `lent` sentence.
- **`fixed_outside_a_group`'s note sends the reader to `[T]`, which the field
  then refuses** (§ 1, `judge/t1-*/fix1/`: `type_mismatch` five times). By §
  Bounded discovery's own words that is *a mistake told only after the first
  is fixed*: **`adjacent`**, unfiled as far as a search reaches (`grep -rli
  'fixed_outside_a_group' issues`: three files, defect 064's, 097's and one of
  2026-08-17; none names the note's `[T]` advice). Under R1 the note could name `rest: zero`
  where the binding feeds a group record's field; without R1, *write the
  literal in the construction*.
- Two of three t3 readers **copied the mutex by value** to print its bytes
  (`show(m)`, `dump(..., m)`, a by-value parameter): 178's open question 2
  (*`m2 = m` copies a mutex, which POSIX does not allow, and nothing refuses
  it*), carried as M-buildable-structs item `2026-09-25-0003`, met by readers
  unprompted. Reading a copy's bytes is harmless here; locking one would not be.

### 4d. The reader test could not reach the conservative route

The ffi-pragmatist measured *nothing* as viable: a program's own header with
`#define UTSNAME_ZERO {0}`, bound through 094's lowering (189 tokens against
R1's 158). The readers could write no file but `report.md`, and `headers.txt`
was the machine's, so **no reader could have taken that route**: the sitting
measured R1 against 1,280 literal zeros, not against the route its
conservative alternative is. An arm in which a reader may write a header is
what would measure it.

### 4e. Three seat claims that are a question, not a measurement

- **Whole-union zero is clang's, under every spelling.** Under the emitter's
  flags `(SA){}` compiles silently, under `-Wpedantic` it is *a C23 extension*
  (`disc/brace/b.c`); the ffi-pragmatist's condition says never `{}`, the
  historian says C23's `{}` is the one spelling C promises whole-union zero
  for and GCC 15 dropped it for `{0}` (verified by the historian, its URL). So
  any § 13 sentence saying a union none of whose members is named is zero
  promises the emitter's C compiler's behaviour; the compiler-engineer and the
  ffi-pragmatist measured clang on three legs, **Windows unrun**.
- **Defect 1's question has no route**: what a binding says when C writes N
  through a typed pointer with no count (`SHA256_Final`'s 32, `MD5_Final`'s
  16, `MD5_DIGEST_LENGTH` at `/opt/homebrew/include/openssl/md5.h:28`). The ffi-pragmatist names no header signal for *how many*; where the
  header writes an array type (`uuid_t`, `unsigned char[16]`) clang knows N,
  where it writes `unsigned char *` it does not. Unrun.
- **t2's shim had no definition** (§ 1): the landing's reader test needs one
  (`static inline`) or every t2 program stops at `ffi_missing_link`.

## 5. Unrun by this pass

- Linux and Windows for any of the nine programs, for R1, for U and for the
  092 prototype (Darwin only here; the C of route C ran on three legs).
- The four defects on Linux and Windows (the ffi-pragmatist ran 1 and 2 on
  three legs; I re-ran them on Darwin only).
- `--sanitize` on `md_field.hero`: the overwrite is shown by the printed value,
  not by a sanitizer (the store is inside libcrypto, which ASan does not
  instrument, as the seat found).
- The historian's URLs, not re-read; its GCC 15 quote is the only claim I
  lean on (§ 4e), and it is marked as the historian's.
- The spec-warden's eight `--refresh` prices: not re-run (no paid run of my
  own); its base control, 9518 equal to the pin, is its own.
- The compiler-engineer's instruction counts: not re-run (they are his, and
  I ran several builds at once on a shared machine).

Finished 18:22 (`date` below).
