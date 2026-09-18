# Panel 162 — shared brief: the language goes from text to bytes and cannot come back

**Every number in this file was produced by a command run on 2026-09-18 while
this brief was being written, and the command is named beside it** (CL-077).
Nothing is copied from a document, a milestone file or an earlier sitting.

## The question

**What turns a run of bytes into a `str`, and what lets a program build a
fixed-array field without writing every element?**

Both halves block one program, which is why they are one sitting.

## The program that cannot be written

`heroes check`, this Mac, verbatim:

```
extern "sys/utsname.h"
    record Utsname tag utsname
        sysname: i8[256]
        …
    function uname(@name: Utsname) -> i32

function main()
    u: Utsname @ ???
    _ = uname(@u).to_i64().must()
    print(u.sysname)
```

```
error[bad_operand]: `print` takes any integer, a float, `bool` or `str`,
found `i8[256]`
```

## The four walls, each measured

| shape | the compiler's answer |
|---|---|
| `print(u.sysname)` where the field is `i8[256]` | `bad_operand`: *takes any integer, a float, `bool` or `str`* |
| `xs.to_str()` where `xs: [u8]` | `bad_operand`: *`to_str` takes an integer, a float, `bool` or `str`* |
| `print(xs)` where `xs: [u8]` | `bad_operand`, same list |
| `Utsname(sysname: [0])` | `fixed_array_length`: *holds exactly 256, and this literal has 1* |

And two escapes that are not escapes:

- **`partial` does not help.** A declared field's length is checked whether or
  not the record is marked `partial` — measured, same `fixed_array_length`.
- **A `tag` with no fields is accepted and is wrong.** `spec § 13`: *one with a
  `tag` and no fields is a handle*, C's pointer to that type, so `uname(@u)`
  would hand across a pointer to a pointer.

**A 256-element literal is 804 characters on one line**, measured by generating
it.

## The direction is the finding

Heroes goes from `str` to bytes and cannot come back. `spec § 10` gives
`s[i] -> u8` and `s.chars()`; `spec § 11`'s conversions are `to_str`, `to_f32`,
`to_f64` and the eight `to_<int>`; `spec § 13` gives `c.validated()`, which
answers a **`cstr`** and only a `cstr`, and the same section says *outside a
group nothing answers `cstr` and no record holds one*.

So this is not one name somebody forgot. It is a one-way door, and every program
that receives bytes from C is on the wrong side of it.

**The compiler agrees with the specification**, checked rather than assumed:
`grep -rn "from_utf8\|from_bytes\|str_from" selfhost/` returns nothing, and
`selfhost/inventory.hero`'s built-in list holds no inbound name.

## The scale, measured

- **24 `char[N]` fields of 321 total fields**, by a clang JSON AST walk over
  `sys/utsname.h`, `dirent.h`, `pwd.h`, `netinet/in.h`, `sys/socket.h` and
  `curl/curl.h`.
- **Zero** programs in `examples/` declare a fixed byte array:
  `grep -rln "i8\[\|u8\[" examples --include='*.hero'` returns nothing. **So
  Principle 0's compiler-need branch is closed in advance** — nothing in the
  closure list is waiting on this, and the thesis branch has to carry the
  proposal alone.
- `struct utsname`'s arrays are `char[65]` on Debian and `char[256]` on Darwin,
  measured on both, so **that struct stays per-platform whatever this sitting
  decides** and no resolution should be argued as making it portable.

## What the answer must not contradict

- **`spec § 3`: `str` is *immutable UTF-8*.** So bytes that are not valid UTF-8
  have to go somewhere: a `str?`, an abort, or a refusal to admit the operation.
  The sitting says which, and says it once.
- **`read_file(path: str) -> str?` already exists** and already turns arbitrary
  file bytes into a `str?`. **The failure mode is therefore already chosen
  somewhere in this compiler**, and a seat that finds where has found the
  cheapest possible answer — consistency with a rule the language already has
  beats a new rule, whatever its shape.
- **`spec § 10` fixes the outbound convention**, *`s[i]` yields a `u8`*. An
  inbound name that implies a different byte type would make the pair read
  wrong.
- **A C `char[N]` field is not NUL-terminated by rule.** `struct utsname`'s are;
  a fixed 4-byte tag field need not be. Any answer that assumes termination is
  making a claim about headers, and it owes the claim in writing.

## Routes named so far, and four is not a claim about the set (CL-057)

1. **A built-in that answers a byte run**, `to_str` widened or a new name, giving
   a `str?` on invalid UTF-8.
2. **A `cstr` view of a `char[N]` field**, so the existing `c.validated()` is the
   only inbound door and nothing new is named. `spec § 13` currently forbids a
   record holding a `cstr`, so this moves an existing rule rather than adding
   one.
3. **A slice-shaped answer**, so the caller says where the text stops and the
   NUL question never arises.
4. **Refuse**, and say a byte field is read one element at a time forever. Held
   to design.md Part 6's standard: a refusal names the program fact that would
   make it wrong.

**The build half has its own routes and the sitting owes them too**: a repeat
literal, a zero default for a fixed array in an `extern` record, an out-parameter
that does not require an initialised value, or a refusal that says a struct with
a long array is taken from the library and never built.

## Constraints every seat is held to

- **CLAUDE.md § Precedence**: robustness beats elegance, token cost, ergonomics,
  compiler size and speed. The most robust and production-ready resolution,
  never the cheapest and never a compromise.
- **Principle 0**: the compiler-need branch is closed (measured above), so the
  proposal enters only on a measured design.md Part 11 effect or a measured
  argument this panel accepts. **A seat that thinks the answer is to refuse
  should say so plainly; it is a live option here and not a formality.**
- **The spec budget, measured this session** with
  `./heroes measure spec/heroes-spec.md`: **5997** vendored, **7984** real
  (`claude-opus-5`, the binding number), ceiling **10240**, headroom **2256**,
  of which the FFI floor mortgages 60.
- **A vendored delta is not a price.** Price on the real instrument or write
  *lower bound* in those words.
- **Build in a copy.** `cp -r` the tree to your scratchpad, `rm -rf build`, then
  `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, a few seconds.
  **Never rebuild from `selfhost/`** (~20 minutes; it has killed seats). **Never
  `archive/bootstrap-rs/`.**
- The working tree is frozen for the duration of this sitting.

## What the sitting must deliver

A resolution the coordinator can implement inside M-readable-bytes, which closes
when the `uname` program above compiles and prints — or when a refusal says, in
design.md Part 6's own terms, why that program is not owed. Each seat gives:
verdict · the section it rests on · cost measured · a falsifiable prediction with
the milestone at which it becomes checkable · any condition or veto.
