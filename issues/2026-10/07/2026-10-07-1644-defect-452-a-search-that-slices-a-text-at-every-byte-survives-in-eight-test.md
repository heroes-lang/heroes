---
kind: defect
area: compiler
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **452 — a search that slices a text at every byte survives in eight test helpers and in `emit/structural`'s `mentions_v`** | defect 296 replaced its three helpers with one byte-safe `strings.holds`; the same shape stands in test helpers of `emit/body`, `callback_thunk`, `extern_field`, `perfn`, `record_constant`, `ir/print`, `word_neighbours` and `word_place`, and in one production function, `emit/structural`'s `mentions_v`, safe today only because the text it reads is ASCII (lane b14-cli's reading) | the nine places · defect 296 · **class: improvement**

    **Origin:** filed by the coordinator at 16:44 on 2026-10-07 from lane b14-cli's final report (*found beside* 3).

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a panic on a character above ASCII a test or a future caller may hand them; no program moves today.
