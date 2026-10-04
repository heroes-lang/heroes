- [x] **panel 125** | ratify the lease's soundness: the cell is the authority and it is nulled on release, a lease's name stands only as an argument of a call and takes no other write, `end_lease` takes only a lease cell, the runtime reads no memory it was not handed in any accepted program, the registry is refused on Part 7.13, and the spelling is `lease`/`end_lease` | `docs/panel/125-the-guard-that-read-freed-memory.md` § Author's verdict | **RATIFIED AS ADOPTED 2026-09-09** (`/decide`, `1a`): the conservative registry was on the table and declined. The fourth point, raised by the site panel the same evening, went with it (`2a`): `spec/heroes-spec.md:246` said a forgotten lease *is named at exit* where the runtime prints a COUNT when `main` returns and nothing on `exit()`; the spec says *counted when `main` returns* now, at **+4** real tokens (5369 to 5373 on `claude-opus-5`, digest `5a9b288604a587fa`), because naming the lease would take the registry the same verdict refuses

    **Origin:** the soundness lane of 2026-09-09, two seats, convened by the
    coordinator on its own runtime guard after measuring that a double release
    was caught by luck: `malloc` had left a zeroed magic word readable in a
    freed block. Three things were the author's to overturn, (i) the registry,
    (ii) the spelling, (iii) the exit path named rather than closed, and a
    fourth (iv) joined from the site panel, the spec's word for (iii). All four
    were verified against the repository before being asked: the runs behind
    (iii) and (iv) are exit 134 with the count on return from `main` and exit 0
    in silence through `exit()`. The tag question that travelled with the batch
    (`m-cstr-lifetime`) was answered `3a`, no tag, and reversed by the author
    minutes later: the tag stands on the commit that carries this verdict, the
    first commit where every list that milestone left open is clean, so
    `records/tagged` reads a clean tree, and the tag's own message says why it
    is not on the close.
