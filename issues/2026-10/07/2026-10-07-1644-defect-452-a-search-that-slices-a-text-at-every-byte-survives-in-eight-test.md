---
kind: defect
area: compiler
milestone: none
filed: 2026-10-07
commit: b1bbe22910ba8bfd4c2bb57d49c2146e03db9767
github: none
---

- [ ] **452 — a search that slices a text at every byte survives in eight test helpers and in `emit/structural`'s `mentions_v`** | defect 296 replaced its three helpers with one byte-safe `strings.holds`; the same shape stands in test helpers of `emit/body`, `callback_thunk`, `extern_field`, `perfn`, `record_constant`, `ir/print`, `word_neighbours` and `word_place`, and in one production function, `emit/structural`'s `mentions_v`, safe today only because the text it reads is ASCII (lane b14-cli's reading) | the nine places · defect 296 · **class: improvement**

    **Origin:** filed by the coordinator at 16:44 on 2026-10-07 from lane b14-cli's final report (*found beside* 3).

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a panic on a character above ASCII a test or a future caller may hand them; no program moves today.

    Repaired at `b1bbe229`, 2026-10-08 (lane b15-emit), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Every search of a text in the nine places, and in `perfn`'s and `types`' test scans, asks its bytes (`strings.holds`, `bytes.bytes_at`), and so does `emit/layout_text`'s `place_of` (`bytes.rfind`), the shape beside it with the same cause, which aborted `heroes build` at exit 134 on a correct binding of an anonymous union declared in a header named `uné.h`; that program builds now and prints what it prints under an ASCII name.

    Corrected 2026-10-09: `b1bbe229` took `selfhost/emit/layout_text.hero` to 301 lines of code, past its 300, which `layout` found at the lane's last pass; `37fc7b39` says `place_of`'s comment in three lines, and the file counts 299. The repair is those two commits, and the card names the first.
