- [ ] **M-reflection-verdict golden ratification** | Five goldens landed with this milestone and they are marked `# UNVERIFIED — pending debrief`. Read the four ways of getting `::` wrong and say whether each message is the one you would have wanted.

    **Where to look:**
    `tests/golden/check/a-field-name-written-four-ways-and-three-are-wrong.hero`,
    its `.expected`, and
    `tests/golden/check/the-old-operator-on-a-record-name-has-one-repair.fixed`.

    **Why it matters:** three of the four are new diagnostics and the fourth is an
    old one that used to answer the wrong question. `Room.name`, with the operator
    the language has had all along, used to say *a record is built by calling it
    with every field* — true, and not the mistake somebody reaching for a field's
    name has made. The judgement worth making is whether the new message earns its
    place, and whether the one fix tagged `certain` really is: the `.fixed` file
    is what the compiler produces when asked to apply it, and it checks clean.
