---
kind: defect
area: compiler
milestone: none
filed: 2026-10-07
commit: b1bbe22910ba8bfd4c2bb57d49c2146e03db9767
github: none
---

- [x] **452 — a search that slices a text at every byte survives in eight test helpers and in `emit/structural`'s `mentions_v`** | defect 296 replaced its three helpers with one byte-safe `strings.holds`; the same shape stands in test helpers of `emit/body`, `callback_thunk`, `extern_field`, `perfn`, `record_constant`, `ir/print`, `word_neighbours` and `word_place`, and in one production function, `emit/structural`'s `mentions_v`, safe today only because the text it reads is ASCII (lane b14-cli's reading) | the nine places · defect 296 · **class: improvement**

    **Origin:** filed by the coordinator at 16:44 on 2026-10-07 from lane b14-cli's final report (*found beside* 3).

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a panic on a character above ASCII a test or a future caller may hand them; no program moves today.

    Repaired at `b1bbe229`, 2026-10-08 (lane b15-emit), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Every search of a text in the nine places, and in `perfn`'s and `types`' test scans, asks its bytes (`strings.holds`, `bytes.bytes_at`), and so does `emit/layout_text`'s `place_of` (`bytes.rfind`), the shape beside it with the same cause, which aborted `heroes build` at exit 134 on a correct binding of an anonymous union declared in a header named `uné.h`; that program builds now and prints what it prints under an ASCII name.

    Corrected 2026-10-09: `b1bbe229` took `selfhost/emit/layout_text.hero` to 301 lines of code, past its 300, which `layout` found at the lane's last pass; `37fc7b39` says `place_of`'s comment in three lines, and the file counts 299. The repair is those two commits, and the card names the first.

## The repair

Repaired at `b1bbe229`, 2026-10-08 (lane b15-emit), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Every search of a text in the nine places, and in `perfn`'s and `types`' test scans, asks its bytes (`strings.holds`, `bytes.bytes_at`), and so does `emit/layout_text`'s `place_of` (`bytes.rfind`), the shape beside it with the same cause, which aborted `heroes build` at exit 134 on a correct binding of an anonymous union declared in a header named `uné.h`; that program builds now and prints what it prints under an ASCII name.

**Closed 2026-10-09** with batch 16 (lanes b16-emit, b16-runtime, b16-land199, b15-parse, b16-tools, b16-compiler, b16-land198 and b16-misc, merged into one round tree made from the trunk), its closing gate run on the round at `164e699a`: the seed regenerated over two generations, the runtime's ABI staying at 29, 46,097,621 bytes, SHA-256 beginning `d3bf3451eae3f3e8`, its fixpoint by `cmp`; the compiler's own tests 1,531, all passed; the net's own tests 322, all passed; the full net 7,237 passed over 29 suites, 0 failed, `run` taken one harness process per case and its one red the skip ratio of a one-case run (defect 437's case, which binds `sys/prctl.h`, skipped on this Mac as in every whole run). It is the first batch under the optimistic chain (author instruction 2026-10-09, `.claude/rules/verification.md` § The optimistic chain): the census and panel 187's R2 run after the push beside the CI, no platform leg ran before it, and a defect at the C boundary closes here, a CI leg red on its case filing a new `blocking` defect naming it. A trial of the same gate on the round at `2dbbf7e6`, before lane b16-misc's four repairs, read the full net 7,226 and 0, the census's every move attributed and R2's one finding read as defect 457's rule speaking.
