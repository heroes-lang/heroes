- [ ] **M-deferral-ledger 9** | Read spec § 10's sentence *"`xs @ xs.push(4)` grows in place while nothing else holds `xs`"*, then write a program that satisfies that condition exactly and is still quadratic. Say what the reader who checked the condition, passed it and shipped the program is supposed to do next.

    **Where to look:** `spec/heroes-spec.md` § 10; the llm-ergonomist's hesitation
    table in
    `docs/panel/144-the-cost-is-in-the-spelling-and-nobody-had-written-the-cheap-one.md`,
    hesitation 5, written by a seat that had read nothing but the specification;
    and the spec-warden's finding 2 in the same sitting, reached from the
    measurements instead.

    **Why it matters:** a record field satisfies the condition — nothing else
    holds `w.parts` — and it is 500× slower. So the sentence is **true and
    insufficient**, and that is worse than silence: silence sends the reader to
    measure, a passing condition sends them home. Two seats with **disjoint
    inputs** arrived at the same sentence, one from prose and one from numbers,
    which is the strongest signal this panel format can produce. The lesson to
    carry is the shape of the defect rather than this instance of it: when a rule
    states a condition, ask what happens when the condition **fails** — § 10 never
    says — and ask whether the reader can evaluate it at all from the line in
    front of them, which here they cannot, because *holds* is a whole-program
    fact in a language that tells them on page three that no aliasing exists.
