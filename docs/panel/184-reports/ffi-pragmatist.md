# Panel 184, ffi-pragmatist report

Written 2026-09-30 from 17:02 by the ffi-pragmatist seat, as it went. Every
number below names the command that produced it, run in this seat's own
directory,
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/184-ffi-pragmatist/`
(below: `<seat>`), a `git archive a294a6ff` extraction with `build/` removed and
the compiler built from the seed, `clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes` (`/usr/bin/time -p`: real 10.26, user 7.15, at a
load average of 27). The clang is `Apple clang version 21.0.0
(clang-2100.3.34.2)`, target `arm64-apple-darwin25.6.0` (`clang --version`).

**Interrupted once.** The session stopped at about 17:10 on an API session
limit (HTTP 429), not by a person, and resumed at 17:22 when the author asked.
The measurements of 17:02 to 17:10 below were run before the stop and written
here at 17:23 from that session's outputs; the seat directory and its files
survived intact (`ls -la <seat>/work`, 17:23). **Interrupted a second time** at about
18:30 by the stream watchdog (no progress for 600 s, the machine at load 150
from three lanes' gates, the coordinator's account), resumed at 19:54: a
background loop emitting Q3's C had written nine of twelve files and was
killed on the tenth, `else-if-chain-2000.c`, left empty (`stat`, 19:55); the
nine were read and the missing three re-run after the resume.

## Setup

- [x] seat directory extracted from `a294a6ff`, compiler built (17:02)
- [x] Question 1 at the C boundary
- [x] Question 2 at the C boundary
- [x] Question 3 at the C boundary
- [x] verdicts (20:31)

## Question 1 at the C boundary

### Q1.1 C interfaces whose strings legitimately hold `{name}`-shaped text

Each verified on this Mac from the library's own header, man page, or by
running it; none from memory. "Integer hole" means the braces hold a text that
parses as an integer literal, "name hole" one that parses as a bare name.

| interface | the text | shape | verified by |
|---|---|---|---|
| POSIX regex, `<regex.h>` (libc on Darwin and Linux) | `[0-9]{3}` | integer hole | `man re_format` lines 28 to 37: *A bound is '{' followed by an unsigned decimal integer ... always followed by '}'*; `regex.h` in the SDK |
| PCRE2 10.48 (design.md §1.11's own table: *Regular expressions, PCRE2*) | `\\d{3}`, `\\p{L}`, `\\p{Nd}`, `\\k{name}`, `\\g{2}` | integer and name holes | `pcre2pattern.3` (Homebrew) lines 320 (`\g{2} or \k{name}`), 624, 726 to 728 (`\p{Nd}`, `\p{L}`), 930, 2305 (`[dume]{3}`), 3207 |
| GLib GVariant 2.90.0 | `a{sv}`, `{sv}` | name hole | `gvarianttype.h:279`, `#define G_VARIANT_TYPE_VARDICT ((const GVariantType *) "a{sv}")` |
| libdbus 1.16.2 signatures | `a{sv}`, `{sv}` | name hole | `dbus-protocol.h:166` and `:170`, `DBUS_DICT_ENTRY_BEGIN_CHAR '{'`, `..._END_CHAR '}'` |
| Tcl, SDK's `Tcl.framework` (`tcl.h` present) | `set name {World}` | name hole | `man n Tcl` rule [6] *Braces*; run: `echo 'set name {World}; proc greet {who} {return "hello $who"}; puts [greet $name]' \| /usr/bin/tclsh` printed `hello World` |
| ICU 78 MessageFormat (`umsg.h`) | `{0} est {1, select, ...}` | integer hole | `umsg.h:141`, the header's own example pattern |
| SQLite 3.51.0 FTS5 (rung 3 of §4.19's ladder) | `MATCH '{title} : heroes'` | name hole | run: `sqlite3 :memory: 'pragma compile_options'` lists `ENABLE_FTS5`; a table `fts5(title, body)` with rows `('heroes','a language')` and `('other','heroes in the body')`: `MATCH '{title} : heroes'` returned `heroes` only, `'{body} : heroes'` returned `other` only, bare `'heroes'` returned 2 |

So the class is not exotic: **two of the seven are named by design.md itself**,
PCRE2 in §1.11's table and SQLite as §4.19's rung 3, and POSIX regex ships with
the C library on both Unix platforms this project targets. The search that
found them was: the coordinator's two candidates, then libraries whose own
syntax uses braces (regex bounds, template placeholders, Tcl quoting, FTS5
column sets), each then checked against what is installed here (`brew
--prefix`, `pkg-config --modversion`, `xcrun --show-sdk-path`). Not found on
this Mac and therefore **unverified, listed as questions only**: Jansson's pack
format, libpq's array and path literals (`'{name}'`), hiredis with Redis
cluster hash tags (`{user1000}`), Lua table constructors (`{x}`).

### Q1.2 What route (1a)'s own spelling hands C, measured

Route (1a) says braces meant as text are written `{{`...`}}` in an `f` literal.
Built and run on today's compiler (`<seat>/work/q1/braces.hero`, `./heroes
check` exit 0, `./heroes run` exit 0), each line's output beside it:

```
print(f"[0-9]{{3}}")    [0-9]{3}}
print(f"[0-9]{{3}")     [0-9]{3}
print(f"a}b")           a}b
print(f"a}}b")          a}}b
print(f"a{{sv}}")       a{sv}}
print("a{sv}")          a{sv}
print("[0-9]{3}")       [0-9]{3}
```

**Only `{{` is an escape; `}}` is two braces.** That is what spec § 2 says
(`spec/heroes-spec.md:52`, *`{{` writes one brace*, and nothing about `}}`),
and the compiler agrees. So **the spelling route (1a) names gives C the wrong
bytes**: `f"[0-9]{{3}}"` is the regex `[0-9]{3}}` and `f"a{{sv}}"` is the type
string `a{sv}}`. The spelling that gives the right bytes today is the
asymmetric `{{`...`}`, which is not the one the two languages run here teach:
Python 3.14.7 prints `[0-9]{3}` for `f"[0-9]{{3}}"` and refuses `f"a}b"` with
*SyntaxError: f-string: single '}' is not allowed*; Rust 1.90.0 prints
`[0-9]{3}` for `println!("[0-9]{{3}}")` and refuses `println!("a}b")` with
*invalid format string: unmatched `}` found* (both run at 17:25; C# is not
installed here, unrun). So in both, `}}` is the escape and a lone `}` an
error; in Heroes `}}` is two braces and a lone `}` is text, and a writer with
either prior gets the wrong bytes with no message. The route's own text,
written by the coordinator, already makes the mistake a binding author would
make.

### Q1.3 The binding judged most likely: POSIX regex, built and run

Regex is the one I judge most likely in a real binding: a program that
checks its input reaches for a pattern before anything else, PCRE2 is in
design.md §1.11's own table, and POSIX regex needs no install on Darwin or
Linux. PCRE2 itself needs a wrapper header first, because `pcre2.h:1057-1058`
is `#error PCRE2_CODE_UNIT_WIDTH must be defined before including pcre2.h`
and its `pkg-config --cflags` is only `-I...` (a separate friction, not this
question's).

`<seat>/work/q1/regex/notag.hero`, the binding (verified by clang against the
SDK's `_regex.h:113-118` and `:220-228`):

```
extern "regex.h"
    constant REG_EXTENDED: i32
    constant REG_NOMATCH: i64
    record regex_t partial
        re_nsub: u64
    function regcomp(@preg: regex_t, pattern: cstr lent, cflags: i32) -> i64
    function regexec(@preg: regex_t, text: cstr lent, nmatch: u64, pmatch: ptr, eflags: i32) -> i64
    function regfree(@preg: regex_t)
```

and a `report(label:, pattern:)` that prints the bytes and whether
`555-1234` and `555}-1234}` match. `check` exit 0, `build` exit 0, run exit 0:

```
plain, today       C receives: ^[0-9]{3}-[0-9]{4}$
    555-1234   -> true
    555}-1234} -> false
f, {{ and }} (1a)  C receives: ^[0-9]{3}}-[0-9]{4}}$
    555-1234   -> false
    555}-1234} -> true
f, {{ and }        C receives: ^[0-9]{3}-[0-9]{4}$
    555-1234   -> true
    555}-1234} -> false
```

**Route (1a)'s stated spelling is a valid regex that matches a different
language, with every exit 0.** That is the silent wrong answer this project
exists to turn into a compile error, produced by the rule meant to prevent
one.

### Q1.4 The same on rung 3 of §4.19's ladder: SQLite FTS5, and (1b)'s scope

`<seat>/work/q1/fts5/main.hero`: the SDK's `sqlite3.h`, the handles and marks
of `examples/sqlite/main.hero`, a table `fts5(title, body)`, and

```
function bodies_titled(db: Db, title: str) -> [str]
    sql = "SELECT body FROM docs WHERE docs MATCH '{title} : " + title + "'"
```

The parameter is named for the column it searches, which is the name a reader
would give it. Today: `check` 0, `build` 0, run 0, `titled heroes: a language`
and `titled other: heroes in the body`, which is right. Then the line alone
changed to each spelling a refusal would push its author toward, every one
`check` 0, `build` 0, run 0:

| line 23 written as | C receives | output |
|---|---|---|
| `"...MATCH '{title} : "` (today) | `{title} : heroes` | `a language` / `heroes in the body` |
| `f"...MATCH '{title} : "` (the `f` the (1a)/(1b) fix writes) | `heroes : heroes` | empty / empty |
| `f"...MATCH '{{title}} : "` ((1a)'s stated escape) | `{title}} : heroes` | empty / empty |
| `f"...MATCH '{{title} : "` (the asymmetric escape) | `{title} : heroes` | `a language` / `heroes in the body` |

The two empty rows are SQLite failing at step (`sqlite3 :memory:` with the same
table: *no such column: heroes*, and *fts5: syntax error near "}"*, both exit
1 from the CLI). The program's loop tests `== SQLITE_ROW`, as
`examples/sqlite/main.hero:95` does, so the error reads as no rows: part of
the silence is the program's, and it is the shape the repository's own example
teaches.

### Q1.5 Positions where a brace-bearing literal stands, measured

- an argument, a binding, a concatenation: an `f` literal stands there
  (above);
- **a constant's body**: an `f` literal is accepted (`<seat>/work/q1/fconst.hero`,
  `constant VARDICT: str` with body `f"a{{sv}"`, `check` exit 0);
- **a `match` pattern**: an `f` literal is refused,
  `error[expected_pattern]: expected a pattern, found a piece of an
  interpolated string`, at `<seat>/work/q1/fpattern.hero:3:9`; the plain
  `"a{sv}" => 1` checks and runs, printing `1` (`plainpattern.hero`). So under
  (1a) as worded, a `match` that dispatches on a D-Bus or GVariant signature
  has **no spelling as a `match`**; an `if` chain over `==` against an `f`
  literal is what remains.

### Side finding, outside the question: `ffi_unknown_tag`'s note on a typedef'd anonymous struct

Written first as `record Regex tag regex_t partial`, the binding was refused
with `error[ffi_unknown_tag]: struct regex_t has no fields in regex.h` and
three `ffi_parameter_type`. The cause is `selfhost/emit/ctype.hero:149`: a
tagged record with fields is always written `struct <tag>`, and `regex_t` is
`typedef struct { ... } regex_t;` with no tag. The spec's rule already covers
it (§ 13: *the same name unless the header writes it after the word struct,
which `tag` gives*), and the record named `regex_t` is the spelling that works.
But the note offers two repairs, a misspelled tag or a handle, and **neither
is this one**; the handle repair (`<seat>/work/q1/regex/handle.hero`, `record
Regex tag regex_t`, `regcomp(re, ...)` with `re` at `nullptr`) checks and
builds at exit 0 and aborts at run, `panic: a null pointer was read through, at
offset 0x8`, exit 134. The runtime's guard held (a message, no corruption), so
this is a §4.17 defect in a note, not a §1.12 one. `div_t`, `ldiv_t` and
`lldiv_t` are the same C shape in the C standard library itself, `typedef
struct { ... } div_t;` with no tag (the SDK's `_stdlib.h:102-116`, read
17:40). For the coordinator to file or not.

### Q1.6 GVariant and libdbus, the coordinator's two candidates, built and run

`<seat>/work/q1/gvariant/main.hero`: `extern "glib.h" package "glib-2.0"` with
`g_variant_type_string_is_valid(type_string: cstr lent) -> i64`, and `extern
"dbus/dbus.h" package "dbus-1"` with `dbus_signature_validate(signature: cstr
lent, error: ptr) -> u32`. `check` 0, `build` 0, run 0:

```
plain, today       C receives: a{sv}    GLib valid: true    libdbus valid: true
f, {{ and }} (1a)  C receives: a{sv}}   GLib valid: false   libdbus valid: false
f, {{ and }        C receives: a{sv}    GLib valid: true    libdbus valid: true
```

Here the libraries catch the wrong bytes, if the program asks them to. And the
`f` fix alone, `f"a{sv}"`, is loud: `error[unknown_name]: nothing named `sv`
is in scope` (`<seat>/work/q1/fixgv.hero:2:15`, exit 1). By contrast the `f`
fix on the regex, `f"^[0-9]{3}-[0-9]{4}$"`, prints `^[0-9]3-[0-9]4$`
(`fixregex.hero`, exit 0): the bounds become digits, silently.

(Aside, one line: `-> u64` against the header's `dbus_bool_t`, a `uint32_t`,
was refused with `ffi_return_type` and a note naming no type to write; spec
§ 13 says *a result may be wider than C's*, and the assertion reads "wider" for
`i64` and `f64` only, `selfhost/emit/extern_assert.hero:50-79`.)

### Q1.7 The forgotten `f` at the C boundary, measured

`<seat>/work/q1/lease/`: `examples/gallery/13-lease.hero` and its header
copied, and line 23's `f` removed (`nof.hero`, the one-line `diff` checked).
Both build and run at exit 0; with the `f`, C reads `13 bytes` three times,
without it `16 bytes` three times: C holds `row-{at}-payload`, the same label on
every row. This is what the question is about, and it is invisible to C and to
the program alike.

### Q1.8 What the tree's `f` literals already hold, measured by the lexer

`<seat>/work/q1/lexall/scan.py`: `./heroes lex <file> --dump-tokens` over
every `.hero` file in the seat's copy of `a294a6ff`, 1,389 files, 1,272 lexed
(117 refused on purpose: 102 under `tests/golden/check`, 10 surface fixtures, 5
panel briefs), `/usr/bin/time -p` real 12.64 user 26.83. **50 `f` literals, 118
pieces.** A piece's text holds `}}` in 4 pieces in 2 files, both goldens that
test braces
(`tests/golden/run/fixedbugs-an-interpolated-string-holds-characters-above-ascii.hero:25`
and `:26`, `tests/golden/run/interpolation-holes-and-braces.hero:13`); a lone
`}` in 1, `examples/gallery/12-interpolation.hero:29`, `{{like this}`, the
gallery teaching the asymmetric escape. The compiler's own code holds none (a
grep's 37 `selfhost/` lines are comments and strings ending in `f"`, read).

The asymmetry is recorded, not accidental. Panel 121's proposal said *`{{` and
`}}` write one brace* (`docs/panel/121-the-brace-was-already-taken.md:28`); the
rule that landed says only `{{` (`spec/heroes-spec.md:52`), and
`tests/golden/run/interpolation-holes-and-braces.hero`'s comment says *a `}` in
the text is text*, its `.expected:2` pinning `4 and {braces}} and 5`. And the
project had already paid for the same asymmetry once:
`docs/records/journal/029-corpus-coverage.md:86-90`, *`{{` escaped and `}}` did
not, so `{{name}}`, what a writer types for a literal `{name}`, came back as
`error[stray_brace]`. Python's and Rust's format strings ... double both.*

A regex whose bound comes from a variable needs an `f` literal under every
route: `f"^[0-9]{{{digits}}}$"`, Python's spelling (which Python 3.14.7 prints
as `^[0-9]{3}$`), hands C `^[0-9]{3}}$` in Heroes; Heroes' own
`f"^[0-9]{{{digits}}$"` hands it `^[0-9]{3}$` (`<seat>/work/q1/regex/bound.hero`,
exit 0).

### Q1.9 Each route, by its own words, on the three bindings

| | POSIX regex `"^[0-9]{3}-[0-9]{4}$"` | FTS5 `"...MATCH '{title} : "` in `bodies_titled(db, title:)` | GVariant `"a{sv}"` |
|---|---|---|---|
| **(1a)** | refused (`3` parses). The `f` fix: `^[0-9]3-[0-9]4$`, silent. The route's `{{3}}`: `{3}}`, silent, wrong language. Left: `f"...{{3}-...{{4}$"`, or `"^[0-9]{" + "3}-..."` (unrun) | refused. The `f` fix and the route's `{{title}}`: empty result, exit 0. Left: `f"...'{{title} : "` | refused. The `f` fix: `unknown_name`, loud. The route's `{{sv}}`: invalid to both libraries. Left: `f"a{{sv}"`; **as a `match` pattern, none** |
| **(1b)** | the hole has no names, and *only where the hole's names are bound* does not say what that means for none: refused under "all of them", accepted under "some" | **refused, because the parameter is named `title`**, and the fixes as in (1a); rename the parameter `word` and it is accepted | accepted unless something is named `sv` |
| **(1c)** | unchanged | unchanged; were `title` unused, the new message would suggest the `f`, and following it gives the empty result, so its fix must be a `guess` | unchanged |
| **(1d)** | unchanged | unchanged | unchanged |

**Every binding keeps a spelling under every route**, so the veto condition,
*a binding the route makes impossible to write*, is not met, with one position
excepted: a `match` arm on a signature under (1a) and (1b), where an `if` chain
over `==` remains. What (1a) and (1b) do instead is worse in kind than a
missing spelling: on the two bindings design.md names (PCRE2's syntax through
POSIX's, SQLite as rung 3), **each spelling a refused author is pointed at,
the fix and the route's own escape, compiles, runs at exit 0, and hands C
something else**. Only the asymmetric `{{`...`}`, which Python and Rust both
refuse as a lone `}`, is right.

### Q1.10 A route nobody listed

**(1e) Make the escape symmetric: in an `f` literal, `}}` writes one `}`, and a
lone `}` in the text is an error**, as Python 3.14.7 and Rust 1.90.0 do (run,
Q1.2), and as `examples/template` was repaired to do after journal 029:
`examples/template/main.hero:64-66`, *`}}` is one literal brace, symmetrically
with `{{`*, a lone `}` failing `stray_brace` (`:76`, `:80`), and `:178`
asserting `render("{{name}}") == "{name}"` (read 17:58). So the repository
holds two opposite rules for one brace, its template example's and its
language's. It is
not an alternative to (1a) to (1d) but what makes (1a) or (1b) safe at the
boundary, and it removes a trap that exists under (1c) and (1d) too, for every
`f` literal that holds a brace as text. Cost, measured by the lexer (Q1.8) over the
1,272 files that lex: 3 files, the two goldens whose purpose is braces and the
gallery line, and their expected outputs (the 117 that do not lex were not
read for it); it reverses a ruling of panel 121's landing, so it is a
language change of its own. Unmeasured: whether a fresh model writes `}}` or
`}` for a literal closing brace in a Heroes `f` literal. That is a paid run,
which this seat did not make; the run worth making, for the coordinator to
decide: ten fresh sessions, one prompt each, spec § 2 in context, each asked
to write the regex `[0-9]{n}` with `n` a Heroes binding, counting `}}}`
against `}}` in the answers.

### Q1 verdict

- **(1a): object.** No ABI change and no binding made impossible, so no veto;
  but on POSIX regex and SQLite FTS5 the route turns a correct plain literal
  into a refusal whose two offered repairs are silent wrong answers, measured
  at exit 0. Condition to approve: (1e) lands first or with it, the fix offers
  both readings (interpolate, or escape the brace) as `guess`, and either a
  hole-free `f` literal becomes a legal pattern or patterns are exempt.
- **(1b): object**, on the same measurements plus one of its own: FTS5's
  literal is refused because a parameter is named for its column, and accepted
  when renamed. Same condition, plus a ruling on a hole with no names.
- **(1c): approve.** It touches no string C receives; its fix must be `guess`.
  Condition: object if the fix is `certain`, or if the message fires where the
  binding is used.
- **(1d): approve at the boundary**, which it leaves as it is, and it leaves
  Q1.7's C-bound silence.
- **(1e): approve**, as the companion any of them needs; it is the one change
  here that removes a silent wrong byte at the boundary without refusing a
  correct program.

## Question 2 at the C boundary

### Q2.1 Can the FFI say a C function never returns? No, measured three ways

- spec § 13: `grep -n -i -E 'noreturn|never return|does not return|diverg|leaves'
  spec/heroes-spec.md` prints nothing, and the `Member` production
  (`spec/heroes-spec.md:423-427`) has no word after the parameters but the
  result, `owned`, the five life marks and `when`;
- the compiler: `grep -rn -i -E '_Noreturn|noreturn|never_returns|never
  returns|diverg' selfhost/` finds the word only in comments about the
  runtime's own `_Noreturn` functions (`hero_panic`, `hero_unreachable`,
  `emit/structural.hero:182`) and in the checker's `Outcome.jumps`, which is
  about the three jump words;
- **the language's own case**: `exit(code: i64)` is Tier 2 library code whose
  whole body is `hero_exit(code)` (`selfhost/library_source.hero:256-260`), over
  an `extern` declared `function hero_exit(code: i64)` with no mark (`:157`),
  against `_Noreturn void hero_exit(int64_t code);` in `runtime/hero_os.h:111`,
  whose comment says the `_Noreturn` *is not decoration*. So the one C
  function the language itself binds that never returns cannot say so either,
  and whatever (2a) decides about `exit(code:)` it decides by the name, unless
  the FFI gains a word.

### Q2.2 What `missing_return` demands after one today, built

`<seat>/work/q2/abort-last.hero`: `extern "stdlib.h"` with `function abort()`,
and `function sign(x: i64) -> i64` whose third path is `abort()` as its last
statement: `error[missing_return]: sign must return i64, and one path through
it returns nothing`, at `:5:26`, exit 1. `abort-then-return.hero`, the same
with `return 0` after `abort()`: `check` exit 0. The emitted C
(`build --emit-c`, `abort-then-return.c`) keeps the dead statement, because the
IR does not know the call leaves, so the return shares the call's block:

```c
bb4:
    (void)abort();
    t9 = INT64_C(0);
    return t9;
```

Clang says nothing about it under the project's flags (the build is exit 0;
`-Wunreachable-code` is not among `selfhost/cli/flags.hero:97-107`). So today a
binding's author writes one statement no path reaches after every
non-returning C call that ends a non-`()` function, as the brief measured for
`exit`, `assert false` and `while true`.

### Q2.3 What the headers give the question, measured

**How many functions would need a mark.** Counted with `grep -c -E
'__dead2|_Noreturn|noreturn'` over the SDK and Homebrew headers:

- **zero in every header of §4.19's five rungs**: `stdio.h`, `_stdio.h`,
  `math.h`, `sqlite3.h`, `curl/*.h`, `raylib.h`; and SDL3's public headers only
  define `SDL_NORETURN` and use it nowhere;
- libc: `abort` (`_abort.h:33`), `exit`, `quick_exit`, `_Exit`
  (`_stdlib.h:165`, `:184`, `:215`), `_exit` (`unistd.h:442`),
  `pthread_exit` (`pthread.h:356`), `err`, `verr`, `errc`, `verrc`, `errx`,
  `verrx` (`err.h:76-81`), `longjmp`, `_longjmp`, `siglongjmp`
  (`setjmp.h:85-91`);
- GLib 2.90.0: `g_abort` (`gutils.h:472`, `G_NORETURN`), and in
  `gmessages.h`, `gtestutils.h` and `gthread.h` further declarations, some
  `G_NORETURN` and some only `G_ANALYZER_NORETURN` (`grep -n`, not counted
  one by one).

**What the header's words are worth.** GLib marks `g_critical`, which returns,
with `G_ANALYZER_NORETURN` (`gmessages.h:422`) beside the real `G_NORETURN` of
`g_abort`: a header's spelling of the fact is not one fact. What counts is
what clang's flow analysis believes, and that can be asked.

**Two ways to ask clang, run** (`<seat>/work/q2/`):

- a `_Generic` over a noreturn function type, the shape of the FFI's existing
  `_Static_assert(HERO_RET_...)`: **it cannot tell them apart**. With `typedef
  void noret_v(void) __attribute__((noreturn));`, a plain `void my_plain(void)`
  also matches `noret_v *` (`probe_generic.c:9`, the assertion that it does not
  fails). C does not make noreturn part of type compatibility;
- **a probe whose body is only the call, in a non-void function, under
  `-Werror=return-type`**, which the project already passes
  (`selfhost/cli/flags.hero:98`): accepted for all ten non-returning
  functions tried (`abort`, `exit`, `_exit`, `_Exit`, `quick_exit`,
  `pthread_exit`, `err`, `errx`, `longjmp`, `g_abort`) and refused with *non-void function
  does not return a value* for `puts`, a plain `void(void)`, and `g_critical`.
  Written as the emitter writes its probes today, through the parenthesized
  name with typed parameters (`paren.c`: `static int64_t
  hero_ffi_noreturn_probe_abort(void) { (abort)(); }`), it holds: exit 0.

So a mark that says a C function never returns **can be verified against the
real header at build**, as a result type is: a wrong mark would be a build
error naming the function, which is §1.11's third point applied to the new
word. It cannot be verified by `check`, which runs no clang (measured in
Q1.3: `check` exit 0 and `build` exit 1 on the same wrong binding), so under
a mark `check` trusts the source, as it trusts `-> i64`, and `build` refuses a
lie.

**What the probe cannot see.** `longjmp` passes, and so would a library's
fatal hook that longjmps (libpng's and Lua's error paths are that shape;
unverified here, neither is installed). A mark would then call it leaving,
which it is, into a C frame past every Heroes frame between, skipping their
releases and copy-outs: §4.6's reason for having no exceptions, *unwinding
across the C boundary is undefined behaviour*, in C's own spelling. That is a
hazard of binding such a function at all, mark or none, and it is not this
question's; it is written here so no one reads the probe as blessing it.

### Q2.4 Does the compiler need it (Principle 0)?

The compiler calls `exit` six times, all in `main`
(`selfhost/main.hero:90`, `:98`, `:125`, `:128`, `:161`, `:188`), a `()`
function where `missing_return` never applies, and none is followed by a
statement in its block (a scan of every `exit(` statement in `selfhost/`). So
the compiler pays nothing today, and a mark enters, if at all, on the thesis,
not on compiler-need.

### Q2 verdict

- **(2a): approve at the boundary, on one condition.** An `extern` call counts
  as leaving only through a mark on its declaration, verified at build by the
  flow probe above, and never by reading the header at `check` time, which
  would make one program's legality depend on the platform's header (`abort`
  is `__dead2` in this SDK's `_abort.h:33`; glibc's and the Windows SDK's
  headers were not read by this seat). Without a mark, the predicate leaves `extern` calls
  out, and a binding's author keeps writing the dead `return`, which (2a)'s
  first half does not refuse because the call is not known to leave: a legal
  spelling survives. If (2a) counts `exit(code:)`, the record should say that
  it does so by the name, since `hero_exit` carries no mark. Cost of a mark to
  a binding author, measured: one word on each of the libc functions listed in
  Q2.3 and on GLib's `g_abort`, and none on any rung of the ladder. The word
  itself is a spec sentence and a probe per marked `extern`, which I leave to
  the spec-warden and the compiler-engineer to price.
- **(2b): approve**; the boundary is unchanged.
- **(2c): approve**; the boundary is unchanged.
- **No veto on any route**: none changes a byte C receives, and none makes a
  binding impossible to write.

## Question 3 at the C boundary

### Q3.1 The two references, from their own texts

**C11 §5.2.4.1**, from the standard's text: the WG14 committee draft N1570
(2011-04-12), fetched from `https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf`
(HTTP 200, 1,692,004 bytes, SHA-256 `208969fe...8492e`, a public document and
no paid run) and read with PDFKit, printed pages 25 and 26:

> 1 The implementation shall be able to translate and execute at least one
> program that contains at least one instance of every one of the following
> limits:18)
> - 127 nesting levels of blocks
> - 63 nesting levels of conditional inclusion
> - ...
> - 63 nesting levels of parenthesized expressions within a full expression
> - ...
> - 511 identifiers with block scope declared in one block
> - ...
> - 127 arguments in one function call
> - ...
> - 4095 characters in a logical source line
> - 4095 characters in a string literal (after concatenation)
> - ...
> - 1023 case labels for a switch statement (excluding those for any nested
>   switch statements)
>
> 18) Implementations should avoid imposing fixed translation limits whenever
> possible.

These are minimums a conforming compiler must reach in one program, not
maxima a program must stay under; footnote 18 says so.

**clang's `-fbracket-depth`.** `clang -cc1 --help` describes it, *Maximum
nesting level for parentheses, brackets, and braces*, and prints no default.
Measured on Apple clang 21.0.0 (clang-2100.3.34.2), arm64, `ulimit -s` 8176,
`clang -std=c11 -fsyntax-only` on generated files (`<seat>/work/q3/`):

| nesting in one function | compiles up to | then |
|---|---|---|
| blocks `{ { ... } }` | 2,047 | 2,048: `error: bracket nesting level exceeded maximum of 2048` |
| parentheses `((( 1 )))` | 1,156 | 1,157: **clang crashes**, `clang frontend command failed due to signal`, `Illegal instruction: 4` |
| calls `g(g(g(1)))` | 1,500 | 2,047: crashes the same way (between them not bisected) |
| subscripts `a[a[a[0]]]` | 1,000 | 5,000: crashes the same way (between them not bisected) |

So the default here is **2048, not the 256 this seat expected from memory**,
and on expressions clang's own stack gives out first: **were the emitter ever
to nest an expression more than 1,156 deep, clang would crash before its
limit spoke**, with no diagnostic. Q3.2 measures that it does not. The Linux (Debian clang
22.1.8) and Windows (LLVM clang 22.1.8 and 23.1.1, `x86_64-pc-windows-msvc`)
compilers are the other targets (`docs/ref/environment/linux/LINUX-MACHINE.md:61`,
`docs/ref/environment/windows/WINDOWS-MACHINE.md:330`, `:341`); every target is
a clang, none is MSVC's `cl` or GCC, and **none of these numbers was run on
them**.

### Q3.2 Does a source's nesting reach the emitted C? No, measured

Each shape of 00-shared.md written by `depth.py` (copied from the briefs
directory) at the largest depth `check` accepted there, `check` then `build
--emit-c` on this seat's compiler, then `<seat>/work/q3/cnest.py` over the C:
the deepest `(`, `[`, `{` and all three together, outside comments and
literals, preprocessor lines counted apart.

| shape, depth | `check` | `--emit-c` | deepest `(` `[` `{`, all | where | identifiers declared at a function's top block |
|---|---|---|---|---|---|
| Q2's `abort-then-return.c`, a baseline | 0 | 0 | 4, 0, 2; 4 | line 37 | 15 |
| `x = ((...1...))`, 500 | 0 | 0 | 4, 0, 2; 4 | line 35 | 4 |
| `x = [[...1...]]`, 200 | 0 | 0 | 4, 0, 2; 4 | line 35 | 405 |
| `x = g(g(...1...))`, 90 | 0 | 0 | 4, 0, 2; 4 | line 35 | 94 |
| `x = g(g(...))`, 100 | 0 | **134**, `panic: stack exhausted in checklower.named` | | | |
| `x = - - ... 1`, 200 | 0 | 0 | 4, 0, 2; 4 | line 35 | 203 |
| `b = !!...true`, 200 | 0 | 0 | 4, 0, 2; 4 | line 35 | 204 |
| `x = 1 + 1 + ...`, 200 | 0 | 0 | 4, 0, 2; 4 | line 35 | 404 |
| `if true` nested, 100 | 0 | 0 | 4, 0, 2; 4 | line 35 | 302 |
| `f"{f"{...}"}"`, 200 | 0 | 0 | 4, 0, 2; 4 | line 35 | 6 |

**Line 35 is fixed text**, the library's own assertion
`_Static_assert(HERO_RET_INT(hero_file_write(0, (HeroStr){0})), ...)`, the same
in every file (`sed -n 35p`). So in every shape measured, **the emitted C nests
4 deep whatever the source's depth**: the lowering is three-address, a nested
expression becomes one temporary per node (`paren-closed-500.c`'s whole `main`
is `t1 = INT64_C(1); h0_x = t1; t2 = h0_x; hero_print_int(t2);`), and a nested
`if` becomes labelled blocks and `goto` (`if-nested-100.c`: 301 labels, each
`if` written `if (t1) goto bb2; else goto bb3;`). None of the files holds a
`switch`. The widest line in every file is 223 characters, the preamble's.

**What does grow with the source is the count of temporaries declared at the
top of one function**, 405 for 200 nested lists: C11's minimum for that is 511
per block, and no measured compiler here refuses more (below).

Re-run after the second interruption, one at a time (`<seat>/work/q3/deep/rest.sh`,
`/usr/bin/time -p` on each `--emit-c`, load average 6.6 to 16 at 20:04):

| shape, depth | `check` | `--emit-c` (real, user) | deepest, all | identifiers at a function's top block |
|---|---|---|---|---|
| `b = true && true && ...`, 200 | 0 | 0 (0.61 s, 0.55 s) | 4, line 35 | 804 |
| `x = v.to_str().len()...`, 150 calls | 0 | 0 (0.43 s, 0.38 s) | 4, line 35 | 231 |
| `if`, 1,999 `else if` and an `else`, 2,000 | 0 | 0 (21.36 s, 20.93 s; 4,172,443 bytes) | 4, line 35 | 10,005 |
| 10,000 `print(i)` statements in one block | 0 | 0 (17.16 s, 16.61 s; 6,290,530 bytes) | 4, line 35 | 10,001 |

**The three kinds of 00-shared.md, nesting, one-operand chains and flat
chains, all reach C at the same depth, 4**, the preamble's. The flat chains
are no exception: `&&` lowers to short-circuit blocks and temporaries, the
method chain to one temporary per call.

### Q3.3 An `extern` call nested in another reaches C through temporaries

`<seat>/work/q3/extern/fabs-50.hero`: `extern "math.h"` with `function
fabs(x: f64) -> f64`, and `x = fabs(fabs(...fabs(-2.5)...))`, 50 deep: `check`
0, `--emit-c` 0, and the C is 50 statements, `t3 = fabs(t2); t4 = fabs(t3);
...` (`grep -c 'fabs(t'` counts 50), deepest nesting 4 at the preamble's
assertion (`fabs-50.c:28`). So a C function never sees its argument as a
nested expression, and the header's own macros cannot multiply the text.

**A function-like macro cannot be bound at all today** (side finding): a header
of this seat's own, `twice.h`, `#define twice(x) ((x) + (x))`, bound as
`function twice(x: i64) -> i64`, and `<sys/wait.h>`'s `WEXITSTATUS`, a
function-like macro in the SDK (`sys/wait.h:144`, `:146`), bound as `function
WEXITSTATUS(status: i32) -> i64`: both `check` 0, both refused at build,
`error[ffi_unknown_name]: sys/wait.h declares no WEXITSTATUS`, then *clang
read the header and could not find it*. The header does declare it, as a macro; the
probe calls the callee parenthesized, `(void)(fabs)(a0)` in `fabs-50.c:65`,
which panel 092 chose so that `(memset)(...)` sees through a
`_FORTIFY_SOURCE` redirect (`docs/panel/092-the-check-that-a-macro-was-hiding.md:108-110`),
and a parenthesized name is not a macro invocation. design.md §1.11 says
*Macros, `inline` functions and `#define` constants are now reachable
directly*; measured, that is true of object-like macros and of functions, and
false of a function-like macro with no function behind it, which a program
calling `waitpid` needs for `WEXITSTATUS`. Not this sitting's question; for the
coordinator to file or not.

### Q3.4 Does clang accept what the emitter writes? On this Mac, at every size measured

Built and run on this Mac with this seat's compiler (`HEROES_RUNTIME=<seat>/runtime
./heroes build`, `/usr/bin/time -p`):

- `and-chain-200`: build 0, prints `true` (real 0.56, user 0.49);
- `else-if-chain-2000`: build 0, prints `1` (real 18.00, user 17.85), a
  function with **10,005** identifiers declared in its top block;
- `print-block-10000`: build 0, prints 10,000 lines, `0` to `9999` (real 10.58,
  user 10.43), **10,001** in one block.

So Apple clang 21.0.0 takes twenty times C11's minimum of 511 identifiers in
one block without a word, as footnote 18 invites. Linux and Windows unrun.

### Q3.5 What (3b)'s own-stack route costs at the boundary, measured on Darwin

The project already chooses the main thread's stack on Windows with a link
flag, `-Wl,/STACK:67108864`, for the compiler (`seed/README.md:17`) and for
every program it builds (`selfhost/cli/flags.hero:194`). The Darwin twin, run:
the seed compiler built with `clang -I runtime seed/heroes.c runtime/runtime.c
-Wl,-stack_size,0x10000000 -o heroes-256m` (real 4.03; `otool -l` reads
`LC_MAIN stacksize 268435456`, against `0` for the plain build), then `check`
with each compiler, one process at a time:

| shape, depth | plain compiler | `heroes-256m` |
|---|---|---|
| `x = ((...))`, 600 | 134, `stack exhausted in grammarexpr.postfix` | 0 |
| `x = ((...))`, 2,000 | 134, `grammarexpr.postfix` | 0 |
| `x = [[...]]`, 2,000 | 134, `grammarexpr.postfix` | 0 |
| `x = g(g(...))`, 2,000 | 134, `grammarexpr.postfix` | 0 |
| `if true` nested, 2,000 | 134, `grammarexpr.postfix` | 0 |
| `x = - - ... 1`, 2,000 | 134, `resolvewalk.expr` | 0 |

and `build` with `heroes-256m`: `call-closed-2000` 0 (real 1.51), which the
plain compiler cannot build past 90; `if-nested-2000` 0 (real 58.06, user
56.86: a compile-time cost for the compiler-engineer, not a boundary one);
`paren-closed-2000` 0; each binary prints `1`. Their C still nests 4 deep,
with 2,004 and 6,002 identifiers in one block, and clang takes it. The guard
of `runtime/parts/stack.c` followed the larger stack: no false panic. This
Mac's `ulimit -s` is 8,176 KB soft and 65,520 KB hard; whether these shapes
needed more than the hard limit was not measured. **Linux is unrun**, and so is
the question whether a Linux binary can choose its main thread's stack as
`LC_MAIN` does here; if it cannot, the route there is a thread with a chosen
stack or a raised limit. A question for the compiler-engineer, not a finding.

### Q3 verdict

The boundary does not choose between the routes, and that is the finding: **a
source's depth never reaches C as depth.** Every kind 00-shared.md names,
brackets, blocks, holes, one-operand chains and flat chains, emits C that
nests 4 deep at every depth measured (up to 2,000, and a 10,000-statement
block), because the lowering is three-address and control flow is `goto`. So
clang's limit (2048 here) and its crash (1,157 nested parentheses here) are
never approached, and **no C compiler this project targets would break a
Heroes promise first; the Heroes compiler's own `build` would**, since
lowering gives out before `check` does (`g(g(...))`: `check` holds 100,
`build` 90, the brief's measurement reproduced under `--emit-c`, exit 134 in
`checklower.named`).

- **(3a): approve at the boundary**, on the condition that its number is
  measured against `build`, lowering and emission included, on the platform
  with the smallest stack, and not against `check`: a limit `check` enforces
  and `build` cannot reach is a false promise.
- **(3b): approve at the boundary.** The C stays flat at any depth measured,
  and on Darwin the stack is one link flag, the one Windows already has.
- **(3c): approve at the boundary**, for the same reason as (3a) and on the
  same condition; nothing on the C side prefers it or (3a).
- **(3d): no objection from the boundary**, which it does not touch; the
  exit-code contract it breaks is `.claude/rules/cli-surface.md:40-42`, other
  seats' ground.
- **Condition on all four**: the lowering stays three-address for nested
  expressions. An emitter that one day writes `a + b + c` or `f(g(x))` as one
  C expression inherits clang's crash at 1,157 nested parentheses on this Mac,
  with no diagnostic, and must bound what it nests.

## The seat's answer

- **verdict**:
  Q1: (1a) **object**, (1b) **object**, (1c) **approve**, (1d) **approve**,
  and (1e), a route nobody listed, **approve**.
  Q2: (2a) **approve** on a condition, (2b) **approve**, (2c) **approve**.
  Q3: (3a) **approve** on a condition, (3b) **approve**, (3c) **approve** on
  the same condition, (3d) no objection from the boundary.
  **No veto**: no route changes a layout, an ownership or a byte C receives
  from a program that is legal today, and every binding built here keeps a
  spelling under every route.
- **section**: design.md §1.11 (*FFI ergonomics rank alongside comprehension*;
  *when a choice makes bindings harder, that is a serious cost, not a
  footnote*; its third point, a wrong `extern` is a compile error) and §4.19
  (the ladder, and its *silent-wrong-answer class this language exists to
  kill*); §4.17 for the fixes.
- **experiment**: POSIX regex (`regex.h`), SQLite FTS5 (`sqlite3.h`), GLib and
  libdbus bindings built and run with each route's spellings (Q1.3 to Q1.6);
  `abort` after `missing_return`, a flow probe under `-Werror=return-type`
  against libc and GLib, a `_Generic` probe that fails (Q2.2, Q2.3); twelve
  nesting shapes emitted, scanned, built and run, clang's own limits bisected,
  a 256 MB-stack compiler (Q3). clang accepted every binding in the form that
  finally built; what it refused on the way (`regex_t` under `tag`, a `u64`
  result against `uint32_t`, two function-like macros) is Q1.3, Q1.6 and
  Q3.3.
- **cost** (measured): under (1a), every C string whose syntax brackets a
  number or a name (seven interfaces verified, Q1.1) becomes an `f` literal
  with an asymmetric escape, and a `match` on one loses its spelling; (1e)
  moves 3 files (Q1.8); a Q2 mark would sit on 15 libc functions (16
  declarations, `_Exit` in two headers; `grep -h __dead2` over the six SDK
  headers of Q2.3) and GLib's `g_abort`, on no rung of the ladder; Q3 costs the boundary nothing.
- **argument**: A refusal is worth what its repair is worth. On regex and FTS5,
  (1a) and (1b) refuse correct C text, and both repairs they point to, writing
  the `f` and the route's own `{{`...`}}`, compile, build and run at exit 0 and
  hand C different bytes: a regex matching another language, a query returning
  nothing. The root is the escape: Heroes writes `}}` as two braces where
  Python and Rust write one, and the repository's own template example agrees
  with them. Fix the escape (1e) before refusing any literal; (1c) reaches C
  text not at all. Q2 and Q3 leave the boundary as it is, measured: a mark can
  be verified by a flow probe, and nesting never reaches C.
- **prediction**: (a) If (1a) lands without (1e), the first regex or FTS5
  binding written against it in this repository will hold `{{n}}` and hand C
  `{n}}`; checkable at the first such binding, by `grep -F '}}'` over `f`
  literals and running the program. (b) Under any Q3 route, the C emitted for
  every shape of 00-shared.md stays at nesting 4, the preamble's; checkable at
  the landing by `cnest.py` over `--emit-c`. (c) A mark verified by the flow
  probe refuses `g_critical` and accepts `g_abort`; checkable the day the
  word lands.
- **condition**: Q1's objections turn to approval if (1e) lands first or with
  the route, the refusal's fix offers both readings as a `guess`, and a
  hole-free `f` literal becomes a legal pattern or patterns are exempt; they
  would turn to a veto if a binding were found that has no spelling left.
  Q2's approval of (2a) turns to objection if an `extern` call is counted as
  leaving by reading the header at `check` time, or by a mark no build
  verifies. Q3's approvals turn to objection if the emitter comes to nest
  expressions without bounding them, or if a limit is set by what `check`
  holds rather than `build`.
