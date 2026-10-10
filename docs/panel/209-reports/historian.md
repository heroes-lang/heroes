# Panel 209, the historian's report

Started after 16:48 by the shared brief's clock (the critic's first pass was
read at 16:48; this seat has no shell, so no `date` reading of its own), on
2026-10-10. Brief: `docs/panel/209-briefs/historian.md`; shared brief read
whole, its last section included. No tree, no Bash: every claim below is a web
source with its URL, read 2026-10-10, and the date the source itself states,
or it is written **unverified** in that word. Where *Heroes of code* pointed
the way (Wirth's Zurich for Q1 and Q4, Ritchie's Bell Labs for Q5 and Q6) it
is named here once, as a route; nothing below rests on it.

Written as it went, one section per question. **Unrun or unverified at the
time box**: Koka's `var x := e` syntax (the book page rendered only images past
its table of contents); the Zig commit that introduced the never-mutated check
(its mirror did not render); Ritchie's paper at its primary URL (HTTP 403;
a mirror is cited instead); the APL wiki's version table (HTTP 403; Dyalog's
own notes cited instead); any corpus frequency of Python's `@=`; the
founders' own rationales for `let mut`, `val`/`var` and `let`/`var` (only
community accounts found); Java's `final var`.

## Q1. A mutable declaration marked by a symbol rather than a keyword or a type

### Go's `:=` against `=`, and what happened to the shadowing complaint

**Verified.** The complaint is golang/go issue 377, *proposal: spec: various
changes to :=*, opened 3 December 2009 by agl: a `:=` inside an inner `if`
block creates a new `err` that shadows the function's named return, so a bare
`return` silently returns the wrong value
(https://github.com/golang/go/issues/377). **It is still open** after sixteen
years. The nearest thing to a ruling is Ian Lance Taylor's reply quoted on the
issue on 4 February 2025, *So I think it is very unlikely that we would adopt
this sort of change*, and his own comment the same day, *We simply disagree*;
on 29 January 2026 benhoyt asked whether it should be closed, with no reply
shown (same page).

**Verified.** golang/go issue 21114, *proposal: Go 2: prevent variable
aliasing/shadowing*, opened 21 July 2017, proposed that `:=` *reuse an already
existing variable* at least in multi-declarations like `logw, err := ...`;
closed by Taylor on 14 May 2018 with *This is covered by the discussion in
#377* (https://github.com/golang/go/issues/21114). Inside it, earthboundkid
suggested allowing shadowing with `var` but not with `:=`; Taylor questioned
whether a ban would be too broad (same page).

**Verified.** The tool built instead of a language change is the `shadow`
analyzer: the one *experimental* checker of the old `cmd/vet`, enabled only by
`-shadow`, split out of vet when vet moved to the analysis framework
(https://go.googlesource.com/tools/+/91ec7db2f84a6830ff144bd0cf14f8f47ecc152f;
its source is marked *Not part of the vet suite*,
https://go.googlesource.com/tools/+/412ee174ef74e13441079fb4bf293dd4101dcdcf/go/analysis/passes/shadow/shadow.go).
On 7 March 2023, on golang/go issue 58917 (a narrower variant), Alan Donovan
wrote that it is *not reporting (with high confidence) an actual mistake* and
*not the kind of thing we would enable by default in vet*
(https://github.com/golang/go/issues/58917, still open). On 9 September 2025
Donovan opened golang/go issue 75342 proposing to deprecate it: it *has at no
point in its history gotten remotely close to the signal-to-noise ratio we
expect*, *a failed experiment* (https://github.com/golang/go/issues/75342).
The thread then moved: on 30 September 2025 Donovan reported 4,100 matches
across a corpus, 8 of the first 10 inspected being clear errors, and called the
results *Promising*; the issue is open, in the proposals' *Incoming* column
(same page).

**What this says for Heroes.** A symbol pair where one line declares and
another re-binds the same name produced a defect class Go's maintainers
refused to fix in the language for sixteen years and could not catch in a tool
with acceptable precision for most of that time. Go's hole is `:=` in an
inner scope declaring what the writer meant to assign. Under R1 that hole
exists exactly where a writer types `@=` meaning `@` inside a nested block,
and the fresh cell shadows the outer one, **if Heroes lets an inner block
declare a name an outer block holds**. I have no tree, so that is a question
for the spec-warden, not a premise: if Heroes refuses that shadowing, Go's
class is closed by construction and R1's one-character typo is caught as
`unknown_name` or a redeclaration; if Heroes allows it, R1 imports issue 377
whole, and B2 does not close it (the inner cell is re-bound, so B2 is silent).

**Verified.** Odin, which took Go's shape, documents that `:` declares and `=`
assigns, so `:=` is not one token, and `::` declares a constant
(https://odin-lang.org/news/declaration-syntax/). Its compiler refuses a local
that shadows a return parameter at the same scope level and accepts it in a
nested block, which a forum thread calls inconsistent
(https://forum.odin-lang.org/t/shadowing-of-return-values/723); the August
2023 dev release split `-vet-shadowing` into its own flag
(https://newreleases.io/project/github/odin-lang/Odin/release/dev-2023-08).
The second language with Go's pair also reached for a vet flag rather than a
grammar rule.

### Wirth's `:=`: assignment only, never a declaration

**Verified.** *The Programming Language Oberon*, Wirth, revision dated
1.10.2013 / 3.5.2016 (HTML translation by Karl Landström,
https://www.miasap.se/obnc/oberon-report.html):
`ConstDeclaration = identdef "=" ConstExpression.`,
`VariableDeclaration = IdentList ":" type.`,
`assignment = designator ":=" expression.` So `:=` never declares; the
declaration is the `VAR` section with a mandatory type, and a `CONST` carries
no type. Standard Pascal's grammar is the same shape, `constant-definition =
identifier "=" constant` and `variable-declaration = identifier-list ":"
type`, in an EBNF transcription that says of itself it *cannot be taken as
being definitive* (https://www.fit.vutbr.cz/study/courses/APR/public/ebnf.html.en).

### F# and OCaml: a keyword or a type constructor, then a distinct symbol

**Verified.** F#: `let mutable x = 1` then `x <- x + 1`; the docs recommend
keeping mutable variables small in scope, and a mutable captured by a closure
is promoted to a `ref` (warning 3180)
(https://learn.microsoft.com/en-us/dotnet/fsharp/language-reference/values/).
Reference cells use `:=` and `!`, and the older page says to prefer `mutable`
where the compiler accepts it
(https://msdn.microsoft.com/library/dd233186(v=vs.100)). OCaml: `ref e`
allocates, `e1 := e2` stores and returns unit, `!e` reads
(https://ocaml.org/docs/mutability-imperative-control-flow;
https://courses.cs.cornell.edu/cs3110/2021sp/textbook/mut/refs.html). Both
languages mark the cell at birth by a word and re-bind by a symbol that is not
`=`: R5's shape.

### The keyword languages, and whether any recorded why

- **Rust** `let mut`: the fail-safe argument (forget `mut` and the compiler
  tells you) is the one the community gives
  (https://corrode.dev/blog/immutability/;
  https://jmmv.dev/2018/05/rust-review-immutable-by-default.html); a `var`
  alias for `let mut` was floated on internals and not adopted
  (https://internals.rust-lang.org/t/var-as-alias-for-let-mut/7346?page=2).
  A founder's design note: **unverified** (not found).
- **Kotlin** `val`/`var`: no official rationale found; a forum answer argues a
  declaration keyword *removes ambiguity between creating a variable and
  referencing one from a wider scope*, the typo creating a local where a
  property was meant
  (https://discuss.kotlinlang.org/t/why-not-have-everything-be-val-by-default/25391);
  another thread finds the two words too similar
  (https://discuss.kotlinlang.org/t/keywords-var-and-val-are-too-similar/75).
  Breslav's own account: **unverified** (a commenter recalls one and cannot
  find it).
- **Swift** `let`/`var`: a forum answer attributed to Lattner says the
  keyword is for humans, *let vs var is an expression of intention*, and part
  of the API contract for public declarations
  (https://developer.apple.com/forums/thread/28673;
  https://forums.swift.org/t/why-swift-uses-let-keyword-for-constants/88579);
  the attribution is the search index's, **unverified** by me.
- **Nim**: `var`, `let`, `const`, all three inferring the type from the
  initializer, *basically the only form of type inference that exists in
  Nim* (https://nim-lang.org/docs/tut1.html).
- **Zig**: `var`/`const`, with the never-mutated error (Q3).
- **Hylo** (the panel's lineage): `let gravity = 9.81` immutable, `var length
  = 1` mutable, and the re-binding is written **`&length = 2`**, the mutation
  site carrying a mark of its own
  (https://docs.hylo-lang.org/language-tour/bindings). **This is R5
  exactly**: a keyword at birth, a glyph at every write.
- **Dafny**: `var x := E;` declares and assigns in one statement, `x := E;`
  assigns, `:=` *pronounced "gets" or "becomes" (NOT "equals"!)*, and a
  local's type *can be left out and is inferred*
  (https://dafny.org/latest/Dafny-cheat-sheet.pdf;
  https://dafny.org/dafny/QuickReference). Keyword plus symbol again.
- **Koka** `var x := e`: **unverified** (page did not render).

### Erlang and Elixir: the two failure modes a declaration mark sits between

**Verified.** José Valim, *Comparing Elixir and Erlang variables*, 12 January
2016: *Elixir does not have mutable variables, it has rebinding*; in Erlang,
adding a `SafeValue` binding before a `case` silently turns a clause's fresh
binding into a match, *You have just silently introduced a potentially
dangerous bug in your code!*; Elixir's `^` makes the match explicit, so
removing the pinned definition *won't even compile*
(https://dashbit.co/blog/comparing-elixir-and-erlang-variables). Heroes'
`totl @ total + x` typo (design.md §4.4, per the brief) is the Erlang
accident mirrored: there a reused name becomes a match, here a new name would
become a cell. Elixir's answer was a mark on the re-use (`^`); Heroes' today is
a mark on the declaration (the type), and R1's is a different mark on the
declaration (`@=`).

### Did any language ship a symbol and withdraw it?

**Verified, none for a declaration symbol.** What was withdrawn: Rust's `@`
sigil, *Patterns with `@`-pointers have been removed* in 0.10 (April 2014) and
*`@T` has been removed from the language* in 0.11 (July 2014), with `~T`,
`~[T]`, `~str` the same release
(https://fuchsia.googlesource.com/third_party/rust/+/refs/tags/0.11.0/RELEASES.txt);
Perl 5's smartmatch `~~`, deprecated in 5.38, *scheduled for removal in Perl
v5.42.0*, removed in 5.41.3, then re-instated with removal *indefinitely
postponed* (https://metacpan.org/release/BOOK/perl-5.42.0/view/pod/perldelta.pod);
Ruby's `Kernel#=~`, removed in 3.2 (Feature #15231,
https://docs.ruby-lang.org/en/3.2/NEWS_md.html) after deprecation in 2.6
because it always returned nil and caused bugs when called by accident on an
Array (https://docs.ruby-lang.org/ja/2.7.0/method/Object/i/=3d=7e.html). Go
kept `:=` through issue 377. The searched set for a withdrawn declaration
symbol: Go, Odin, Rust, Perl, Raku, Ruby, Python, Pascal, Oberon, Dafny.

## Q2. `@=` elsewhere, and `@` as a binding glyph

**Verified.** PEP 465, created 20 February 2014, Status Final, Python 3.5:
`@` for matrix multiplication *together with the corresponding in-place
version* `@=`, mapped to `__imatmul__`; Python 3.5's What's New lists it
(https://peps.python.org/pep-0465/; https://docs.python.org/3/whatsnew/3.5.html).
The PEP counts uses of `dot` as a proxy for `@` (99 per 10,000 SLOC in
scikit-learn, 74 in nipy, 0 in the stdlib, *~780 uses* across the two
packages) and **counts nothing for `@=`**. How widespread `@=` is in corpora
a model has seen: **unverified**. What is verified is the meaning a
Python-trained reader carries: `@=` is the C `op=` habit, *apply `@` to the
existing value and store it back*. Under R1 `v @= 0` means the opposite, a
fresh cell; under R7 it means a re-bind, which is the habit.

**Verified, `@` has a BINDING meaning in three pattern languages.** Rust:
`variable @ subpattern` *binds the whole matched value to a variable while the
subpattern still applies*; the variable *will shadow any variables of the same
name in scope* (https://doc.rust-lang.org/reference/patterns.html). Haskell
2010: `apat → var [ @ apat ]` (as pattern), §3.17.1, and rule 8 of §3.17.2
(https://www.haskell.org/onlinereport/haskell2010/haskellch3.html). Scala:
`varid '@' Pattern3`, a pattern binder that *binds the variable name to that
value* (https://scala-lang.org/files/archive/spec/3.4/08-pattern-matching.html).
So `name @ ...` reading as *bind `name` here* has a shipped lineage; it is a
point for the `@` family and not for any one of R1, R7 or R10.

**Verified, `@` means *at these positions, amend* in the K/APL room.** k's
Amend At, `@[d;i;u]` and `@[d;i;v;vy]`, replaces the selected items by the
function's result (https://code.kx.com/ref/amend); Dyalog APL 16.0's release
notes list a new `@` operator, At
(https://docs.dyalog.com/16.0/Dyalog%20Version%2016.0%20Release%20Notes.pdf);
the release month, June 2017, is the APL wiki's version table as the search
index excerpted it, the page itself returning 403 to me (**unverified** as a
date). That is Heroes' *`@` re-binds a field or element inside one* in another
notation, a precedent for `@` as the write glyph, which every route but R7
keeps.

**Verified.** Ruby: `@name` is an instance variable, `@@name` a class
variable, both sigils in prefix position
(https://ruby-doc.org/3.3/syntax/assignment_rdoc.html). The brief says
`at_prefix` already names `@v` and `@@v` before a name as a known mistake with
a certain fix; that is R11's and R3's ground. No language was found that gives
`@` or `@=` a *declaring* meaning in infix position (searched: Python, Ruby,
Rust, Haskell, Scala, k, APL, Julia's and Java's prefix `@`).

## Q3. A mutable never mutated as an error

**Verified, one language makes it an error by default.** Zig 0.12.0, released
2024-04-20 (https://ziglang.org/download/): *Zig 0.12.0 introduces a new
compile error which is emitted when a local variable is declared as a `var`,
but the compiler can infer that `const` would suffice*, printed `error: local
variable is never mutated` / `note: consider using 'const'`
(https://ziglang.org/download/0.12.0/release-notes.html, section *Unnecessary
Use of var*). What it caught: mlugg, 24 November 2023, on the thread about the
new error, *identified 3 separate bugs (two in the standard library and one in
the compiler)*, and on error versus warning, *People will treat these "sloppy"
errors just like warnings if they don't block compilation*
(https://ziggit.dev/t/error-local-variable-is-never-mutated/2238/42). Its
limits: the check fires only on a local never used as an lvalue, and a method
call lets a `var` stand even where the method takes no pointer (the originating
commit's message, quoted by the search index from
https://git.jakstys.lt/motiejus/zig/src/commit/baabc6013ea4f44082e69375214e76b5d803c5cb,
**unverified** by me: the page did not render); a `var` slice compiled without
the error on 0.14.1 and with it on 0.15.2, read as a regression
(https://ziggit.dev/t/why-var-slice-that-is-not-mutated-does-not-trigger-compile-error/13249).
The policy's root, ziglang/zig issue 335 *compile errors for unused things*,
opened 19 April 2017 by andrewrk, open and labelled `accepted`, lists *compile
error for unused local variable* and *compile error for unused assignments to
variables* (https://github.com/ziglang/zig/issues/335); **it does not contain**
the sentence *if a variable is declared mutable with var, then the function
must mutate it* that forum threads attribute to it
(https://ziggit.dev/t/zig-and-liveness-of-code/6400), so the never-mutated
rule is 2023's extension of 2017's policy, not its text. A correction to the
search index's reading, made after opening the issue.

**Verified, the others warn.** Rust's `unused_mut` and `unused_variables` are
both on the rustc book's warn-by-default listing
(https://doc.rust-lang.org/rustc/lints/listing/warn-by-default.html). Swift
prints `warning: variable 'f1' was never mutated; consider changing to 'let'
constant` (a Swift Package Index build log,
https://swiftpackageindex.com/builds/51FC1303-F57D-437A-A395-DA5F7EF7DCA7);
its checks *are not comprehensive*
(https://forums.swift.org/t/combine-definite-initialization-and-variable-usage-checks/14030),
with a filed false positive on a lazy property's mutating getter
(https://bugs.swift.org/browse/SR-11066) and a false negative
(https://bugs.swift.org/browse/SR-13821). A project that turned the warning
into an error and reported what it caught, other than Zig's own tree:
**unverified** (not found).

**Verified, the error-not-warning doctrine in Go's words.** Go's FAQ, on
unused variables and imports: *if it's worth complaining about, it's worth
fixing in the code. (Conversely, if it's not worth fixing, it's not worth
mentioning.)*, and *having the compiler generate warnings encourages the
implementation to warn about weak cases that can make compilation noisy,
masking real errors* (https://go.dev/doc/faq, *Can I stop these complaints
about my unused variable/import?*). Go has no mutability marker, so it has no
never-mutated rule; the doctrine is the precedent for B2 being an error.

**For B2.** The brief's probe shows today's compiler accepting `v: i64 @ 0`
with only `print(v)`. Zig's experience says the rule finds real defects in a
tree written by people who already held the discipline, and that its
exceptions (a write through a call, the critic's `@db`) must be enumerated
before it is an error, or it is Swift's incomplete check with Zig's severity.

## Q4. Mandatory type on a mutable, optional on an immutable

**Verified, the asymmetry has shipped three times, for another reason.**
Pascal and Oberon: `CONST N = 100` carries no type, `VAR x: INTEGER` must
(the productions in Q1). Ada: a number declaration `Max : constant := 500;`
has no type and denotes a `universal_integer`, while an object declaration
names its type (Ada 2012 RM 3.3.2,
https://www.adaic.org/resources/add_content/standards/12rm/html/RM-3-3-2.html).
In all three the mutable is typed because it has **no initializer** to infer
from, or the immutable is untyped because it is a compile-time universal; in
none is the type what tells a declaration from an assignment, since `:=`
never declares there. So the Heroes rule's *shape* has a Zurich pedigree and
its *reason* (the type as discriminator) has none that I found.

**Verified, the inverse shipped too.** C#: `const var x = 0;` is CS0822,
*Implicitly typed locals cannot be const*; the fix is an explicit type, while
the mutable `var` local infers
(https://learn.microsoft.com/en-us/dotnet/csharp/misc/cs0822). Java 10, JEP
286 (owner Dan Smith, created 2016-03-08): *There was a substantial diversity
of opinion over a second syntactic form for immutable locals (`val`, `let`)*,
*In the end, we chose to support only `var`* (https://openjdk.org/jeps/286);
whether `final var` is legal: **unverified** from the JEP.

**Verified, the one shipped case where a TYPE on a declaration was the mark of
mutability.** Turbo Pascal's typed constants, `const x: integer = 5`, were
writeable; Embarcadero: *In early versions of Delphi and Object Pascal, typed
constants were always writeable*, controlled since by `{$J+}` /
`{$WRITEABLECONST}`, with `{$J-}` making them truly constant and recommended
for new code, the default listed as `{$J-}`
(https://docwiki.embarcadero.com/RADStudio/en/Writeable_typed_constants_(Delphi)).
A 2000 O'Reilly reference gives `{$J+}` as the default
(https://www.oreilly.com/library/view/delphi-in-a/1565926595/re469.html), so
the default moved, an inference from two sources and not a statement by
either. Free Pascal's `objfpc` mode still defaults to writeable, and a 2009
list thread asks what the point is
(https://lists.freepascal.org/fpc-pascal/2009-June/021604.html). **This is R0's
closest ancestor and it is remembered as a wart**: a reader who saw a type
could not tell a constant from a variable without knowing a compiler switch.
Heroes' version is milder, since the brief's probe shows `x: i64 = 5` legal
today, which means the type is *not* the mark of mutability in Heroes either;
only the `@` is, and the type is the mark of *declaration*. That is a weaker
claim than §4.4's sentence, and a precedent-free one.

**Searched and not found**: a shipped language with an initialized mutable
whose type is REQUIRED while the immutable's is optional, the type being what
distinguishes declaring from mutating. Searched: Pascal, Oberon, Ada, C#,
Java, Nim, Dafny, Go, Rust, Swift, Kotlin, F#, OCaml, Zig, Hylo, Carbon
(whose `var x: auto = 5` proposal p0851 lists eliding the type as an
alternative, https://docs.carbon-lang.dev/proposals/p0851.html), Odin.

## Q5. Compound-assignment habits: an `X=` token that does not mean *apply X then assign*

**Verified, C's own author shipped the reversed form and withdrew it.**
Ritchie, *The Development of the C Language* (HOPL-II, 1993): B took
`x=+y` from Algol 68 via McIlroy, and *in B and early C, the operator was
spelled `=+` instead of `+=`; this mistake, repaired in 1976, was induced by a
seductively easy way of handling the first form in B's lexical analyzer*
(mirror https://www.cnblogs.com/wangshide/archive/2012/07/13/2590791.html;
the primary at https://www.nokia.com/bell-labs/about/dennis-m-ritchie/chist.html
returned 403 and the Tufts PDF did not text-extract, so the quote is the
mirror's). What the reversed form cost readers: in old C `i=-1` decremented
`i` (Chris Torek, comp.lang.c, April 1988,
https://www.tuhs.org/Usenet/comp.lang.c/1988-April/015520.html), and
compilers kept printing *warning: ambiguous assignment: assignment op taken*
into 1989 (https://www.tuhs.org/Usenet/comp.lang.c/1989-March/013393.html).

**Verified, five tokens of the `X=` or `=X` shape with a non-update meaning
and what readers did.**
- Go `:=` declares and initializes (Q1): issue 377, open since 2009.
- GNU make 4.0, 9 October 2013: `!=` is shell assignment, added for BSD
  compatibility; it broke every makefile that wrote `variable!= value`, the
  fix being whitespace, `variable! = value`
  (https://fuchsia.googlesource.com/third_party/make/+/4.0/NEWS).
- Perl `=~` binds a match; Raku *replaced* it with `~~`
  (https://docs.raku.org/language/perl-op); Ruby removed its catch-all
  `Kernel#=~` in 3.2 (Q1).
- Erlang `=<` is less-or-equal, inherited from Prolog because `<=` was an
  arrow there (Richard O'Keefe, erlang-questions, March 2008,
  https://erlang.org/pipermail/erlang-questions/2008-March/033967.html).
- Python `:=`, PEP 572, created 28 February 2018, Status Final, Python 3.8,
  resolution a python-dev message of July 2018; its own reason for a token
  distinct from `=`: *The syntactic similarity between `if (x == y)` and `if
  (x = y)` belies their drastically different semantics*
  (https://peps.python.org/pep-0572/).
- Python `@=`, by contrast, IS apply-then-assign (Q2): the habit confirmed in
  the one language that has the token.

**For R1 and R7.** R7's `@=` as re-bind is the only reading a C- or
Python-trained reader brings; R1's `@=` as birth is a new meaning for a token
whose family has meant *update* since 1976. The hazard is the reader's, not
the writer's: a model does not spontaneously write `v @= v + 1` meaning
matmul, but it may write it meaning *update*, and under R1 that line declares
a second `v` (or is refused, if Heroes refuses redeclaration in the same
block; a question for the compiler-engineer). The blind seat's B arm can count
it.

## Q6. The reverse spelling `=@`, and `=~`'s fate

**Verified, two-character tokens beginning `=`.** `==`; `=>`; `=~` (Perl,
Ruby); `=<` (Erlang, Q5); Erlang's three-character `=:=` and `=/=`; Haskell's
`=<<`; Ruby's line-initial `=begin`/`=end`; and old C's `=+`, `=-`, `=*`,
withdrawn in 1976 (Q5). **No language found with `=@`** (searched the above,
plus make, Raku, k, APL, Dafny, Odin). The precedent for R2 is therefore old
C's `=-`: an `=` followed by a glyph that also has a prefix meaning. In Heroes
`@x` is the argument mark (`bump(@x)`, brief) and `@v` before a name is a
known mistake at `at_prefix`, so `v =@x` against `v = @x` is `i=-1` against
`i = -1`, the shape Ritchie called a mistake. The critic's finding that `=@`
is already a planted mutant for `named-arg-equals` says the same from the
other side.

**Verified, what became of `=~`.** Raku renamed it (`~~`, Q5); Perl 5's `~~`
was deprecated in 5.38, scheduled for removal in 5.42, removed in 5.41.3 and
re-instated (Q1); Ruby's `Object#=~`, deprecated in 2.6 for silently returning
nil on the wrong receiver, was removed in 3.2 (Q1). Readability complaints
about the glyph pair `=~` as such: **unverified** (none found that is not
about smartmatch semantics).

## Verdicts, advisory, per route

- **R0** status quo: **object.** Its one ancestor, a type annotation as the
  mark of mutability, is Turbo Pascal's writeable typed constant, repaired by a
  switch whose default moved. The locality half of §4.4 stands (Go, Erlang and
  Zig all show name-reuse defects are real); the type-as-discriminator half has
  no precedent and, since `x: i64 = 5` is legal today, is not even true of
  Heroes.
- **R1** `@=` declares, `@` re-binds, type optional: **approve, conditional.**
  `name @ ...` as *bind here* has Rust's, Haskell's and Scala's lineage; `@=`
  as *birth* contradicts Python's `@=` and the whole `op=` habit, a reader's
  hazard the blind seat can count. The condition is the Go question above:
  whether an inner `@=` may shadow an outer cell.
- **R1b** R1 with the never-re-bound error: **approve, the strongest route
  historically.** Zig 0.12.0 shipped exactly this error, found three bugs in
  its own tree, and argues the error over the warning in Go's own terms; its
  exceptions (a write through a call or an `@` argument, the critic's
  `@db`) must be enumerated first or it is Swift's partial check at Zig's
  severity.
- **R2** `=@`: **object.** Old C's `=-`, withdrawn 1976; already a planted
  mutant here.
- **R3** `@@`: **object.** Ruby's class-variable sigil; `at_prefix` already
  owns `@@v` as a mistake. No infix precedent.
- **R4** `:=`: **object.** Four shipped readings (Pascal and Oberon assign,
  Go declares, Dafny does both, Python names an expression), Go's sixteen-year
  issue, and §4.4's own refusal.
- **R5** a keyword, `@` re-binds: **approve, the precedent-rich route.** Hylo
  (`var` + `&x = 2`) is this shape glyph for glyph; F# and OCaml, Dafny, Zig,
  Swift, Kotlin, Rust, Nim all chose a word at birth. §4.4 refused `var` only
  in a world where `=` also mutated (brief), which is not this one. Between
  R1b and R5 history prefers R5; the blind seat decides whether the glyph
  argument beats it.
- **R6** type mandatory on both: **object.** Every language above infers a
  local's type from its initializer; the brief measures the cost.
- **R7** `v @ 0` declares, `v @= e` re-binds: **object.** `@=` as update
  matches the habit, but `v @ 0` as a birth has no precedent and is today's
  mutation line in 7553 places of `selfhost/` (brief's table).
- **R8** type from all writes: **object.** Dafny's look-ahead inference is
  the one precedent and its own manual calls it best-effort that may fail; a
  verifier's luxury, against design.md's locality (brief).
- **R9** repair the `v @ 0` message: **approve as a floor** under any route;
  Go's FAQ doctrine, an error worth raising is worth telling well.
- **R10** `v: i64 @= 0`: **approve as the measurement arm**, no shipped
  language requires both a symbol and a type, so it isolates the symbol's
  effect, which is what the blind seat needs.
- **R11** `@v = 0`: **object.** Ruby's sigil; `at_prefix`'s recovery already
  owns it.

## A falsifiable prediction, and its instrument

1. **B2 over `selfhost/` flags more than zero cells.** Zig found three in a
   tree written under the discipline; Heroes' `selfhost/` holds 5926
   declaration lines (brief) and today accepts a never-re-bound cell (probe).
   Instrument: the prototype's `heroes check selfhost/main.hero` with B2's
   rule on, counting its new diagnostic, `test` blocks included via `heroes
   test`. Zero flags falsify my reading of Zig as transferable.
2. **The `op=` habit shows in the blind seat's B arm.** At least one of the
   six B2 sessions writes `v @= <expr containing v>` meaning *update*, the
   Python and C reading of an `X=` token. Instrument: the sessions' outputs
   grepped for `@=` whose right side names the left name. Zero of six
   falsifies the hazard I weigh against R1.
3. **The one-character typo is caught only by B2 and only off the write
   path.** A new operator, *`@=` written where `@` was meant* (the inverse of
   `forget-at-decl`), re-planted over the 839 declarations of
   `plan-singles.jsonl` (brief): under R1 without B2 every mutant compiles;
   under B2 the mutant is caught where the original cell is then never
   re-bound, and compiles where it is (Go's shape). Instrument:
   `docs/metrics/operators.md` and panel 187's R2 run.

## The condition that would change these verdicts

- **R1 and R1b rise to unconditional** on one fact from the tree: Heroes
  refuses an inner block declaring a name an outer block holds. Then Go's
  issue-377 class cannot occur and prediction 3's surviving mutants vanish.
- **R5 overtakes R1b** if prediction 2 holds at a rate above the A arm's
  declaration mistakes: the glyph's prior would be costing more than a
  keyword's tokens.
- **R0 recovers** if a source shows Delphi kept `{$J+}` by default and its
  users wanted it: the one precedent against a type as the mark of
  mutability would be weaker than I read it.
- **R8 recovers** if a compiled, non-verifier language is found inferring a
  cell's type from later writes with local errors; I found none.
