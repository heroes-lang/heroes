# Panel 157 — ffi-pragmatist brief

Read `docs/panel/157-briefs/00-shared.md` first. **This is the SOUNDNESS LANE**:
you and the compiler-engineer only, because `--emit-c` is a tool surface with no
spec token and no diagnostic class.

**You found this defect**, at panel 156, at your own boundary and unasked. The
sitting is on your finding and you are asked to finish it rather than restate it.

You judge design.md §1.11 and §4.19 — everything comes from C — and you have a
veto on ABI breakage.

## What only your seat can settle

**Is `struct X *` ever WRONG?** The whole design rests on it being unknowable
without asking clang. Panel 152 measured that `struct nosuchtype *p;` is legal
C, so *does it compile with `struct`* cannot validate a tag. But the question
this sitting needs is the other one: **given a tag that the author wrote and the
header really declares, is there a header shape where `struct X *` fails and
`X *` works?** The obvious candidate is a typedef of an ANONYMOUS struct,
`typedef struct { … } node;` — there is no `struct node` there at all. Write the
headers and compile them. If that shape is real and bindable, then no fixed
spelling can be correct and the round is unavoidable; if it is NOT bindable by
this language for another reason, the option set is larger than the sitting
thinks.

Related and worth one probe: what does this compiler do with
`typedef struct node node;` — both spellings legal — and does the round fire
needlessly there? A round that runs when it need not is a cost on every build
with a handle.

## What you are asked to measure

1. **Reproduce the defect from the tree**, not from your panel 156 notes: build
   `tests/golden/surface-fixtures/structtag/main.hero` to a binary, then with
   `--emit-c`, and compile the artifact with the same flags the build would use
   (`selfhost/cli/flags.hero` is the list). Report the error count and the first
   error verbatim.
2. **The header survey above.**
3. **Whether the artifact is wrong in any OTHER way** than the tag spelling. The
   round exists for `needs_struct`; ask whether anything else the round settles
   is also missing from the `--emit-c` output — the pointee check
   (`cli/pointee.hero`) runs inside the round too, and a caller reading the C
   would not know it never ran.
4. **R3**: `suite_emission.hero`'s fourteen wrong-FFI cases emit precisely
   because `--emit-c` never calls clang. If the repair makes it call clang, say
   what those cases become and whether they still test what they were written
   for.

## Process

Build in a copy: `cp -r` the tree to your scratchpad, `rm -rf target build`. The
repository working tree is frozen for this sitting. **No command over ~60
seconds**; the seed builds in a few seconds, rebuilding from `selfhost/` is 59 s
measured — at most once. Never read `archive/bootstrap-rs/`. Capture exit codes
directly, never through a pipe. Anything unrun is written down as UNRUN with the
command that would settle it.

The Windows box is up and reachable as `ssh win '<bash>'` if a platform question
arises, but nothing here is expected to need it — say so if it does. Do not shut
it down.

## Your verdict owes

A verdict per R1-R4, the C you compiled and what clang said, a falsifiable
prediction, the condition under which you would change your vote, what you left
UNRUN, and whether you cast your veto.

Write your report to `docs/panel/157-reports/ffi-pragmatist.md` **first**.
