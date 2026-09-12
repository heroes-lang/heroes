- [x] Owed a panel — the path pair | **CLOSED 2026-08-15 by `docs/panel/057` — refused, three vetoes, and the
  proposal that lost was the one written in this file.** `path_join` fails §1.0 on
  its own concession; `path_parent`'s §1.0 claim was `directory_text` and **it does
  not compute it**, measured twice. Nothing lands: the port writes the scan once in
  `modules/` at 0 spec tokens, which was option (c) all along. The `\`-as-separator
  clause is refused outright with a falsifier that already fired. The Tier-2 route
  is measured at zero compiler lines and left available. **The ratification item is
  below.**

**This is the one item of the eight `/decide` took on 2026-08-15 that `/decide`
could not close, and the reason is jurisdictional rather than a hesitation.** Two
new built-in names plus spec tokens is surface, so CLAUDE.md §4 makes it a `/panel`
path, and the soundness lane does not fit because the change costs spec tokens. The
skill's own rule is to say so and stop. **The proposal is already instructed, so the
sitting can be convened whenever there is time to wait for it:**

> **Adopt `path_join(dir: str, name: str) -> str` and `path_parent(p: str) -> str?`,
> neither of which normalises.**
>
> - **`path_parent` is the one with the §1.0 argument.** The port's module loader
>   must take a path the author typed and find its directory part;
>   `modules/mod.rs`'s `directory_text` does `path.rfind(['/', '\\'])`, looking for
>   **either** separator because the author may have typed a Windows one. In Heroes
>   that is a backwards loop over a string looking for two characters — the shape a
>   built-in exists to remove.
> - **`path_join` has no §1.0 argument and comes along anyway.** Verified: no corpus
>   program builds a path, and `dir + "/" + name` is already correct on all three
>   platforms, so it buys **discoverability, not correctness**. It enters as the
>   inverse of a function that does have the argument, or not at all.
> - **`path_parent` returns `str?`.** Absent when there is no separator. Returning
>   `"."` would be inventing a meaning; `str?` makes the caller handle it and is the
>   language's own idiom (§4.6, absence as a variant).
> - **`path_join` inserts one `/` unless `dir` already ends in `/` or `\`** — a fact
>   about the value in hand (§11), not a decision about a path's meaning.
> - **Neither normalises, and that is the load-bearing half.** The runtime is
>   portable today *because it does nothing to a path*: `runtime/parts/os.c` has no
>   `strcat`, no separator literal, no normalisation, and the string reaches `fopen`
>   verbatim. A `path_join` that rewrote `\` to `/` or collapsed `..` would be the
>   first place this project decides what a path **means**, and that is a larger
>   step than the name suggests — it is also the step that has produced security
>   defects in every language that took it eagerly.
>
> **What the sitting must price**: the spec tokens for two names against §1.6's
> headroom (888 today), and whether `path_parent` alone is the honest §1.0 purchase
> with `path_join` refused under Principle 0's *neither → it waits*.

Was | author instruction, 2026-08-14 | **A runtime function that joins two path pieces — and the honest question is whether joining is the half that is missing.** Verified before writing this: **no program in the corpus builds a path today**, `runtime/parts/os.c` does nothing to one (no `strcat`, no separator literal, no normalisation — the string reaches `fopen` verbatim), and `/` works on all three platforms because Windows accepts it in every filesystem API. `examples/adventure/main.hero` proves it on the Windows leg with a literal `examples/adventure/walkthrough.txt`. So a program that wants to join **can** today: `dir + "/" + name` is correct everywhere, and a doubled separator from a trailing `/` is harmless on all three. **Which means `path_join` buys discoverability, not correctness — and that is not a Principle 0 argument.** What *is* one is the other half. The port's module loader must take a path the **author typed** on the command line and find its directory part: `crates/heroes/src/modules/mod.rs`'s `directory_text` does `path.rfind(['/', '\\'])`, looking for **either** separator, because the author may have typed a Windows one. In Heroes today that is writable — `chars`, `slice`, `len` — and it is a loop over the string backwards looking for two characters, which is the shape a built-in exists to remove. **So the three options are not join-or-nothing**: (a) `path_join(dir, name)` alone, which is the asked-for thing and the half the port does not need; (b) **the pair** — `path_join` and `path_parent` — where the second is the one with the §1.0 argument and the first comes along because a program that can take a path apart will want to put one together; (c) neither, and the port writes the backwards loop once, in the file that already knows why (`modules/`), with `directory_text`'s own comment carried over. **A fourth thing to decide with it**: whether either function normalises. The runtime's portability today is that it does **nothing**, written down as a claim that can die — a `path_join` that rewrote `\` to `/`, or collapsed `..`, would be the first place this project makes a decision about a path's *meaning*, and that is a larger step than the name suggests | runtime/parts/os.c · crates/heroes/src/modules/mod.rs (`directory_text`) · crates/heroes/src/library/source.hero · spec § Strings, arrays, maps | the runtime is portable because it does nothing to a path, and the first function that does something is where that stops being true
