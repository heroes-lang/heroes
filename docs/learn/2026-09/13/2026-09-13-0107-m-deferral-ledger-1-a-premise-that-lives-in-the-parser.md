- [ ] **M-deferral-ledger 1** | `selfhost/check/table.hero:66-67` says `T??` *"is rejected by the parser, so this node's argument is never itself a fallible"*. Say which of `.claude/rules/module-shape.md`'s two kinds of premise that is — a fact about the value or a premise about the world — and name the one two-line program, legal under the sentence panel 135 drafted for spec § 4, that would make it false without changing a single line of the parser.

    **Where to look:** `selfhost/parse/type.hero:31-66` (`parse_type`, the
    `nested_fallible` refusal at the `?` loop); `selfhost/check/table.hero:60-70`;
    `.claude/rules/module-shape.md` § A narrowing asks the value, never the world;
    the engineer's robustness finding in
    `docs/panel/135-the-form-was-cheap-and-the-reasons-under-it-were-borrowed.md`.

    **Why it matters:** the comment is true today and will go on reading as true
    the day it is false, because the argument stays valid and only the premise
    dies — the parser still refuses `T??` where it is WRITTEN, and an alias is a way
    to write a type without writing it. That is CL-004's shape in the checker, and
    the sitting's answer was not to weaken the comment but to owe the invariant a
    second home in the checker, with a test that fires, on the day a name can stand
    for a fallible type.
