# Panel 261, the completeness critic: first reading, the briefs

Written 2026-11-26 from 09:16 by the completeness critic, before any seat starts. My copy is the detached worktree
`.claude/worktrees/261-critic` at `07e747bc66bcd49add47b2ba8011194969df61f2` (`git log -1`, 09:17); its compilers are
built there from the seed and stage 2. Long outputs and my probes are in `.claude/worktrees/261-critic-scratch/`
(`probe/` the clerk's probes re-run, `mine/` mine, `ref/` the references, `linux/` the Linux arm64 run). Every sentence
below was settled by the command beside it, run by me in this session, or says *unrun* or *a question*. I give no
verdict on any route.

## 1. My compilers

`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` then `./heroes build selfhost/main.hero -o heroes-next`,
in my copy, 09:17 to 09:24, both exit 0; `./heroes-next --version` reads `heroes 0.6.0`, **`sources 30e08049445a2331`**,
the clerk's key (`00-shared.md` § 3.6, § 8). The load read 20 to 21 (`uptime`, 09:19 to 09:22), so the times in
`261-critic-scratch/build.log` (stage 2 `real 324.11 user 209.30`) are not durations.

## 2. The framing facts, re-run

**Held, each by the command named** (all from my copy's root, stage 2 unless said):

- **§ 3.1, the spec**: `wc -l -c` 516 lines, 34,828 bytes; `shasum -a 256` `aaf23b9e…6f3`; `sed -n` of lines 109, 123
  to 128, 164, 178, 305 to 309, 310 to 320, 321 and 322, 323 to 329, 334, 383, 427 and 428, 468: every quotation is on
  its line. The section headings (`grep -n '^#'`) put them in § 3, § 4, § 5, § 9, § 11 and § 13 as the brief says.
- **§ 3.2, design.md**: 5,432 lines; §4.8 at 1517 (to 1627, §4.9 at 1629); §4.13 at 2058, the quotations at 2065 and
  2068 to 2074; §4.12's *"Generics on functions only, not on types"* at 2053; Part 6's closure row at 3640 with the
  *capture-free* clause; Part 8 item 19 at 4578 to 4636, its last lines as quoted; §4.8's 1587 to 1595 (the `guess`)
  and 1616 and 1617 (*"`@` parameters never alias ... `restrict` stay legal"*).
- **§ 3.3**: `p1` to `p4` and `u1` under my stage 2 print the clerk's lines, path aside (`diff` after `sed` of the
  path; `261-critic-scratch/probe/*.mine.out`), exit 1 each. `selfhost/data_errors.hero:213` is
  `marker_through_a_value`, code `marker_mismatch`; `walk.hero:1323` and `markers.hero:107` are as named.
- **§ 3.4**: `f1` to `f6` print `6`; `6 8`; `10`; `20`; `6`; `7`, exit 0 each. `f7 --emit-c`: lines 57, 113 and 133 are
  the three quoted. (The clerk's `f7.c`, 12,005 bytes, and `f7-next.c`, 13,409, and mine, 13,697, differ in size by
  the `#line` paths only; the three lines are equal in all three.)
- **§ 3.6**: the seed prints claude-legacy 9,966, cl100k_base 10,144 and the `STALE` line; stage 2 prints real
  **13,309** (`claude-opus-5`, 2026-11-25), headroom 3,075, the floor's 60. The spec's last change is `168d3d6c7`
  (2026-11-26 00:13:40), the pin `selfhost/measure/pinned.hero:64`.
- **§ 4**: all 23 line counts (`wc -l` of the 22 files named plus `runtime/parts/thread.c`, 144) and every function's
  line (`sed -n`: `type.hero` 194, 226, 286, 321, 325, 335; `table.hero` 71, 142, 157, 250; `markers.hero` 35, 64,
  107; `walk.hero` 1296, 1323, 1678; `writes.hero` 260; `flatten.hero` 608, 645, 726; `types.hero` 155;
  `signature.hero` 102, 260; `ctype.hero` 398; `synth.hero` 35; `grammar.hero` 138). `function_ty` on **374 lines in
  141 files**.
- **§ 5.1**: `web.hero` 4,269 lines; `web/internal/routing.hero` 347, `tree.hero` 160, `patterns.hero` 268; lines 125,
  147, 223, 242, 256, 301, 324, 331, 1394, 1408, 1424, 2116 of `web.hero` and 2349, 2390, 2438, 3022, 3494, 3555, 3621
  of `http.hero`; the quotations at `web.hero:19-23`, `:582-585`, `:1361-1362` and `http.hero:2339-2341`; **293 lines
  in 79 files** and **22 lines** by the brief's two commands.
- **§ 5.2**: `ground/census.sh` re-run in my copy: its output is byte-identical to `ground/census.txt` (`diff`).
- **§ 5.3**: notes 49 files, 8,704 lines, 22 `add(@` lines (128 to 152), **35** `if name ==` lines (20, 8, 3, 4 by
  file; 35 with comments excluded too), `http/dispatch.hero` 107 lines, 26 `name == routes.`; auth 22 `web.add`
  (24 to 45), `answer` at 55, 21 arms and `_` (76 to 97); registry 18 `web.add` calls (55 to 72; a bare `grep -c
  'web\.add'` reads 19, line 49 being a comment), 14 arms and `_` (106 to 120), `guarded` at 137, its `match` at 138,
  the three `if found.name ==` at 147, 154, 157, `packages/names.hero:81` and `unpack.hero:179` not dispatch; searchd 10
  (34 to 43), 10 arms and `_` (66 to 76), `answer(s: Service, r)` at 58; chat 5 (49 to 53), `api.answer` 2 arms and
  `_`, `on_read` at `serving.hero:143`, its three `if found.name ==` at 147, 149, 151; reach 3 (31 to 33), `app` at 38, `handle` at
  56, `serve_conn` at 66. **On notes' 35 against the coordinator's 34**: four of the 35 are `main.hero:108-114`, the
  command line's subcommands (one `code = if name ==`, three `else if name ==`), not HTTP; the HTTP dispatch is 31 lines
  (20 + 8 + 3), and the 34 is not reproduced by any command I ran.
- **§ 5.4**: gateway 36 files, 7,291 lines, and every line named (`routes.hero` 46, 75, 102; `pipeline.hero:148`;
  `settings.hero` 83, 198; `calls.hero:38`; `state.hero:104`; `owner.hero` 35, 122; `catalog.hero:139`;
  `reviews.hero:122`); certs, worker, ingest and backup's file and line counts and each line named.
- **The references**: `heroes-next check` exit 0, `fmt | cmp` same, `run` exit 0 with empty stderr, `cmp` against each
  `expected.txt` same, for t1, t2 and t3; every hash and byte count of `registration.md`'s two tables (`shasum -a 256`,
  `wc -c`, `wc -l`). The price lines: `257-reports/llm-ergonomist-blind.md:12` (*"spent 6.297764 USD in all"*),
  `259-reports/llm-ergonomist-blind.md:12` (10.393155); panel 256's runner 23 entries (`ls | wc -l`); the arithmetic
  45 × 0.60 + 1.20 = 28.20, 30 × 0.60 + 1.20 = 19.20, 20.00 − 1.20 − 0.10 = 18.70.
- **Others**: issue 1734 as described (`gh issue view 1734`: filed 2026-10-06 by panel 194's compiler-engineer, closed
  2026-10-07, `9aeac48e`); panel 109:378-386, 112:293-299, 210:36-38, 217:113-126 and its three reports at the lines
  named, 252:565-579 and :650, 254:157, all as quoted; `ftw.h:59-60` is `nftw`; `sqlite3.h` 217 lines with `(*`; spec
  § 13 line 410 holds *"may keep only while the place does not move"*; `docker images` lists `heroes-linux-arm64:latest`
  and `heroes-linux:latest`.

**Wrong or incomplete, each with its command:**

1. **`backup/cli/commands.hero:158-170` is not one `match name` over 8 subcommands** (`sed -n 150,175p`): line 158 is
   `if name == "demo"` and the `match name` is 160 to 169, eight arms and `_ => upkeep.copy(sub)`, so nine
   subcommands, one by `if`, and the `_` arm answers as `copy`.
2. **Panel 252's prediction is carried at its first count, and the sitting corrected it**: `00-shared.md` § 3.5 and
   `compiler-engineer.md` cite *"at least 111 files"*; R3 itself (`252-*.md:567-569`) says *"288 incomplete matches in
   109 files (the critic's re-count of the compiler-engineer's 290 in 111, on the frozen `selfhost/` alone)"*.
3. **§ 1 item 1's parenthesis, *"as § 9 already makes a parameter's name part of it"*, carries half of its precedent.**
   Names are part of a function type's identity **and** have a one-way fit: `selfhost/check/type_fit.hero:13-36`
   (`fits`: *"an expected type that carries NO run of names at all"* accepts a value whose type has one, *"It is a
   widening, and the word is used on purpose"*), feature 1133 (`gh issue view 1133`: *"an expected type carrying none
   accepting a value whose type does"*), and a generic path that compares names on its own
   (`check/generics.hero:118`, `named`; design.md §4.9:1757-1759, `callback_name`). The spec states the identity (§ 9
   305 to 309) and not the fit (`grep -n` of *carrying no*, *names nothing*, *carries none* in the spec: nothing).
   Question 5's *"a conversion"* is this shape for modes; the seats should be told it exists for names.
4. **The two silent sites of panel 112's R6 are not the only places a mode can be lost.** `grep -rn
   'intern_function('` outside tests: a function type is **rebuilt from its parameters, labels and result** at
   `ir/mono.hero:299` (monomorphisation, sketch B's core), `check/generics.hero:326` (substitution),
   `check/type_fit.hero:81` (`fits` re-interns its filled blanks), `check/lower.hero:66` (a written type),
   `check/fixed_repairs.hero:61`, `check/function_value.hero:78` and `:240` (a name as a value); **and two places
   relate function types without comparing ids**: `type_fit.fits` and `generics.bind` (`check/generics.hero:96-130`,
   parameter by parameter, then labels), where a generic `(function(T) -> ())` bound to a value of
   `(function(@App) -> ())` would hide the `@` unless `bind` compares modes (an inference from `bind`'s code, unrun: no
   compiler with modes exists to run it on). And `synth.content_key`
   (`synth.hero:35-42`) hashes `render.render_ty`'s **rendering**, so its silence is `check/render.hero`'s: a mode the
   renderer does not print is a mode the typedef's name does not carry; `instance_key` (`synth.hero:47`) keys a generic
   instance the same way, so `map` instanced over two function types differing in a mode is the same question. **The
   class R6 warned of has happened since, in another dimension**: defect 2650 (`gh issue view 2650`, closed 2026-11-23),
   `(function(main.Value) -> other.Value)` and `(function(other.Value) -> main.Value)` got one typedef,
   `h_0fn_33f39f67`, and clang refused at exit 2 a program `heroes check` accepted. A measured precedent the
   compiler-engineer is not handed: the labels run's landing, `2724a4f9a` (2026-09-11, `git show --stat`), touched **16 files of `selfhost/`, 48
   in all**, and its body (`git show -s --format=%B`) says what question 9 and this item turn on: *".function_ty is
   CONSTRUCTED at one site and its 192 other mentions are match arms"* (today one site still, `table.hero:275`, `grep
   -rn 'function_ty('`); *"generics were free: check/generics.hero's bind descends a function type structurally and
   never compares two whole interned ids, so every generic higher-order call in the tree kept working without a line
   written for it"*, which for a mode is the silence, not the saving; and *"The canonical key omits the names' share
   when a type has none, so a type nobody named keeps the C typedef name it always had"*, nine of 441 emission traces
   moving by a typedef's name alone. A mode run keyed the same way is what keeps a mode-free program's bytes.
5. **f4 is a contradiction between the spec and design.md, and a ruling the brief does not cite.** Spec § 9 line 310:
   *"`x.f(y)` is sugar for `f(x, y)` (UFCS)"*, with exceptions for threads and mailboxes only; by that text f4 prints 13.
   It prints 20 because design.md §4.11:1945 (*"The resolver, on seeing a dot, first looks for a field; failing that,
   looks for a free function"*) and **panel 079** (`docs/panel/079-the-dot-that-two-passes-read-differently.md:66-84`)
   ruled the field first and **refused the collision clause** (*"The collision clause is NOT adopted as a new refusal"*,
   item 4). `grep -n` of *field first*, *looks for a field* in the spec: nothing. So a spec reader, the blind seat
   included, predicts 13. Question 2's `app.handler(@app, r)` stands on this ruling.
6. **The control arm is named nowhere in the briefs.** `tests/golden/control/marker_mismatch-extra-at.hero`,
   `-missing-at.hero`, `-non-place.hero` and `never_rebound.hero` (`ls tests/golden/control`, 58 files) pin what Part
   11's control arm (`build --permissive`, panel 226's R1) gives a dropped rule: a missing `@` *"is the caller's place
   all the same: the declaration decides"*. A call through a value has no declaration; under the proposal its type
   decides, and `ir/flatten.hero:608` `value_call` is where the control arm's meaning of a missing or extra `@` through
   a value is written (`.claude/rules/verification.md`'s map: the control arm's meanings live in `ir/flatten.hero`).
   § 3.3's list of goldens a landing flips omits these four, and the `control` and `permissive` forms.
7. **Question 7's carried code is stale.** `00-shared.md` § 2 question 7 and `ffi-pragmatist.md` (from design.md Part 8
   item 19, 2026-09-05) say an out-parameter in a callback type, `(function(i32, @out: i32) -> i32)`, is
   `expected_function_type_params_close`. Under my stage 2 it is **`error[expected_type]: expected a type, found `@``**
   at the `@`, exit 1 (`261-critic-scratch/mine/o1_out_param.txt`, copied to `.hero` and checked): the same code as
   `p1`. `expected_function_type_params_close` is `lent`'s refusal (`p4`), not `@`'s.
8. **Question 10's list of tools that re-print a program lacks `heroes doc`** (`selfhost/cli/doc.hero`,
   `doc_shape.hero`, `doc_text.hero`; `selfhost/cli/table.hero:195`: *"each declaration's signature ... the call a
   program writes"*) **and the generator that reads it**: `rebel/examples/PACKAGES.md` says *"Every word of it is read
   from what `heroes doc` prints of each face"* (its line 3), written by `rebel/generators/rebel_index/`
   (`packages_text.hero`, `faces.hero`), whose reader of `heroes doc` already broke once on a generic function's
   signature (defect 2761, `gh issue list --search`). A face whose handler type carries a mode is printed by both.
9. **§ 3.3's goldens count (11) is not reproduced** by `grep -rlE` of the three codes and their messages (10 files:
   the two frozen files are `selfhost/data_errors.hero.frozen` and `tests/harness/suite_surface.hero.frozen`; the
   brief's third frozen file is unnamed). `marker_mismatch` alone is on 89 files of `tests/golden` (`grep -rlE | wc
   -l`), almost all calls by name: a landing that changes its reading through a value owes the census of those, not 11.

## 3. The negative sentences and the option set, searched

- **§ 2 question 4's *"`Router<S>` is not a form"*: holds, and wider.** `record Stack<T>` and `variant Maybe<T>` are
  both `expected_end_of_line` (`261-critic-scratch/mine/g1`, `g2`, exit 1): a generic variant is refused too, so no
  declared type of the library can carry `S`.
- **§ 3.5's list of who weighed the form.** With wider words (`grep -lE` over `docs/panel/*.md` for *through a function
  value*, *function type.\*`@`*, *used as a value*, *marker_mismatch*, *generic record*; `gh issue list --search` for
  *function value lent*, *Router*, *closure*, *generic record*, *never_rebound function value*), the brief's list lacks:
  - **panel 121, R5** (`121-the-brace-was-already-taken.md:283-289`): `selfhost/grammar_expr.hero` had no legal home
    because *"the escape hatch is shut because a function type cannot declare an `@` parameter"*: a second measured need
    of the compiler's own beside panel 109's;
  - **panel 138** (`138-*.md:42-48`, `100-126`): the sub-parser taking `@p: Parse` *"is not passable"*, and *"a second
    feature — `@` in function types — that Part 7 does not list and no sitting has proposed"*: the record saying, on
    2026-09-13, that nobody had proposed the form;
  - **panel 079** and design.md §4.11:1945, the field first (item 5 of § 2 above);
  - **panels 129 and 130 and feature 1133**, names in the type with a fit and a generic check (item 3 above);
  - **panel 226**, the control arm's `marker_mismatch` (item 6 above);
  - **feature 2658** (closed 2026-11-23 at `12a19bd98`): *"`web`'s steps cannot reach the server's state"*; it delivered
    `before_with<S>`, `after_with<S>`, `require_basic_with<S>` and `require_bearer_with<S>`, **the by-value route (f)
    already taken in the library**, and says *"a step that changes state, `limit_rate`'s limiter, stays a call the
    program makes"*: those four faces are question 14's census too;
  - **decision 2802** (closed 2026-11-24): `web` stands at **88 public functions** at the base (`grep -c '^function
    [a-z]' rebel/library/0.1.0/files/web.hero`) against the `rebel` suite's fence of **90**
    (`tests/harness/rebel_faces.hero:45-46`, `SIZE_MOST`); sketch B's `get`, `dispatch` and `serve_with`, or a face per
    method, cross it, which is a cost no brief names;
  - **defect 2650** (closed 2026-11-23): two function types got one typedef (§ 2 item 4);
  - **panel 236** (a large record through a function value takes C's convention, `large_value.value_large`;
    `signature.hero:100-102`'s comment) and **panel 199** (a self-call through a function value): two value-call
    conventions a mode slot meets;
  - **panel 217's spec-warden, item 6** (`217-reports/spec-warden.md:71-76`): *"an adapter passing the argument's
    address is sound"* for a `lent` function used where a by-value type is asked. Panel 217 chose the refusal
    (R2, `217-*.md:119-123`, *"a function with a `lent` parameter used as a value"* among *"the refusals that are part
    of the form"*) **with that alternative written in its record**: so question 12's *"a ruling or an absence"* has a
    measured answer the brief can hand over, a ruling with its rejected alternative named.
- **§ 5.4's *"unread"* for `agent`, `iothub` and `monitor`: read** (`grep -rnE` of `serve`, `listen`, `if name ==`,
  `path ==`, `match name` in each, then `sed -n`):
  - **agent dispatches by name, and it is the one read-only table in Rebel's examples**: `tools/toolbox.hero:94-113`,
    `run(box: Toolbox lent, name: str, input: json.Value) -> outcomes.Outcome`, six `if name ==` for the six MCP tools,
    one of them guarded (`name == "run_command" && box.allowed.len() > 0`), every handler reading parts of `box`; it is
    served over stdio by `mcp/server.hero:106` (`serve_stdio`), a `jsonrpc`/`mcp` face (`jsonrpc.hero:8`, *"A server
    finds, the program calls"*). Its stand-ins dispatch HTTP by path (`standins/openai_api.hero:138`, `:158`;
    `claude_api.hero:118`), and `app/commands.hero:65-80` its command line by `if name ==`;
  - **monitor**: `standins/collector.hero:141-199`, `answer_of(@c: Collector, r)`, a stop path and three OTLP paths, the
    shape of worker's collector;
  - **iothub**: one path, `feed/server.hero:135` (`/feed`, a WebSocket upgrade); its `match name` at
    `config/registry.hero:119` and `:142` are parsers of a name into a variant, not dispatch;
  - **and `server`, in the census (1 file, 313 lines) and in neither § 5.3 nor § 5.4**: `server/main.hero:249-253`,
    `answer(request)`, one path `/hello`, no state, under `http.serve_conn`.
- **§ 6.2's *"none compiled"***: holds for A and B as written (each writes `@S` in a function type, which is `p1`). What
  is unbuilt beneath it: sketch A indexes `app.handlers` by the router's own index while notes adds routes into three
  groups, `mount`s them and lets `openapi.add_routes` add its own (`notes/http/routes.hero:143-157`, `add_routes` at 155); whether the
  router's index stays aligned with a push order across `mount` is a question for the seat that builds A.
- **§ 7's list**: § 6 below names the routes it lacks.

## 4. The blind registration

**Can a `now` reader meet each task**: yes, by the three references, re-run above on the Mac and in § 4.1 on Linux
arm64.

**Does a task's wording tell the reader which arm or which form**: I found no word that names an arm, a mode or `@`
(`grep -n '@\|lent\|mode\|function value'` of the three tasks: nothing). The project's name does not leak beyond what
the spec itself says (`main.hero` names the extension; the spec's line 1 names the language). **But the uniform table
is the task's structure, not its wording**: each task asks one table, so every bound function has one type, and the
read-only ones take the writers' mode. That is question 5 by design and is fair to both arms.

**Repairs the registration needs, each a rule two graders could read two ways or a count that measures something
else:**

1. **`never_rebound` is not a mode's code alone.** `selfhost/resolve/errors.hero:340` raises it for *"a mutable cell
   and nothing re-binds it"* and `:357` for `_ @= e`, `written_nowhere.hero:75` for `@_`, and `:92` for *"an `@`
   parameter and nothing writes it"*: one code, four messages. `refused-for-mode` counts the code, so a reader's `x @= 0` never
   re-bound, a mistake about cells, counts against D3. The rule must read the message (the `@` parameter's and the
   `@_` parameter's) or the line (a bound function's parameter).
2. **A write that changes nothing passes `never_rebound`, and the grades cannot see it.** `function health(@app: App,
   x: i64) -> i64` with `app @ app` as its first line checks and runs, exit 0, printing 4 (`mine/c1_selfassign.hero`);
   so does `app.n @ app.n` (`c8`), and a cell's `x @ x` (`c7`, `heroes-next check` exit 0 each). A reader under
   `adopted` who meets or foresees `never_rebound` can write one, and is graded compiles, bound, mode `@`, not
   refused-for-mode, so D3 passes on a workaround. The registration should report a self-write in a bound function as
   its own row, and whether such a write is a write by design is a question for the sitting (I found no ruling:
   `grep -niE 'itself|self-assign'` of panels 209 and 210, nothing).
3. **`compiles` has two judges for a `now` program and the rule does not say which grade counts.** *"under the landed
   compiler ... a `now` session's program is also built by the base's stage 2"*. A `now` reader who writes a mode in a
   function type (the language it read forbids it) is refused by the base and may be accepted by the landed compiler;
   D2 compares the arms' not-prints-right counts, so the choice moves D2. Say: a `now` program is graded by the base's
   stage 2 (the language it read), the landed result reported beside it.
4. **`bound`'s rule (3) reads the t1 reference two ways.** Its `handle` chooses the answering function with `if
   route.method == r.method` while walking the table. The rule's permitted list names *"(t1 only) the matching of a
   path's segments against a pattern"* and not a method; its forbidden list does not name a method either. Say that a
   walk comparing each entry's key fields (method and segments in t1, state and event in t3) for equality with the
   request is the table's own lookup; t3 written as a list of `(state, event, function)` records walked by `if
   t.state == m.state && t.event == e` meets the same two readings.
5. **D3 leaves out t1, whose GET routes are read-only wherever the count is kept outside the handlers**, as the
   reference keeps it (`s.answered @ s.answered + 1` in `main`). A t1 reader meets question 5 or not by where it puts
   the count; D3 should say why t1 is out, or read it.
6. **A program refused for several codes**: *"A refusal is recorded with its code and line"* says one; `heroes check`
   prints several. Say: every code printed, and refused-for-mode where any is a mode's.
7. **`brief-session.md` and `registration-runner.md` do not exist yet** (`ls blind/`), so their fields, which decide
   what a reader is asked, are not readable by me now; the second reading should read them.

### 4.1 The references on Linux arm64

`docker run --rm --pull never --name p261-critic heroes-linux-arm64:latest` over `git archive HEAD selfhost runtime
seed vendor spec` of the base (`261-critic-scratch/linux/`), 09:29 to 09:41: `aarch64`, Debian clang 22.1.8; the seed
then stage 2, whose `--version` reads **`sources 30e08049445a2331`**, the Mac's key; then `heroes-next run` of each
reference three times: **nine runs, exit 0, stderr empty, each `cmp`-identical to its `expected.txt`**
(`linux/run.log`, `linux/ref/t*/linux-*.out`). So the key holds on both platforms the registration grades on, for the
`now` references. The container is removed (`docker ps -a`, no `p261-critic`).

## 5. Who was handed what

- **The historian** (Read, Write, WebSearch, WebFetch: `.claude/agents/historian.md`'s `tools:`) **cannot open a
  glob**: its brief cites `docs/panel/217-*.md:107`; the file is
  `docs/panel/217-a-read-only-lend-is-lent-on-a-parameter-the-callers-place-read-and-never-written-refused-beside-an-at-of-that-place-and-past-a-variadic-prototype.md`.
  It needs panel 138's historian (OCaml's refusal of constructors as functions, quoted at `138-*.md:58`) only as
  context. **Its item 10 asks a negative** (*a language with neither closures nor generic types*): C's own routers,
  which thread a `void *` the program owns beside a function pointer, are the route by type erasure; I have not read
  their pages, so this is a question for it, not a fact.
- **The ffi-pragmatist** (Read, Write, Bash) is handed Part 8 item 19's counts as carried and asked to re-run them: it
  can. It is not told that the emitter already writes a pointer slot in a library function's value twin
  (`signature.hero:326-342`'s test: `hero_value_h_library_dist(int64_t h0_a, int64_t *ph1_b)`, from a library function
  with `@b`), nor that a large record through a value takes C's convention (`signature.hero:100-102`, panel 236).
- **Whether a conclusion rests on carried counts alone: question 7's C half does.** Every function-typed parameter
  inside a `group` or `extern` block of the tree is a thread body: 2 in Rebel's library (`threads/hero_os.hero:22`,
  `:37`) and 16 in `selfhost/`, `examples/` and `rebel/examples/` (`awk` over the files `grep -rlE '^(group|extern) '`
  names, printing each `(function(` line inside a block), every one `(function(i64) -> i64)`, `(function(x: i64) ->
  i64)` or `(function(str) -> str)`, the `body` of a `hero_thread_spawn` function. So no binding in the tree needs a C callback receiving a struct pointer or an
  out-parameter, and what the proposal unlocks at the C boundary rests on Part 8's `sqlite3.h` counts (carried, the
  ffi-pragmatist told to re-run them), on panel 217's libuv example and on the bindings the ffi brief asks it to write
  (`nftw`, `sqlite3_exec`): a lower bound of my search, by the command above.
- **The compiler-engineer** is not handed: the rebuild and relation sites of § 2 item 4; `check/render.hero` under
  `content_key`; the labels run's cost (`2724a4f9a`); panel 079's ruling (§ 2 item 5); the control arm (§ 2 item 6); my
  `q2` probes (§ 6). **Nothing it is asked rests on panel 112's or Part 8's carried counts alone**: it is told to count
  both shapes by building them.
- **The spec-warden** is not handed: that the spec does not state the field-first rule f4 rests on (§ 2 item 5), nor
  the names' fit (§ 2 item 3); and `never_rebound`'s shared code (§ 4 item 1), which its D3 predictions read.
- **The blind seat**: § 4.

## 6. The routes and shapes nobody listed

Each probe is in `261-critic-scratch/mine/`, run by my stage 2.

- **A value read from the place its own call changes, today**: `b.f(swap(@b))`, where `swap` writes `b.f` and answers
  5, prints **10**: the callee `double` was read before the nested call made it `triple` (`q2_nested_changes_callee`,
  exit 0); the same for an element, `fs[0](swap(@fs))` prints 10 (`q2_array_nested`). The emitted C loads the callee
  into a temporary before the arguments (`f7.mine.c:134`, `t7 = ...[t6]; ... t10 = t7(t8, t9);`). So question 2's
  *"a copy taken before the lend"* is what the base already does for a nested `@`; the seats can start from it.
- **A handler that takes no state** (notes' `logout()`, auth's `login_page(r)`), the shape beside question 5's
  read-only one: in a table of `@` handlers it is `@_: App`, which `never_rebound` refuses in its own words (*"`_` binds
  nothing, so an `@` on it marks a parameter nothing can write"*, `c2`, exit 1); `_: App lent` is accepted (`c3`, exit
  0); an unread named one is `unused_binding` (`c4`, `c5`). So a uniform `@` table has three kinds of member, not two.
- **The table as a top-level `constant`**: `constant TABLE: {str: (function(i64) -> i64)}` holding `{"d": double, "t":
  triple}` runs, printing 12 (`k3`), and a `[double]` list too (`k1`); a list of records is refused,
  `constant_body` (*"may not contain a call"*, `k2`). A command table can therefore be one constant, listed once and
  never built at run time; the refusal's note says a constant's body is re-evaluated on every read, a cost per lookup.
- **A name-keyed map beside the router**: `{str: (function(@App, web.Found, http.Request) -> http.Response?)}` keyed by
  `found.name` needs no field in `web.Found` (sketch A needs `index`); unbuilt.
- **The callee and the state in disjoint places**: `server.handlers[i](@server.state, r)` reads the callee from a field
  the call does not lend, so question 2 does not arise; unbuilt.
- **`lent` adapted to a by-value type** (panel 217's spec-warden, item 6): a third conversion beside question 5's
  `lent` to `@`; unbuilt.
- **The state lent read-only and the changes answered as data**: each handler takes `App lent` and answers a list of a
  variant of changes that one function applies by `@`; distinct from (f), which answers the whole state; unbuilt.
- **The compiler's own knot through a value**: panel 109's `@b` walk, panel 121's grammar module and panel 138's
  sub-parser, and Rebel's `interpreter/run/value.hero:8-13`. No seat is asked to build one; it is the only route by
  which the form is compiler-need (CLAUDE.md § 2).
- **A callback C runs on another thread**: the 18 thread-body callbacks above are function values C calls on a thread
  of its own; a mode admitted in an `extern` callback's type and handed to `hero_thread_spawn` would, by my reading of
  those declarations (unrun), hand one thread's place to another, so question 7 (a mode at the C boundary) and question
  8 (`spawn`) meet in them and are one question there, not two.
- **Shapes no brief names**: a mode-carrying value sent through a mailbox (function values cross threads as copies); a
  function-typed `@` parameter whose type carries modes, `(function(@(function(@App) -> ())) -> ())`; a mode in a
  generic parameter's position, `(function(@T) -> ())` bound by `bind`; the `T?` of such a value called after
  `.must()` (f3's shape with modes); an empty table `[]` typed by context; and **the library faces that take a function
  today besides `web` and `http`**: `web`'s `*_with<S>` steps (feature 2658), `openapi.add_routes` (notes' API routes
  are named by its document, `notes/http/routes.hero:155`), `jsonrpc`/`mcp` (agent), `graphql`, `grpc`, `jobs`, `ui`
  (`kanban`, `explorer`).

**The program of § 5 the briefs read least**: **agent**, marked *unread*, which is the one table in Rebel's examples
whose every handler only reads (`Toolbox lent`) and which carries per-entry data beside the handler (the
`run_command` guard): it is question 5's driving program with no `@` at all, and the `lent` half's.

## What the coordinator must repair before any seat starts

1. **Hand every seat the sites that rebuild or relate a function type** (§ 2 item 4): `ir/mono.hero:299`,
   `check/generics.hero:326`, `check/type_fit.hero:81`, `check/lower.hero:66`, `check/fixed_repairs.hero:61`,
   `check/function_value.hero:78` and `:240`, `type_fit.fits`, `generics.bind`, and `check/render.hero` under
   `content_key`; and the labels run's cost, `2724a4f9a`. Question 1 names two silent sites where there are at least
   ten, and a `(function(T) -> ())` bound to an `@` value through `bind` is the case to build first (my inference that it
   would hide an `@` is unrun).
2. **Correct § 1 item 1's analogy** (§ 2 item 3): names are identity plus a one-way fit and a generic agreement check;
   say so, and put question 5's conversion beside it.
3. **Repair the blind registration** (§ 4): `never_rebound` read by its message, the self-write reported, the `now`
   arm's `compiles` judge named, rule (3)'s method and key-field comparisons named, D3's exclusion of t1 justified or
   removed, every printed code counted; and say that `brief-session.md` and `registration-runner.md` are owed.
4. **Add panel 079 and design.md §4.11:1945 to § 3.5 and question 2**, and tell the spec-warden the spec does not state
   the field-first rule (§ 2 item 5).
5. **Add the control arm to § 3.3, question 6 and the compiler-engineer's case list** (§ 2 item 6): what `build
   --permissive` gives a missing or extra `@` through a value, and the `control` and `permissive` forms among the
   goldens a landing moves.
6. **Add the sittings and issues § 3.5 lacks** (§ 3): panels 121, 138, 079, 129 and 130, 226, 236, 199, panel 217's
   spec-warden item 6 (answering question 12), feature 2658 and decision 2802 (`web`'s 88 of 90 public functions).
7. **Read agent, monitor, iothub and server into § 5.4** (§ 3), agent as question 5's driving program.
8. **Add § 6's routes and shapes to § 7 and to the compiler-engineer's cases**: the no-state handler, the constant
   table, the name-keyed map, disjoint places, the `lent` adapter, changes answered as data, the compiler's own knot,
   a callback C runs on another thread; and tell the ffi-pragmatist that no binding in the tree needs question 7's
   spelling (§ 5), so its verdict on the C half rests on the bindings it writes.
9. **Small corrections**: question 7's and the ffi brief's code for an out-parameter in a callback type, which is
   `expected_type` today (§ 2 item 7); backup's `cli/commands.hero` range (§ 2 item 1); panel 252's count as its R3
   corrected it (§ 2 item 2); notes' 31 HTTP dispatch lines beside the 35; `heroes doc` and `rebel_index` in question
   10's list (§ 2 item 8); § 3.3's golden count and its third frozen file (§ 2 item 9); the historian's glob path
   (§ 5).
