# Panel 172 — completeness critic

No verdict on the proposal. This seat reads what the sitting produced and names
what is missing from it.

Everything below was run on 2026-09-21 in
`<scratch>/critic-172`, a copy of the tree at `abf9a17e` with `heroes` built
from the seed (`rm -rf build && clang -I runtime seed/heroes.c runtime/runtime.c
-o heroes`, exit 0). Where a command used the compiler-engineer's route-A
prototype it says so and the binary is
`<scratch>/compiler-engineer-172/heroes-next`. Darwin 25.6.0, arm64. No timing
claims: the machine is shared, and `.claude/rules/platforms.md` means every
number here is Darwin's alone.

---

## 1. The empty stderr is not a message being lost or suppressed. On this platform the allocator produces none, for any bad free at all

The brief, the defect and three reports all treat *stderr is empty* as the
allocator's message going missing. It is not missing. It was never written.

    $ cat /tmp/c172/variants.c        # five bad-free shapes, one binary
    ...  which==0 free(b+16)  /* interior */
         which==1 free(b); free(b)    which==2 free(stackbuf)
         which==3 free((void*)0x1234) which==4 free(malloc(100000)+16)
    $ clang -O0 -g variants.c -o variants
    $ for i in 0 1 2 3 4; do ./variants $i 2>err; echo "$? $(wc -c <err)"; done

    case 0: exit=133 stderr=9B : before 0|      (the 9 bytes are my own marker)
    case 1: exit=133 stderr=9B : before 1|
    case 2: exit=134 stderr=9B : before 2|
    case 3: exit=134 stderr=9B : before 3|
    case 4: exit=134 stderr=9B : before 4|

Not one byte of allocator text, in any of the five, including the classic
double free and the free of a stack address. The knobs change nothing:

    $ ./lease070                                       exit=133
    $ MallocNanoZone=0 ./lease070                      exit=133, stderr 0B
    $ MallocScribble=1 ./lease070                      exit=134, stderr 0B
    $ MallocDebugReport=stderr ./variants 0            exit=133, no text
    $ MallocNanoZone=0 MallocDebugReport=stderr ./variants 0   exit=133, no text
    $ MallocNanoZone=0 MallocStackLogging=1 ./lease070 2>&1 | head -5   (empty)

And it is not going to a crash report either: `ls -t
~/Library/Logs/DiagnosticReports/` after these runs shows nothing newer than
2026-09-18.

**What it changes.** *Pointer being freed was not allocated* is a sentence from
an older macOS. On Darwin 25.6.0 there is no message to recover, route back or
re-enable, so every proposal that assumes the platform will eventually say
something is resting on a premise that is false here. The 133/134 split is
explained too: 133 is `SIGTRAP`, which the interior and double-free paths raise,
and 134 is `SIGABRT` from the other three. The instability the defect title
complains about is two different platform paths, not randomness.

---

## 2. THE ROUTE NOBODY LISTED: the runtime can say it, and it closes the FILED reproducer, which no route on the table does

The three routes are all about the vocabulary. The defect is about a program
that dies saying nothing. Those are not the same question, and nobody separated
them.

The runtime already installs signal handlers and already writes from inside one:
`runtime/parts/stack.c:582-596` (`hero_stack_guard_install`, `sigaction` with
`SA_SIGINFO|SA_ONSTACK` for SIGSEGV and SIGBUS, previous disposition saved and
chained) and `runtime/parts/stack.c:218` (`hero_stack_say`, `write(2)` only). It
also already keeps the number that answers the question: `hero_live_held`,
`runtime/parts/alloc.c:98`, the live lease balance.

So I wired the two together. **50 lines, two files, no compiler change, no spec
change, no word:**

    $ git diff --numstat runtime/
    2   0   runtime/parts/alloc.c     # a flag set when the exit sweep runs
    48  0   runtime/parts/os.c        # SIGTRAP/SIGABRT handler + install

    $ rm -rf build && clang -I runtime seed/heroes.c runtime/runtime.c -o heroes
    $ CPATH=docs/panel/172-briefs ./heroes build .../lease070.hero -o p_lease070
    $ for i in $(seq 10); do ./p_lease070 2>e; echo -n "$?/$(wc -c<e) "; done
    133/250 133/250 134/250 133/250 133/250 133/250 133/250 133/250 133/250 133/250

    panic: a C function freed bytes this program still leases — 1 lease(s) were
    live when the allocator refused the free.
      the bytes of a `.lease()` are the program's, freed by `end_lease`,
      and the callee that was handed one frees what it is handed

Defect 070's filed reproducer — `lease070.hero`, the one whose callee carries no
word at all, the one the compiler-engineer measured as **exit 0 before and after
route A** — goes from *zero bytes* to *250 bytes naming the lease*, with the
vocabulary question untouched.

The control is unharmed and does not double-report:

    $ ./p_control     exit=134 stderr=111B
    panic: 1 lease(s) never ended — every `.lease()` owes one `end_lease`, ...

Under the sanitizer the two compose rather than collide:

    $ ./heroes build --sanitize .../lease070.hero -o san_lease070   exit 0
    $ ./san_lease070 2>san.err   exit=134  stderr=1324B  AddressSanitizer lines=2
    $ grep -c "still leases" san.err        1

**The two exceptions I found, and they are measured, not conceded.**

*A panic while a lease is live prints a second, false line.* A Heroes panic
reaches `abort()`, which is SIGABRT, which is the handler:

    function main()
        x = "payload"; c: cstr @ x.lease(); look(s: c)
        a = [1, 2, 3]; print(to_str(a[9]))          # out of range
    $ ./p_panic    exit=134 stderr=282B
    panic: array index out of range
    panic: a C function freed bytes this program still leases — 1 lease(s) ...

No C function freed anything. The prototype's guard covers only the exit sweep;
the panic path needs the same flag. That is a defect in fifty lines written in
one sitting, not evidence about the route, but it is the shape a landing owes a
case for.

*It is silent where no lease is live*, which is correct and also a limit:
`lend070.hero` (a lend, no lease) stays at exit 134 with 0 bytes, and so does
the `@out` shape in §4. This route reports the LEASE class. It does not report
the C-pointer double-free class, and a resolution that adopted it would have to
say so.

**What it changes for the resolution.** Defect 070 as filed is *an empty stderr
and an unstable exit code*. That is a diagnostic defect, and a diagnostic defect
has a diagnostic repair that is measurable today, orthogonal to whether
`consumes` gets a meaning. Whatever the sitting decides about the word, the
question *may the runtime name the lease at the crash* was never asked, and the
answer is yes.

---

## 3. A negative claim asserted and now falsified by running it

The compiler-engineer's report, § *3. The residual, and can the runtime catch
it?*:

> The runtime cannot catch it, and the repository already says so in the code,
> `runtime/parts/str.c:430-444`, panel 125's own comment.

The quoted comment is about a different question. It says a *double release
cannot be told from a never-held pointer by READING the freed block*, because
freed memory owes nobody its contents. That is true and the prototype reads no
block: it reads a counter the runtime keeps in its own static storage, from a
signal handler, after the allocator has already refused. The report then says
*"Control never returns to the runtime, so there is nothing to detect with."*
Control does return to the runtime — through a signal, on the mechanism this
runtime already installs for stack exhaustion.

CLAUDE.md § RUN IT: *"a negative claim rests on the searcher's vocabulary rather
than the world, so 'X cannot be done' goes out as a question naming what was
searched for."* The seat searched for a way to inspect the pointer. It did not
search for a way to speak at the crash, and `stack.c` is the file that already
does.

---

## 4. THE QUESTION THE SITTING SHOULD HAVE ASKED: three shapes beside the adopted one, none of them in the defect's four, all `check` 0 under BOTH compilers, all 133 with zero bytes

I wrote the shapes the brief's own list names. Header `beside.h`, five programs,
run against the shipped compiler and against `heroes-next` (route A).

| shape | file | shipped `check` | route A `check` | run, stock runtime |
|---|---|---|---|---|
| **C invokes a callback with the leased pointer, and the callback frees** | `b_callback.hero` | **0** | **0** | **exit 133, stderr 0B** |
| **C frees the lease on a LATER call than the one it was handed to** | `b_later.hero` | **0** | **0** | **exit 133, stderr 0B** |
| **an `@out: cstr` C fills and then frees, then the program frees it** | `b_out.hero` | **0** | **0** | **exit 133, stderr 0B** |
| a lease into a group record's `cstr` field | `tests/golden/check/ffi-a-lend-parked-in-a-group-record.hero:36` | 1 `lease_escapes` | 1 | — |
| two leases into one consuming call | `b_two.hero` | 1 `unread_mark` | 1 `consumed_by_c` (first lease) | — |
| a consuming parameter of a function nobody calls | `b_never.hero` | 1 `unread_mark` | **0** | — |

The programs, in full:

    # b_later.hero — C frees it on a call that takes no pointer at all
    extern "beside.h"
        function stash(s: cstr)
        function later_free()
    function main()
        x = "payload"
        c: cstr @ x.lease()
        stash(s: c)
        later_free()          # static inline void later_free(void)
        end_lease(@c)         #   { free((void*)(uintptr_t)stashed); }

    # b_callback.hero — the disposer arrives as a VALUE
    extern "beside.h"
        function take_cb(s: cstr, cb: (function(cstr) -> ()))
        function eat(s: cstr)
    function main()
        x = "payload"
        c: cstr @ x.lease()
        take_cb(s: c, cb: eat)
        end_lease(@c)

**`b_later.hero` is the shape that decides the vocabulary question, and no seat
ran it.** `later_free()` has **no pointer parameter**. There is nowhere to write
`consumes`, nowhere to write `lent`, nowhere to write a keeps-word. Route A,
route B and every spelling in the ergonomist's cold test are marks on a
parameter, and this program frees a lease through a function that has none. A
vocabulary on parameters cannot reach this class, and the class dies exactly
like the filed reproducer: exit 133, zero bytes.

**`b_callback.hero` is measurement 037's own R5 construct with a lease where the
`malloc` was.** The ffi-pragmatist's recommended resolution is *hand the disposer
over as an argument* — and handing the disposer over as an argument is one
substitution away from defect 070, invisible to `check` under both compilers.
The seat measured that R5 is correct; nobody measured what R5 looks like when it
is wrong.

**`b_never.hero` is a regression in coverage under route A.** Today the compiler
refuses `function never_called(s: cstr consumes)` at the declaration
(`unread_mark`). Under route A it is admitted at exit 0 and never examined,
because nothing calls it. That is the spec-warden's *legal declaration of a
falsehood* with the call site removed, so the one thing that would have judged
the word is gone.

Both prototype runtimes agree on the run side; and the § 2 prototype names the
lease on the first two and stays silent on the third, which is the class
boundary:

    b_callback  proto: exit=133 stderr=250B   "…1 lease(s) were live…"
    b_later     proto: exit=133 stderr=250B   "…1 lease(s) were live…"
    b_out       proto: exit=133 stderr=0B     (no lease live — the pointer is C's)

**What it changes for the resolution.** Defect 070 has at least **seven** shapes,
not four, and the three new ones are all `check` 0 under the route the sitting is
adopting. Under CL-078 the defect is written as the finder saw it and the shapes
beside it are where what it actually is becomes visible: what it actually is, on
this evidence, is *a lease whose bytes C frees, by any route*, and two of the
three routes have no parameter to mark.

---

## 5. A SECOND ROUTE NOBODY LISTED, priced: the compiler-engineer's approve-condition (1) is route B's corpus cost in full, and it lands on the parameter the ffi-pragmatist vetoed over

The engineer's condition 1 is *route A paired with a lease refused at an unmarked
pointer parameter*, costed as "under 20 further lines in that one file". The
corpus price is not in the report. Measured:

    $ grep -rn '\.lease()' examples tests/golden --include='*.hero'
    (12 non-comment sites in 11 files, the shared brief's count reproduces)

    $ grep -rn "function sqlite3_bind_text\|function keep_label\|function \
      curl_easy_setopt\|function strlen\|function stash_put\|function after_dash" \
      examples tests/golden --include='*.hero'
    examples/ledger/db/sqlite.hero:115: … text: cstr, length: i32, destructor: ptr
    examples/gallery/13-lease.hero:16:  function keep_label(s: cstr)
    examples/curl/main.hero:51:         function curl_easy_setopt(… value: cstr) -> i64
    tests/golden/run/lease-tail-points-into-the-bytes.hero:13/16: strlen(s: cstr),
                                        after_dash(s: cstr, @tail: cstr)
    tests/golden/run/abort-lease-never-ended.hero:9:  function strlen(s: cstr) -> u64
    tests/golden/run/lease-c-keeps-the-pointer.hero:12: function stash_put(s: cstr)
    tests/golden/check/…:                              function strlen(s: cstr) -> u64  ×6

**Not one parameter that a shipped lease reaches carries any word** — no `lent`,
no anything. So condition (1) refuses **all twelve sites**, which is exactly
route B's leasing half and exactly the corpus cost the shared brief prices
against B and the warden calls B's blocker. It is not a cheap addendum to A; it
is B arriving under A's name.

And it lands on the one parameter the ffi-pragmatist's falsifier is written
about: `examples/ledger/db/sqlite.hero:355` leases into `sqlite3_bind_text`'s
`text: cstr`, whose ownership is chosen by the **fifth** argument per call
(`sqlite3.h:4888-4903`, that seat's citation). Under condition (1) that site
takes a word or goes red, and the seat's veto says no word is true of it.

Marked honestly: **I did not build condition (1)**, and the refusal count above
is reasoning from the declarations plus the grep, not a compiler run. UNRUN: the
`corpus` suite under condition (1).

---

## 6. Re-measurements of what the resolution would rest on

**The four shapes, in my own copy, shipped compiler.** All reproduce.

    lease070    check 0  |  10 runs: 133/0 133/0 134/0 133/0 133/0 134/0 133/0 133/0 133/0 133/0
    lend070     check 0  |  10 runs: 134/0 ×10
    lend070b    check 1  error[lend_kept]
    consumes070 check 1  error[unread_mark]
    control     check 0  |  10 runs: 134/111 ×10

**Measurement 037 reproduces exactly**, reconstructed from the record's own
lines 18-39, and route A leaves it alone:

    CPATH=$W ./heroes check r5.hero                       exit 0
    CPATH=$W ./heroes run   r5.hero                       exit 0, prints 72
    ./heroes build --sanitize r5.hero -o r5.bin           exit 0
    ./r5.bin                                              exit 0
    grep -c AddressSanitizer r5.err                       0
    wc -c < r5.err                                        0
    ./heroes-next check r5.hero    (route A)              exit 0

The ffi-pragmatist's *check 0 / run 0 / zero ASan lines* is confirmed by a second
pair of hands.

**The spec baseline reproduces, and the warden's price does not reproduce its
own digest.**

    $ . /Users/joseph/Temp/heroes/heroes-lang/.env      # key length 108, never printed
    $ ./heroes measure spec/heroes-spec.md --refresh
    SPEC_REAL_TOKENS 8216   SPEC_DIGEST "2b1556634e73455a"     # baseline, matches

Applying A1' verbatim — *On a `cstr` or `ptr` parameter it says C frees what it
is handed, so nothing Heroes frees reaches it: no lend and no lease.* — merged
into the `consumes` sentence at `spec/heroes-spec.md:381`, with my own line
wrapping:

    SPEC_REAL_TOKENS 8268   (+52)   SPEC_DIGEST "34270e866110cfde"
    (then `git checkout -- spec/heroes-spec.md`; the copy is clean but for the
     prototype's two runtime files)

The warden predicted **8269 / `7ddda6086951f675`**. The count is one token off,
inside the tolerance the warden's own prediction names (*"differ from 8269 by
more than 3"*). **The digest is different**, and the digest is what
`tests/harness/suite_spec.hero` pins and what goes STALE — my copy printed
`STALE: the recorded count is for 2b1556634e73455a and this file is
34270e866110cfde` on the plain `measure`. So the prediction names the wrong
instrument: it can be satisfied on tokens and fail on the thing that actually
turns the suite red, because the digest is a function of line wrapping and the
token count is not.

---

## 7. A contradiction between seats, and which one is checkable

**The engineer and the ffi-pragmatist disagree about route B's corpus cost, in
the same direction, with opposite signs.**

- Engineer, § 4: under B, `strlen` and `sqlite3_bind_text` "would be rewritten to
  `lent` — which means B does not merely add words, it reveals that two shipped
  sites use a lease where a lend would do … it is on B's side of the ledger, not
  against it."
- ffi-pragmatist, § 4 and the prediction: `sqlite3_bind_text`'s third parameter
  **cannot be worded truthfully** for all three values of its fifth, and the
  binding at `:355` passes `SQLITE_TRANSIENT`.

Both cite the same line. They are compatible only if a word may be written for
the CALL the binding happens to make rather than for the declaration, which is
the thing the ffi seat's whole § 4 says the declaration cannot carry. **The
checkable half is the engineer's**, and its instrument exists: rewrite
`examples/ledger/db/sqlite.hero:115` to `text: cstr lent`, keep `:355`'s
`SQLITE_TRANSIENT`, and run `./heroes run tests/harness/main.hero -- ./heroes
corpus`. If it is green, the engineer is right that this site wanted a lend; and
it is still a *program* fact rather than a *binding* fact, because the same
declaration with `destructor: free` at another call site would then be a lie the
compiler believes. **UNRUN by me** — I did not edit a shipped binding, and the
corpus suite under that edit is the measurement the sitting is missing.

A second, smaller one: the warden writes that route A's reading "keys on
nothing: it is a call-site question", offered as the reason design.md:2667 does
not reach route A. `b_never.hero` in § 4 measures the cost of that being true:
where there is no call site, route A reads nothing and admits the declaration at
exit 0, which is strictly less checking than today's `unread_mark`.

---

## What I could not run, in those words

- **Condition (1) built.** I did not implement *a lease refused at an unmarked
  pointer parameter*; § 5's refusal count is reasoning from declarations.
- **Any suite.** I ran no suite of the net against the § 2 prototype, so its
  effect on `run`, `emission` and `determinism` — the three
  `.claude/rules/verification.md` names for a change that reaches golden
  programs — is **UNRUN**.
- **Timing.** The sitting forbids it and the machine is shared; the prototype's
  cost at process start (two `sigaction` calls) and at crash time is **UNRUN**.
- **Linux and Windows.** Every number here is Darwin 25.6.0 arm64. Whether the
  Linux allocator prints the message this one does not, and whether `SIGTRAP` is
  even the signal there, is **UNRUN** and is the first thing § 2 would owe.
- **Whether the handler can name the CALL as well as the lease.** `stack.c`
  already walks frames and blames a Heroes function (`hero_stack_blame`,
  `:330-357`); I did not try to reuse it, so *the message could name `eat`* is
  **UNRUN** and is a question, not a premise.

---

## The one question I would put to the sitting

Three of defect 070's seven measured shapes free a leased pointer through a
function with **no pointer parameter to mark** — a callback handed as a value, a
later call, an out-cell — and all three are `check` 0 under the route being
adopted, while a fifty-line runtime prototype names the lease in all the ones
where a lease is live: **what is the measured reason the report has to come from
the declaration rather than from the crash, and if it is that a compile error
beats a runtime message, which of those three shapes does any vocabulary on a
parameter reach?**
