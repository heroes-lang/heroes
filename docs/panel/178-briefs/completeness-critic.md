# Panel 178 — completeness critic

You are not a sixth judge and you give no verdict. Read the six briefs in this
directory and the five reports in `docs/panel/178-reports/` (the lane
`~/Temp/heroes-lane-panel-178`), and name what is MISSING: a route nobody
listed, a claim asserted and not measured with the command that settles it, a
contradiction between seats and which side is checkable, and the question the
sitting should have asked. `.claude/skills/panel/SKILL.md` step 3b is your
definition.

Work in `<scratchpad>/178-completeness-critic/`, a `git archive` of HEAD
`57679005` made for you; build the compiler there from the seed. You may READ
and copy the seats' prototypes and probes out of their directories and rerun
them in yours; never build or run inside theirs. The lane and the trunk are
read-only. Return your report as your final message; the coordinator writes it
to `docs/panel/178-reports/completeness-critic.md` unchanged.

## Known to the coordinator, so report them only if you find them wrong

- **The padding method measures nothing at `-O2`**: the control reads 0 there
  on every leg (`00-shared.md` measurement 8).
- **`cond` and `rwlock` zero-validity on Linux are unrun**: the macro
  expansions contain enum names the script cannot evaluate.
- **The UTF-8 `sun_path` probe never reached its question**: it dies at defect
  091 before the first `to_i8()`.
- **Windows is unrun for every row**: the box was not started.
- **The three kinds of array are a classification by the coordinator**, not a
  count.

## And one question put to you directly

The shared brief argues that `[x; N]` writes a length that is true on one
platform (`utsname` is `[256]` on Darwin and `[65]` on Linux), while `rest:
zero` writes none. But the record's own field declaration writes the length
too, and is refused on the other platform (`ffi_field_type`). Say whether that
argument separates the routes at all, or only moves the length from the
construction to the declaration.

## Known since the five seats reported, added before you started

- **Defect 091's lowering was built twice, independently**: the ffi-pragmatist
  (+18 / −1 by `diff -u`, `docs/panel/178-reports/ffi-pragmatist-work/defect-091-lowering.diff`)
  and the compiler-engineer (+27 / −6 in its report), each green on the
  compiler's 676 tests; the net ran with neither.
- **Three findings in what ships, reproduced by the coordinator on all three
  legs** before filing, from the seats' own probes: a whole group record lent to
  a `void *` parameter with a C count (`read(fd: 0, buf: @h, n: 4096)` into a
  `partial` addrinfo) is `check` 0 and dies at 138 / 135 / 139, `--sanitize`
  reading *stack-buffer-overflow* on every leg (to be defect **092**); a `str`
  with an interior zero byte from `read_file` reaches C through `.cstr()` as 2
  bytes of 5 at exit 0 (**093**); a group `constant` whose header value is a
  struct initialiser is `check` 0 and build exit 2, *internal error* (**094**).
- **Three reports were written out by the coordinator** from the seats' final
  messages (the llm-ergonomist, the ffi-pragmatist, the historian), because the
  harness refused their writes or the seat has no write tool; the
  compiler-engineer's and the spec-warden's were copied from their own
  `REPORT.md`. Each file's header comment says which.
- **The llm-ergonomist's report ends by saying a `CLAUDE.md` was in its
  context.** Its directory held two files; the harness loads rule files into a
  subagent. That is panel 175's open question about the blind seat, not new.
- **The seats' work files** the synthesis may cite are copied into
  `docs/panel/178-reports/<seat>-work/`, generated C and build caches removed.
