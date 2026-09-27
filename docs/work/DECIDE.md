# DECIDE — the decisions the compiler is waiting on

Read by **`/decide`**. Every item asks **what should be true**, and until it is
answered the compiler goes on behaving some way by default — so the item names
that default, because it is the cost of leaving the item open.

**Only open items live here.** The moment one is answered it is ticked with the
verdict written into it and moved to `docs/records/done/`, the record. Rank by
what an item blocks, never by age, and verify it against the repository before
putting it to the author: asking a settled question is the one cost this list
cannot pay.

**The shape.** One line per item, and an optional body indented four spaces
under it. The first field is the item's **origin**, and where that origin is a
sitting it is spelled `panel NNN`, padded — because
`tests/harness/suite_records.hero`'s `queued` check reads the `- [ ] ` lines
alone and scans them for exactly that, so a citation that slides into the body
makes every pending sitting report as unqueued, and it fails silently. Nothing
lives outside the two banners: `records/lists` is the executor of that.

Format: `- [ ] **<origin>** | <the question, in one line> | <where to look>`

*******************************************************************************
**OPEN: 5**

- [ ] **panel 175** | ratify panel 175: a consuming call is three things, so route A lands only with a vocabulary 176 decides; route E lands now as the composite; § 13 lands its true sentences in full; the blind seat gets a copy of the spec outside the tree | `docs/panel/175-a-consuming-call-is-three-things-and-the-runtime-speaks-after-it-listens.md`

    **Origin:** panel 175, 2026-09-23, M-agreed-retention. **The default the
    compiler runs on meanwhile** is the provisional resolution: items 3 to 6
    land, defect 075 and 079 stay open until panel 176, and `/panel` keeps
    reading the spec from inside the tree until the author says otherwise, since
    CLAUDE.md § 4 leaves the skills to the author's instruction. **What
    conservative would have been**: route A now, keyed on one name, at the price
    of three correct transfer programs over real libraries and a correct
    `sqlite3_close_v2`.

- [ ] **panel 176** | ratify panel 176: the releaser mark takes a set; a transfer is its own word and names the releaser it hands the life towards, checked against the set; a reference is its own mark on a result or a parameter and joins the life it finds; the call-site rule does not land; two declarations of one C function agree at one type; `sqlite3_bind_text` gets no new form; `owned` on a const cell is refused | `docs/panel/176-a-transfer-names-where-the-life-goes-and-a-reference-joins-the-life-it-finds.md`

    **Origin:** panel 176, 2026-09-23, M-agreed-retention. **The default the
    compiler runs on meanwhile** is the provisional resolution: nothing of it
    has landed, so defects 075, 078 and 079 and the milestone's three items
    stay open until the landing, and the spellings of items 2 to 4 go to a
    reader in panel 177 before they do. **What conservative would have been**:
    the prototype as built, with an unchecked `transfers` and the call-site
    rule, refused because the critic measured it silencing defect 075 on three
    platforms and refusing a correct module checked alone.

- [ ] **panel 177** | ratify panel 177: a dead handle is poisoned where it lay (P) and its address remembered (T), with every crossing into C checked; the compile-time half is route M on every path, inside one function; R and S are refused on the ffi seat's vetoes; the success clause is on the result and governs every end; a transfer needs a receiver; and, decided at the landing with no sitting's ruling, a transfer that was made ends the program's own name for the value | `docs/panel/177-a-dead-handle-is-poisoned-where-it-lay-and-remembered-where-it-was.md`

    **Origin:** panel 177, 2026-09-24, M-agreed-retention. **The default the
    compiler runs on meanwhile**, since the landing of 2026-09-25 (step 14),
    is the provisional resolution itself: the poison, the dead set and route
    M (must), defects 077 and 088 closed by it. **What conservative would have
    been**: P alone, refused because it misses every copy and, with callback
    results unchecked, hides a use-after-free ASan catches today.

    **One more decision to ratify, found at the landing (2026-09-25).** Panel
    176's historian named *using the child after the transfer* as Risk 2, "a
    question for the spec seats", and neither synthesis ruled it. The landing
    decides it: a transfer that was made ends the program's own name for the
    value, as a release does (the checker reads a transfer position as an end,
    the runtime poisons the binding, and § 13 now says so), so json-c's
    README order, add the child and then fill it, is refused and is written
    fill first. **What the other reading would have been**: the name stays a
    borrowed reference while the receiver lives, which the runtime cannot see
    end, since C frees the child inside the receiver's release; refused as
    the silent use-after-free it would allow.

- [ ] **panel 179** | ratify panel 179: the formatter's probe is a subcommand, `heroes probe [path]`, in process, over every family the seats ran, with two judges and the three open cases ruled (a comma crossing kept, a comment on a dropped parenthesis kept, the ascending-run rule the canonical form), a site and a reduced reproducer, exit 2 for a failing variant as `fmt` gives it, the fixtures in the net under every family and the whole tree under the thin one | `docs/panel/179-the-formatter-s-probe-is-a-verb-and-its-oracle-is-ruled-before-it-lands.md`

    **Origin:** panel 179, 2026-09-27, M-agreed-retention, a retro-record of the
    author's decision of 2026-09-26 that the probe enters the tool. **The
    default the compiler runs on meanwhile** is the provisional resolution: lane
    g repairs the 17 refusals the sitting found at `83ac68c1` first, then the
    probe lands in its own lane. **What conservative would have been**: `fmt
    --probe` on one file, the guard as the only judge, the first failure's text,
    the fixtures only; refused because a flag cannot take a directory and a
    guard that judges itself cannot see a wrong rule.

- [ ] **panel 180** | ratify panel 180: inside brackets a line breaks by how it ends and a NEWLINE may stand before every closer and every `,` (the compiler made uniform, route b); a list refuses a line that begins with a `-` set apart from its operand where a NEWLINE separates without a `,` (route ii, the silent split of defect 106); a line end the next token cannot continue is refused with the reason and a fix (route f); the spec's sentences on brackets and on strings made true, design.md §4.15 given the ruling, and the 117 shapes kept as `surface` rows | `docs/panel/180-a-line-inside-brackets-breaks-by-how-it-ends-and-a-list-refuses-a-subtraction-it-would-split.md`

    **Origin:** panel 180, 2026-09-27, M-agreed-retention, convened on defect
    104. **The default the compiler runs on meanwhile** is the provisional
    resolution, landed in one lane with defects 104 and 106. **What
    conservative would have been**: route (a), the spec listing today's seven
    exceptions and the compiler unmoved, the silent split left standing;
    refused because it writes accidents into the spec and keeps an exit-0
    wrong answer.

*******************************************************************************
