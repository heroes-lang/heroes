---
kind: decision
area: records
milestone: none
filed: 2026-10-04
commit: none
github: none
---

- [ ] **panel 191** | ratify, amend or overturn R1 to R9 (route `c-dirwide` for defect 238: the UTF-8 code-page manifest compiled into the runtime's own object, a refusal to start where `GetACP()` is not 65001, the directory door made wide; the lone-surrogate directory repaired at the landing; the cases red on `7f4c0cc5` first; (a) alone, (a′), (e), (f) and (g) refused; Q-c and Q-i handed to panel 192), and set the floor: the oldest Windows a Heroes program starts on | `docs/panel/191-on-windows-a-name-reaches-the-runtime-through-a-utf-8-code-page-its-own-object-carries-and-a-program-refuses-to-start-without-it.md` § The resolution

    **Origin:** panel 191's synthesis, 2026-10-04 from 16:47, on the seats' archive of `7f4c0cc5`, convened by the author's *6b*. Until it is answered defect 238 stays open, and, `blocking`, it lands in the next batch on the provisional resolution (CLAUDE.md § 4, *the panel never blocks*).

    **Recommendation: ratify R1 to R9, the floor at Windows 10 version 1903**, the first to honour the manifest's code page by Microsoft's *Use UTF-8 code pages in Windows apps* (dated 2025-07-17, read 2026-10-04 at 16:58), which also says such a program may still *target/run on earlier Windows builds* on the legacy code page, so below the floor (d) speaks rather than the loader refusing, by the documentation. On what was run on the box:
    - every row of 238 and panel 189's four omitted rows red at the base and green through the route, built by both seats, under `lld-link` and `link.exe` alike;
    - the manifest alone measured unsound (a walk truncated at exit 0, a link replaced by a file), closed by the wide directory door;
    - W21: a bound library's own `fopen` reads the program's path right only under a process code page, -1 at the base, 6 under the route;
    - nothing moves on this Mac or Linux arm64, the seed's fixpoint byte-identical, no `selfhost/` line but cases.

    **What the floor costs**: below it every Heroes program, the compiler included, refuses to start with one sentence and exit 2. Unrun below it: nothing here is older than build 26100, the box and the CI alike, so the documentation's *run on earlier builds* is read, not measured.

    **The conservative alternative, yours to choose instead**: route (b) whole, every door wide and no manifest, about 250 lines, unbuilt by either seat. It sets no floor, and leaves every bound library reading the program's UTF-8 as 1252 (W21: -1), so §1.11's boundary stays broken for every binding that takes a path.
