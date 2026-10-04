# Panel 191 sat: on Windows a name reaches the runtime through a UTF-8 code page its own object carries

2026-10-04, written at 17:03 by the clock (`date`). Panel 191 sat in the
soundness lane on defect 238: on Windows the runtime reaches file names,
arguments and the environment through the narrow API. It was convened by the
author's *6b* of the same day
(`docs/records/log/2026-10-04-1236-the-author-answers-1a-2a-3a-4a-5a-6b.md`).

**The seats**: the compiler-engineer and the ffi-pragmatist, with the
completeness critic over the briefs and over the reports, and no paid run.
Both seats ran on the Windows box, which stopped uncleanly four times that
day, each time from outside the guest by its own System log.

**The synthesis**:
`docs/panel/191-on-windows-a-name-reaches-the-runtime-through-a-utf-8-code-page-its-own-object-carries-and-a-program-refuses-to-start-without-it.md`.

**Its resolution, `provisional — author ratification pending`, R1 to R9:**
- **The route**: `c-dirwide`, which both seats approve. It has three parts:
  - the UTF-8 code-page manifest compiled into the runtime's own object, a
    carrier nobody had listed;
  - a refusal to start, one sentence and exit 2, where `GetACP()` is not
    65001;
  - the directory door made wide.

  It is about 150 lines of `runtime/` and none of `selfhost/`, and it leaves
  the seed and every link line unchanged.
- **Why not less**:
  - the manifest alone truncates a walk at exit 0 and replaces a link by a
    file;
  - wide doors alone leave a bound library's own `fopen` reading the
    program's path as 1252 (W21: -1).
- **Owed at the landing**:
  - the directory named with a lone surrogate, red under the route too (the
    critic's measurement);
  - the cases, each red on `7f4c0cc5`'s runtime first;
  - 239's two cases rewritten, since they made their names through narrow C
    and passed at the base while testing 238.
- **Refused**: (a) alone in any carrier, (a′) (`LNK1158` under `link.exe`),
  (e), (f) and (g).
- **The floor goes to the author**: Windows 10 version 1903, read from
  Microsoft's documentation at 16:58 and never run below. The conservative
  alternative, route (b) whole, is recorded unbuilt.
- **Handed to panel 192**:
  - Q-c, *`args()` ... one that is not UTF-8 aborts*, false on Windows for a
    lone surrogate;
  - Q-i, an interior NUL in a path lent as `cstr`.

Queued as the DECIDE item `panel 191`.

**Corrected underneath**: the closed records of 239 and 243. On Windows
their *not UTF-8* messages are false for a valid name or value that the
narrow door misread.

**Filed from beside the sitting**:
- **336**: `heroes doctor` fails its `cc` row on Windows on a working
  toolchain, `blocking`.
- **337**: a clang or linker failing in words that are not UTF-8 has them
  dropped whole. Run by the coordinator on this Mac at 16:55 with a stand-in
  `clang`. Filed `adjacent`, with the demotion of an author's extern error it
  could cause left as a question.

Three more checks concern code that exists only after the landing, so they
are carried in 238's item rather than filed:
- a program built before the landing launching one built after;
- the OEM code page under the manifest;
- the resource object on other Windows targets.
