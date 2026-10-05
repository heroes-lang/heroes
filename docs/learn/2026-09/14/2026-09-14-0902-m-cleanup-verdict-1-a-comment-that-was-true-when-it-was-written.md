- [ ] **M-cleanup-verdict 1** | Read the arm that caused defect 032, as it stood: `if fields.len() == 0` emitting `return true;` under the comment *record E is error[empty_record] in the checker, so this is belt to those braces*. **Every word of that comment was true the day it was written.** Say which word stopped being true, name the commit that did it, and then say why no reviewer reading the file that day would have seen anything wrong.

    **Where to look:**
    `git show e0f7b84f -- selfhost/emit/structural.hero` for the arm as it
    stood, and the same file now; `selfhost/handles.hero`'s module doc for what
    M-handle-verdict made legal; `.claude/rules/module-shape.md` § *A narrowing
    asks the value, never the world*; `docs/records/contract/case-law.md` CL-004.

    **Why it matters:** the arm rested on *a fieldless record cannot be
    written*. That is a claim about the **world around the code**, not about the
    value in hand, and M-handle-verdict falsified it by making a fieldless group
    `record` with a `tag` legal. Here is the part that makes this class nasty:
    when the premise died, **the argument stayed valid**. *A fieldless record is
    impossible, therefore this branch is unreachable, therefore its answer does
    not matter* reads as sound prose forever. Only the first clause went false,
    and nothing in the file says so.

    **Then look fifteen lines up.** The `partial` arm in the same function faces
    the same *this cannot happen* and answers `hero_panic`. One arm put its
    fallback in the loud direction and the other in the quiet one, in one
    function, written by the same hand. Ask: **what would the quiet arm have
    cost if it had been loud instead — and what did it cost by not being?**
