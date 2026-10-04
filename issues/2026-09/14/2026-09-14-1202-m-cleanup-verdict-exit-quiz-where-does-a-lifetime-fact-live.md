- [ ] **M-cleanup-verdict exit quiz** | This language has placed a C lifetime fact four times. For each, say **where the mark sits** — on a type, a result, a declaration, a parameter position, or an expression position — without looking. Then check. Then say what panel 147 refused and why the answer was already implied by your four.

    ```
    (a)  function strdup(s: cstr) -> cstr owned free
    (b)  c: cstr @ text.lease()          ... end_lease(@c)
    (c)  function curl_easy_cleanup(handle: Curl consumes)
    (d)  n(s: text.cstr())
    ```

    **Where to look:** `spec/heroes-spec.md` § 13, which states all four; then
    `docs/panel/147-the-obligation-is-created-by-a-call-and-not-by-a-type.md`
    § *The question the sitting should have asked, and did not*.

    **Why it matters:** panel 147 spent five judges and a completeness critic
    reaching a refusal that `.claude/rules/module-shape.md` settles in one line
    — *a narrowing asks the value, never the world*. The refused form,
    `record Stmt tag sqlite3_stmt released sqlite3_finalize`, would have been
    the **first** lifetime fact in this language attached to a TYPE. Four
    precedents said where such a fact goes and nobody read them as a series
    until the sitting was over.

    **The second half, and it is the one that bites.** Say, from the four above,
    **why** a type cannot carry it. The shape of the answer: some C function
    hands you a `sqlite3_stmt *` you must finalise, and another hands you the
    same type **borrowed** — `sqlite3_next_stmt`, which this very milestone's
    census used to measure the leak. A mark on the type cannot tell those apart,
    so it closes something you were lent. Compiled at the sitting: a program
    reading three rows reports **nought**, at exit 0, with the sanitizer silent.

    **The question to carry away:** a leak is loud when you look for it and a
    wrong answer at exit 0 is never loud. When a repair converts one into the
    other, what is the rule that tells you it is not a repair? (It is in
    `CLAUDE.md § Precedence`, and it is rank 3.)
