# 056 — M-marked-acquisition: the mark goes where the obligation is created

## Goal

Land the form panel 147 admitted and the author ratified: a C handle's release
obligation is marked **where it is created**, on the acquiring call, never on
the type, and **the compiler never picks the release call**. The milestone
opened with two things it was forbidden to inherit — the census, and the price
of the two instruments the sitting refused to choose between.

## What surprised

**The census had two levels, and the first count had flattened them.** Seven
acquire sites and seven release sites at the `extern` level; at the wrapper
level, never counted before, `opened` with 7 call sites, `prepared` with 6,
`closed` with 9, `finalized` with 6. The acquiring `extern` call sits one module
away from every place a handle is held, because §4.19 refuses an `extern` call
across a module boundary — and that single structural fact went on to decide
both sittings and the instrument question at the end.

**The two instruments were never alternatives.** Escape refusal would refuse
**2 of 2** of the reference binding's handle-producing wrappers. The warden had
reached the same wall from the other side at panel 147 — 22 of 23 leaking paths
acquire inside those two wrappers — so the two measurements meet: *what an
escape rule forbids is exactly where the corpus acquires*. The ladder had one
rung and the other was a clause of it.

**The cheaper option was the stronger one, and nobody could see it.** Panel 148
priced a bare `acquires` and an inferred obligation. The historian proposed a
third — the mark NAMES its releaser, GCC's `malloc(deallocator)` shape, and the
shape this repository already ships one line away as `owned sqlite3_free` — and
could not measure it, having no shell. Measured after four of five reports were
in: **+66 against the bare word's +70 once its two repairs are applied.** The
most robust resolution and the cheapest text were the same text.

**A rule can be killed by opening headers.** The inferred option died on a count
nobody had taken: **twelve of nineteen** real headers declare both a releaser
for a type and a function handing back a borrowed instance of that same type.
`SSL_get0_peer_certificate` and `SSL_get1_peer_certificate` are adjacent lines,
identical to clang, opposite in ownership — and a probe taking both through one
function type compiles with zero warnings under `-Weverything`. Then the seat
stopped arguing and reproduced the corruption twice against real libraries.

**And a hinge cost can dissolve.** The compiler seat priced a group-keyed rule
at ~90 lines over a key the language does not define, and every line of that
reproduced — `parse/group.hero` says nothing after the parser knows the word
*group*. The critic found the compiler had already solved it:
`check/decls.hero`'s `one_tag_one_type` is **17 lines**, program-wide, and its
own comment is the answer to the hinge, written before the sitting convened.
**One tag is one handle type across the whole program.** The sentence about to
be carried as the inferred option's cost would have killed the two better
options with it.

## What broke and why

**CL-036 fired three times in one day, on one form.** The formatter dropped the
mark off a parameter in the pricing prototype. An hour later it dropped it off a
**result**, in the real tree, because the first pass had taught one position and
not the other. Each time: the word gone, exit 0, no diagnostic.

**And the third was not this milestone's.** The AST dump had been dropping
`consumes` since **the day that mark shipped** — `heroes parse --dump-ast`
printed `function curl_easy_cleanup(handle: Curl) -> ()` for a program that says
it. The comment one line above the hole had already described the damage, about
a different word: *"this dump is what `heroes fmt` compares its own output
against, so a form missing HERE makes the formatter's self-check agree with a
formatter that is deleting it."* The guard and the guarded had the same blind
spot. Nothing caught it because the dump's own fixture carried neither mark, so
the case that would have failed did not exist.

**A correct program was killed by a message saying the opposite of what
happened.** `examples/curl/main.hero` has carried `consumes` since panel 145
with no `acquires` to pair it, so the first counter went to -1 and announced *1
C handle(s) never given back*. Positive accuses the program; negative accuses
the binding. One counter, two sentences, and the second exists because the first
version was wrong out loud.

**And the synthesis adopted half an answer.** R5 takes the axis *a producer of a
consumed type must say which it is* — but what step 4 landed was `acquires`
alone, so a borrowing producer had no legal spelling and the rule would have
refused a correct binding. That is the ergonomist's own veto reasoning against
the inferred option, carried back in by the coordinator. Found by trying to
build the rule, not by reading the sitting back.

## What landed, and what carried forward

**Three words, each saying one thing.** `acquires <releaser>` — this call hands
a handle over, and that function takes it back. `consumes` — this call takes one
back. `borrows` — this call hands back one it keeps. None inferred, none the
compiler's guess.

**Two instruments, and they answer different questions.** The runtime counter
answers *did the program give back what it took*, at exit, for every library,
in both directions. `check/acquiring.hero`'s 126 lines answer *is this binding
finished*, at compile time, keyed on the type. Neither needs flow analysis,
which the checker states twice that it does not have.

**R6 decided and not deferred**: a missed release is a loud exit, because the
mark is extern-only while the corpus acquires a module away, and a compile error
would need the obligation to cross an ordinary signature that carries no mark of
any kind. The return condition is a program that leaks and **continues** — none
exists today.

**The spec grew +125 vendored and +168 real**, in two payments, each inside the
1.20–1.45 band panel 148's warden gave while refusing any single factor. The
ceiling moved to **10240** by author decision, and it is 10 × 1024 because the
site derives the number and says it in K.

**Carried forward**: the highlighter class, moved to `M-vscode-extension` rather
than ticked here, because closing it here would have been ticking somebody
else's work.
