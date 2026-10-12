---
kind: decision
area: records
milestone: none
filed: 2026-09-03
commit: 4d16200f150ee6daa45534c88c5adec4415cb727
github: https://github.com/heroes-lang/heroes/issues/842
---

- [x] **panel 105** (2026-09-03, full panel, five seats) — ratify the resolution: a type parameter unbound by the arguments takes the type the CONTEXT asks for (Option A, flat), with every condition of the approving seats and both ruling-independent repairs | `docs/panel/105-a-type-parameter-takes-the-type-the-context-asks-for.md` § Resolution; the defect is `docs/defects/006` | **Default while open: A is being built** — the checker binds a still-unbound type parameter against the type the context asks for (annotation, declared result through `return`/`match`/`if`, a non-generic callee's parameter), refuses a call with no context at the call under the existing `cannot_infer`, refuses a type parameter that appears nowhere in the signature at the declaration, keys instantiations by the bound types, and the verifier refuses any unresolved type before the emitter. Seats: ergonomist A/approve-with-conditions · engineer A/approve · historian A/approve flat · pragmatist A/approve with three blocking engineering conditions and B/approve · **warden A/OBJECT on Principle 0 cost (+32 tokens, 52 of 52 signatures bind from arguments), B/approve**. The author's instruction of the same evening — robust over cheap, *"non transigo"* — is the criterion the synthesis applied; a NO here compels Option B in the warden's B3 wording (+15) with the declaration diagnostic and `value.hero` keeping its four duplicated lines | the language either admits the program a writer naturally reaches for, or refuses it with no outlet: the one decision about generics the spec has never stated

  **RATIFIED 2026-09-03 by the author, the same evening, minutes after the synthesis** (*"Va bene, la ratifico subito"*), after the plain-words answer to *"esce la soluzione più robusta? più sicura?"*: yes on both — the natural program compiles wherever the type is known, no wrong guess can compile into a different program (the ergonomist's parametricity argument), and the verifier refuses any unresolved type before the emitter. The verdict is written into `docs/panel/105` § Author's verdict; the work lands in the same session, in its complete form under every approving seat's condition.

## Prior art

Panel 105 (`docs/panel/105-a-type-parameter-takes-the-type-the-context-asks-for.md`): each line says what the cited page itself states, the sitting's own file says what was taken from it, and every link was re-read 2026-10-11.

- **Taken from Swift**: [swiftlang/swift#48363](https://github.com/swiftlang/swift/issues/48363): 'generic parameter T is not used in function signature' is an error at the declaration
- **Taken from Go**: [golang/go#50285](https://github.com/golang/go/issues/50285): inference is not easily localised to a single flat assignment or call once it nests, as in f(f(f(0))), and the proposal for it sits on hold
- **Taken from Java**: [JEP 101, Generalized Target-Type Inference](https://openjdk.org/jeps/101): the result of a generic method may be inferred from the right-hand side of an assignment, and from the formal parameter type in a method call
