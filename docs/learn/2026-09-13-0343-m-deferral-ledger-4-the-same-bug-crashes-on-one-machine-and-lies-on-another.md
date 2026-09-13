- [ ] **M-deferral-ledger 4** | Defect 029 is one line changed in the sqlite example: a database handle passed where a statement handle is expected. On this Mac it segfaults, exit 139. On Linux the same program builds clean and finishes at exit 0, printing `rows: -1`. Say why one machine crashes and the other answers, and which of the two outcomes is the dangerous one for the language's own promise.

    **Where to look:** `docs/work/DEFECTS.md` item 029 and the second platform
    run recorded beneath its reproducer; `examples/sqlite/main.hero:46-48,74`; the
    emitted probe line in `heroes build --emit-c`; and `spec/heroes-spec.md` § 3's
    row for `ptr`, which is where the two handles become one type.

    **Why it matters:** the defect was filed as a crash because a crash is what
    the machine that found it produced. The class is not a crash — it is a wrong
    answer that a crash sometimes masks, and the masking is an accident of where
    each library happens to put its memory. A defect described by its loudest
    platform is a defect whose repair will be aimed at the wrong thing.
