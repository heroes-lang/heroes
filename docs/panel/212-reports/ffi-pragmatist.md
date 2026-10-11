# Panel 212, the ffi-pragmatist's report: how does the verb talk to GitHub (Q2)

Seat: ffi-pragmatist. Tree frozen at `09060ea3`, own worktree
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-panel-212/ffi-pragmatist`, own compiler built from
the seed there (`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, 7.5 s, `heroes 0.3.0`).
Everything I wrote is under `scratch/` of that worktree. **No request that changes GitHub was made**; the only
requests to GitHub were GETs (`gh api` reads) and one GraphQL **query** (a POST at HTTP level, a read in effect:
section 4, said here because the brief wrote "GET"). Every POST in this report went to a local fake server I wrote
(`scratch/fake.py`, plain and TLS, loopback only).

## DISCLOSURE FIRST: I printed the author's GitHub token into my tool output

While checking whether `gh auth token` honours an empty `GH_CONFIG_DIR` I ran it without redirecting its output, and
it printed the author's OAuth token (a `gho_...` token, 40 characters) into this session's tool output. I wrote it to
**no file** and it is **not** in this report; it sits in this session's transcript. It does not stand in
the repository tree. **Recommendation: tell the author; `gh auth refresh` or a logout and login rotates it.** The
same run shows something the sitting needs anyway: `gh auth token` answered from the keyring even with an empty
config directory, so any process of the same user can ask for the token the same way (section 5).

## The five required lines

- `verdict`: **approve Route A** (spawn `gh api`) as the verb's transport; **veto Route B inside the compiler binary**
  (libcurl across the C boundary); Route C (`curl`) is an acceptable second, with its token handling objected to.
  Route D is Route A with a different payload and is not a separate transport.
- `section`: design.md §1.11 (no standard library, everything from C) and §4.19 (the FFI, the importc-style header
  check), spec § 13 (the sentence *two records may not name one tag but `void`*, and *a parameter ... is taken to
  keep what it is handed unless declared `lent`*), CLAUDE.md § Commands line 1 and `seed/README.md` (the compiler is
  built **from C alone**, one command), and `selfhost/cli/refresh.hero:3-5` (the compiler's one network address).
  design.md has no section on *spawning a program*; the objection to B on the bootstrap line stands on CLAUDE.md and
  `ci.yml:439, 469`, not on §4.19.
- `experiment`: see sections 1 to 6. In short: a libcurl POST binding written and **built by clang against the real
  `curl/curl.h`** (accepted, 6 modules, 124 lines, run against a local server: status 201, 54,818-byte body
  received), the same C written by hand and compiled (accepted), a deliberately wrong option/value pairing (clang
  accepted both the Heroes and the C); `gh api` driven from a Heroes probe that binds the runtime's own
  `hero_run_go` (scenarios: ok, missing, unauthenticated, bad token, 404, secondary and primary rate limit, slow,
  partial, 301, 307, graphql errors beside data, killed); `curl` likewise.
- `argument` (112 words): Route A costs the boundary nothing. `hero_run_go` already takes a stdin path and
  `hero_run_limit` a timeout, so `gh api --input -` is a Heroes-side parameter, zero lines of C. `-i --jq` returns
  status, `Retry-After` and the issue number in one file, and `-f`/`-F` make gh build the JSON, so no JSON builder and
  no second escape. The token stays in gh's keyring. Route B compiles but needs six modules, because
  `curl_easy_setopt` is variadic (one module per value type, the handle written `core.Curl`); clang cannot check a
  wrong option/value pairing (measured); and `-lcurl` enters the bootstrap line, while `ci.yml:439` says the Windows
  leg has no libcurl. In the compiler, that is a veto.
- `prediction`: (1) Under Route A, `heroes issues push` needs **0 lines changed in `runtime/`**, **no change to the
  seed's link line**, and in `selfhost/cli/process.hero` one new extern line (`hero_run_limit`, bound today only in
  `selfhost/mutate/child.hero:26`) plus an `in:` parameter on `run`; falsified if the first implementation edits
  `runtime/` or `seed/README.md`. (2) Under Route B, the CI Windows leg cannot link `heroes` from `seed/heroes.c`
  until a libcurl is installed there (`ci.yml` installs libcurl only on Linux, line 316; line 439 says the Windows
  leg has none); the same emitted C without `-lcurl` fails with *Undefined symbols ... `_curl_easy_init`* (measured).
  (3) The `examples/curl` rung (§4.19 ladder step 4) **cannot be extended to a POST in one module**.
- `condition`: I move B from veto to object if the sitting makes libcurl an optional, separately linked helper that the
  bootstrap line never sees (which is a second binary and § 10 forbids it). I withdraw the approval of A if the
  transcribing machine has no `gh` (then C), or if a measured run shows `gh api --input -` or `-i --jq` changing
  behaviour across the `gh` versions in use (I measured 2.102.0 only; the floor is unrun). I would also object to A
  if the sitting wants per-request connection reuse for 5,918 requests: measured read latency is 0.48 s per `gh`
  call against 0.33 s with one reused connection, so the pacing that GitHub's secondary limits ask for (unread here)
  will dominate, an inference.

## 1. Route A: spawn `gh`, what a Heroes program sees

**The probe** is a Heroes program (`scratch/probe.hero`) binding `hero_os.h` exactly as `selfhost/cli/process.hero`
and `tests/harness/shell.hero` do (`hero_run_reset`, `hero_run_arg`, `hero_run_go`, `hero_run_why`,
`hero_run_signal`, `hero_run_limit`). It prints `Ran`'s four fields. Built with
`../heroes build probe.hero -o probe`.

**A correction to the brief's F7 and the critic's reading.** `process.run` passes `in_path: "".cstr()`
(`process.hero:96-103`), but the **runtime** already takes a stdin path (`hero_run_go(program, in_path, out_path,
err_path, @status)`, `runtime/hero_os.h:369`, `runtime/parts/run.c:1025-1035`, plumbed so a harness case can feed a
program). Measured with my probe: `in_path = scratch/in.txt`, program `cat`: output `hello-from-file`. And an
**empty** path means *inherit the parent's stdin*, not *no stdin*: `echo via-parent-stdin | ./probe out err "" 0 cat`
printed `via-parent-stdin`. So `gh api --input -` is reachable today with **no C change**: it needs an `in:`
parameter in the Heroes wrapper, and I ran it: `./probe out.txt err.txt $PWD/big.json 30 gh api -X POST
/repos/o/r/issues/ok --input -` delivered 54,818 bytes (the server logged `te: chunked`, `len: 54818`). The `--input
<file>` form needs nothing (also 54,818 bytes). **The 54,818-byte body passes both ways**, and a third way, `-F
body=@file` (gh reads the file as a JSON string field), delivered a 54,849-byte request (the body plus the other
fields).

| scenario (command run through the probe) | `code` | `started` | `why` | `signal` | out file | err file |
|---|---|---|---|---|---|---|
| `gh api repos/heroes-lang/heroes --jq .full_name` | 0 | true | 0 | 0 | `heroes-lang/heroes` | empty |
| `gh` missing (`PATH=/usr/bin:/bin`) | -1 | **false** | 2 (ENOENT) | 0 | empty | empty |
| not logged in (`GH_CONFIG_DIR=<empty>`, no token) | **4** | true | 0 | 0 | empty | `To get started with GitHub CLI, please run:  gh auth login ...` |
| bad token (`GH_TOKEN=notatoken`, a read) | **1** | true | 0 | 0 | the body: `"message": "Bad credentials"`, `"status": "401"` | `gh: Bad credentials (HTTP 401)` |
| 404 on a read | **1** | true | 0 | 0 | body | `gh: Not Found (HTTP 404)` |
| fake secondary limit (403, `Retry-After: 60`) | **1** | true | 0 | 0 | body, or headers + body with `-i` | `gh: You have exceeded a secondary rate limit... (HTTP 403)` |
| fake primary limit (403, `X-Ratelimit-Remaining: 0`, `-Reset`) | **1** | true | 0 | 0 | same | `gh: API rate limit exceeded ... (HTTP 403)` |
| slow server, `hero_run_limit(2)` | **124** | true | 0 | 0 | empty | empty |
| `sh -c 'kill -9 $$'` stands for a killed `gh` | 137 | true | 0 | **9** | | |

What this says. **Every HTTP failure is exit 1**, the status only in stderr's last words and, with `-i`, in the first
line of the out file, so **the verb must read the status line, never the exit code**. Exit 4 is the one thing the exit
code names (no authentication). `started=false` separates "no gh" from "gh failed", as `Ran.started` was designed
to. **`gh` retries nothing**: the fake's log holds one request per invocation, for both 403s. **Where a rate limit
shows** (the shapes are the ones my fake sent, a construction: **I did not provoke a real limit and what GitHub sends
is unrun**; what is real is that `gh -i` copies every header GitHub sends to the out file, shown on a real GET, which
carried `X-Ratelimit-Limit/Remaining/Reset/Resource/Used`): in the out file with `-i`, never in the exit code and,
without `-i`, nowhere. `gh api -i --jq` on a 2xx prints **headers, a blank line, then the jq result**: on the real
repository `-i --jq .full_name` printed 30 header lines, a blank line and `heroes-lang/heroes`. So **one call, one
file, gives the status line, `Retry-After`, the rate headers and the new issue's number, and needs no JSON parser**.
On an error `--jq` is not applied (the body comes back raw).

**JSON and escaping under A: no second escape, no builder.** `-f key=value` (raw string) and `-F key=@file` make gh
build the request object. Verified against the fake, which logged the body it received for
`-f title=123 -F body=@tricky.txt -f 'labels[]=kind:defect' -f 'labels[]=area:selfhost'`, where `tricky.txt` holds
a quote, a backslash, a newline, a tab, a 0x01 control, `é`, `日本`, `#12` and `@someone`:
`{"body":"line1 \"quoted\" \\ back\nline2\ttab \u0001ctl é 日本 and #12 @someone\n","labels":["kind:defect","area:selfhost"],"title":"123"}`.
`-f title=123` stays a **string** (`-F title=123` would become a number: use `-f` for text, `-F` only for `@file` and
numbers such as `milestone`). `selfhost/cli/json_text.hero`'s `json_string` is therefore **not needed** under A.

**The host is not pinned by the tree unless the verb says so.** With `GH_HOST=localhost:18766` in the environment a
plain `gh api /ok` went to the fake (one logged request); with `--hostname github.com` the same environment went to
`api.github.com` (a TLS refusal against my pinned `SSL_CERT_FILE`, the fake's log unchanged). So `--hostname
github.com` belongs in every invocation, otherwise an environment variable chooses where the 2,014 issues go. This
is the answer to `selfhost/cli/refresh.hero:14-17` (*one address ... no other URL exists in this tree*): under A no
URL literal is needed, but the pin has to be a flag.

## 2. Route B: libcurl across the C boundary

**The reference C** (`scratch/post_ref.c`, 40 lines: `curl_easy_init`, `curl_slist_append` x2, six `setopt`s,
`curl_easy_perform`, `curl_easy_getinfo`) was compiled with `clang -Wall -Wextra -o post_ref post_ref.c -lcurl`
against the SDK's `curl/curl.h`: **accepted** (one unused-parameter warning from `argc`) and it ran against the fake:
`rc=0 status=201 body={"number": 1, ... "got": 54818}`.

**The Heroes binding** is `scratch/post/` (`core.hero`, `opt_text.hero`, `opt_long.hero`, `opt_slist.hero`,
`opt_file.hero`, `main.hero`; 97 non-comment lines in 23 `function`/`constant`/`record` members). `heroes build
main.hero -o post` **was accepted by clang** (generated C compiled against the real header) and the program runs:
status 201 and the 54,818-byte body on `/ok`, 403 + body on `/secondary`, 301 on `/moved`, rc 18 *Transferred a
partial file* with status 201 on `/partial`. What §13 let me write and what it refused, in the order I met it:

1. **`curl_easy_setopt` is variadic, so one module binds one value type.** The value for `CURLOPT_URL` and
   `CURLOPT_POSTFIELDS` is `cstr`, for `CURLOPT_TIMEOUT` it is an integer, for `CURLOPT_HTTPHEADER` a
   `struct curl_slist *`, for `CURLOPT_WRITEDATA` a `FILE *`. Panel 094's two-module route
   (`tests/golden/surface-fixtures/twoarity/README.md`) is the only way, so **four setopt modules**.
2. **The handle cannot be declared in each of them**: `error[duplicate_tag]: Curl and Curl are both handles over
   CURL`, and `error[type_mismatch]: expected Curl, found Curl` when one module's handle is handed to another's.
   This is spec § 13's *two records may not name one tag but `void`*. **What works**: the handle is declared once in
   a home module (`core.hero`) and every other group writes `handle: core.Curl`, a qualified type in a group's
   parameter, which the checker accepted. Nothing in the spec says this is allowed; I found it by trying.
3. **An `extern` is never callable across a module** (`error[extern_across_modules]`), so each group has a Heroes
   wrapper and `perform` has one, in `core`, which `opt_text` calls.
4. **`consumes` in a wrapper is a syntax error** (`expected_params_close`): a wrapper takes `@handle: Curl`.
   `curl_slist_append` needed `@list` and `list @ curl_slist_append(list, ...)`, and the first call needs
   `list: core.Slist @= nullptr`.
5. **`lent` is refused on a handle parameter** (`error[lent_shape]`: only `cstr` and `ptr` are lent).
6. **`CURLOPT_POSTFIELDS` keeps the pointer**, so the `cstr` parameter is declared without `lent` and the body
   crosses as a lease, set, performed and released **in one function** (`opt_text.perform_posting`), which is why
   `perform` is called from inside `opt_text`.
7. **The response body is sunk to a `FILE *`**: the default `fwrite` write function with `CURLOPT_WRITEDATA`
   given a `File` (`record File tag FILE`, `fopen ... acquires fclose`), read back as a file afterwards: the same
   scratch-file dance as route C, in process. A write **callback** is expressible
   (`value: (function(data: ptr, size: u64, count: u64, user: ptr) -> u64)`, and a Heroes function calling
   `fwrite(data: ptr, ..., stream: ptr)` **built**, `scratch/post/cb/`) but **I did not run it**; Heroes has no
   way to read bytes behind a `ptr`, so a callback can only forward them.
8. **The response headers (`Retry-After`) are not in this binding.** They need `CURLOPT_HEADERDATA` (another `File`
   option, one more constant in `opt_file`) or `curl_easy_header`; unbuilt, so a rate limit under B shows only as
   `status=403`, without `Retry-After`.

**The importc-style verification the thesis rests on does not reach the option/value pairing.** I built
`scratch/post/wrong/`: `CURLOPT_URL` handed an integer, `12345`. **Heroes and clang accepted it**, and at run time the
runtime panicked (exit 134, *a null pointer was read through, at offset 0x3030*, a message about a null that was a
wrong integer). The same mistake in C with a **constant** option, `curl_easy_setopt(h, CURLOPT_URL, 12345L)`,
`clang -Wall -Wextra -fsyntax-only`: exit 0, no diagnostic, because `typecheck-gcc.h`'s checks are
`__attribute__((__warning__))`, which clang does not implement (`typecheck-gcc.h:166`). **What is checked is the
option's *number* (the constant comes from the header) and the existence of the names; what is not is which of the
seven options takes which type**, and that is exactly where a wrong binding corrupts. §4.19's promise is not
broken by the proposal (variadics are already like this, panel 094), but libcurl is the biggest customer of it.

**What B does to the build** (this is the veto). `heroes build main.hero --emit-c -o post.c` then `clang -I runtime
post.c runtime/runtime.c -o post_nolink` fails: `Undefined symbols for architecture arm64: "_curl_easy_cleanup" ...
"_curl_easy_init"`; with `-lcurl` it links and runs. The compiler's own bootstrap is `clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes` (CLAUDE.md § Commands, `seed/README.md`: *one command, no flags beyond the include
path*; `ci.yml:504, 546` run it). A libcurl in the compiler puts `-lcurl` into that line and a dynamic
dependency into every `heroes` invocation, `lex` and `fmt` included, against design.md Part 11 metric 1 (*no API key,
no network*, which `refresh.hero:3-5` cites). `ci.yml:439` states *sqlite, curl and raylib are not [on the Windows
leg]* and `ci.yml:469` that the curl examples *skip for want of their libraries*; `suite_surface.hero:585-586` marks
`examples/curl` `.may_skip()`. The Windows box has `gh` and `curl` through winget and Git for Windows
(`docs/platforms/windows/WINDOWS-MACHINE.md:329-342`) and no libcurl headers (`ci.yml:439`, the box itself unread).
**The project's own record says libcurl cannot be assumed on all three platforms, and the spawn routes build
everywhere and say `started=false` where the program is absent.**

## 3. Route C: spawn `curl`, which the compiler already does

`selfhost/cli/refresh.hero:81-82` runs `curl --silent --show-error --variable %ANTHROPIC_API_KEY --expand-header
"x-api-key: {{ANTHROPIC_API_KEY}}" --header ... --data-binary @<file> <ENDPOINT>`, the answer to a file via the
runtime's stdout redirect, the complaint to another. **Pointing it at `https://api.github.com/repos/heroes-lang/heroes/issues`
takes**, measured against the real read endpoint and the fake: `ENDPOINT` becomes the issues address; the header is
`--expand-header "Authorization: Bearer {{GH_TOKEN}}"` (`--variable %GH_TOKEN`), plus `Accept:
application/vnd.github+json` and `X-GitHub-Api-Version: 2022-11-28`; **`refresh.hero` checks no HTTP status** (curl
exits 0 on a 403 or a 422: measured, `exit 0` with `403` on `/secondary`), so the verb needs `--dump-header <file>`,
`--output <file>` and `--write-out '%{http_code}\n'` (stdout), or `--fail-with-body` (exit 22 on any >=400, losing
which); it also sets **no timeout** (`--max-time` / `hero_run_limit`: curl exit 28, status `000`, measured against
`/slow`); **the JSON body must be built by the verb** (`json_string` for each field, and no object builder exists,
F7) and **the answer read** (a reader exists, which the brief did not say: `selfhost/mutate/json_read.hero`,
`read_json(text) -> Json?`, 407 lines, RFC 8259, depth-bounded; it lives under `mutate/`).
`GH_TOKEN="$(gh auth token)" curl ... https://api.github.com/repos/heroes-lang/heroes` returned `200 8302` with the
token never printed.

**Missing and too old.** `curl` missing: `started=false`, `why=2` (measured with `PATH=/nonexistent`);
`refresh.hero` turns it into `no_curl` with a sentence about *this one flag*, which would have to be generalised.
Too old: `--variable` arrived in 8.3.0; an older curl answers *option ... is unknown*, exit **2** (measured with a
bogus option on 8.7.1); `version_note` reads the words `--variable`/`--expand-header` in curl's complaint, which is
sound. I could not run a curl older than 8.3.0 (none installed): the real old-curl text is **unrun**.
`heroes doctor` checks `clang` and nothing of `curl` or `gh` (searched: `grep -n '"gh"\|"curl"'
selfhost/cli/doctor.hero`, no match).

**The 3xx and partial behaviours, measured against the fake**: a 301 on a POST without `-L` returns status 301, exit
0; with `-L`, a 307 re-POSTs (the server logged `POST /ok`); a body cut short is exit 18 with the status line already
`201`; so every route must read the status and treat "the status says created and the body is unreadable" as *created
and number unknown*.

## 4. Route D: `gh api graphql`

Measured twice. **Real GitHub, a read query** (`query { a: repository(owner:"heroes-lang", name:"heroes") { id }
b: repository(owner:"heroes-lang", name:"no-such-repo-zz") { id } rateLimit { cost remaining limit resetAt } }`,
`gh api graphql -i -f query=...`): `code=1`, HTTP **200**, headers, then
`{"data":{"a":{"id":"R_kgDOTs0D4g"},"b":null,"rateLimit":{"cost":1,...}},"errors":[{"type":"NOT_FOUND","path":["b"],...}]}`,
stderr only `gh: Could not resolve to a Repository with the name ...` (the first error's message). **So the partial
answer (`errors` beside partial `data`) is carried: in the out file, whole, and `Ran` carries only `code=1`**, the
same code as a 401 or a 404. The resource is `graphql` (`X-Ratelimit-Resource: graphql`, its own 5,000 points,
`rateLimit.cost` per query). The mutation cost, the secondary-limit weight and `createIssue`'s behaviour are
**unrun** (a write). With the fake, `--jq` is **not** applied when `errors` is present (raw body): so a partially
failed batch can only be read by parsing JSON (`json_read.hero` exists), or by re-asking. **Escaping**: variables go
by `-f name=value` / `-F name=@file` (gh builds the `variables` object), so the mutation text holds `$t0`, `$b0` and
no string literal; no GraphQL string escape is needed. Nothing here is a different *transport* from A.

## 5. Comparison, with what I ran

**Where the token comes from and what shows it** (the dummy token `FAKE0TOKEN` only; the command in each case is
`ps -o args= -p <pid>` and `ps eww -o command= -p <pid>` while the request was in flight, against the slow fake):

| route | token source | in `argv` | in the process's environment (same user, `ps eww`) | on disk |
|---|---|---|---|---|
| A, gh keyring | gh reads the keyring itself | 0 matches (`gh api -X POST /repos/o/r/issues/slow --input .../big.json`) | not there | none |
| A, `GH_TOKEN` in env | the user's shell | 0 | **1 match** | none |
| C, `curl --variable %GH_TOKEN --expand-header` | env var, or `gh auth token` | 0 (argv names the variable) | 0 for `/usr/bin/curl`, which is SIP-protected, so I read this as *the tool cannot see it*, not as *it is absent*; the same variable in `gh` showed 1 | **if `gh auth token` is spawned into the out file: mode `-rw-r--r--` (measured: `open(out, O_CREAT, 0644)`, `run.c:1037`, umask 022)**, readable by every user of the machine |
| C naive `-H "Authorization: Bearer $T"` (what the comment forbids) | | **1 match** | | |
| B libcurl | env var | 0 (`./post URL file sink`) | 1 match | the token sits in the Heroes process's memory |

Linux's `/proc/<pid>/environ` is unrun. **Route A with the keyring is the only one where the token is never in the
compiler's process, argv, environment or a file**; the cost is that any same-user process can run `gh auth token` and
get it (the disclosure at the top is that very command), which is the keyring's property and no route's.

**Failure modes, measured** (fake server unless said):

| | A (`gh`) | C (`curl`) | B (libcurl) |
|---|---|---|---|
| timeout | `hero_run_limit(2)`: code 124, no output; **the server had logged the whole 54,818-byte body** | `--max-time 2`: exit 28, `000` | `CURLOPT_TIMEOUT` (not provoked) |
| answer cut short | exit 1, stderr `unexpected EOF`, out file holds headers only | exit 18, `201` already written | rc 18 *Transferred a partial file*, status 201, sink holds `{"number": 5, "html` |
| redirect on POST | 307: exit 1, `gh: HTTP 307`; **301: gh follows it as a GET, exit 0**, the answer is a 201 from the GET, the POST having become a read | 301 without `-L`: status 301, exit 0 | 301: status 301, rc 0 |
| HTTP error | exit 1; status only with `-i` | **exit 0** unless `--fail*` | rc 0, status in `getinfo` |
| rate limit | exit 1, `Retry-After` in the `-i` file | `--dump-header` file | no header binding (item 8) |
| killed between request and answer | **the server held the full request**; the Heroes process killed with `kill -9`: `gh` was gone a second later (`pgrep -x gh` = 0; the runtime's sentinel, `run.c:943-966`, defects 437 and 465) | same mechanism | `kill -9` of the `post` process: the server held the full request, the sink file was 0 bytes |

**All three leave the same state when killed after the request is sent: GitHub holds the issue and the file does not
know.** The spawn routes add no new window (the child dies with its parent on Darwin and Linux by the runtime; the
Windows arm is documented as a job and unrun here). The reconciliation therefore cannot be client-side. Two real
reads I made: `gh api -X GET search/issues -f q='repo:heroes-lang/heroes is:issue field.defect:235' --jq .total_count`
returns `0` with exit 0 (HTTP 200) **and so does a search for any issue**, because the repository holds none: **an
unknown qualifier would also answer 0**, so whether `field.defect:` is a recognised search qualifier is **unrun**
until one issue carries the field, and search is a separate 30-per-minute resource (`X-Ratelimit-Resource: search`,
measured) and, I infer, indexed after the write. `gh api --paginate -X GET repos/heroes-lang/heroes/issues -f
state=all -f per_page=100 --jq '.[] | "\(.number)\t\(.state)"'` is the read that does not depend on an index: 21
pages for 2,014 issues on the core limit (an inference from 100 per page), and its output (empty today) is one line
per issue for a Heroes loop to match against the permalink in each body.

**Latency, measured on real reads** (`rate_limit`, five sequential calls, a still machine otherwise): `gh` 2.39 s
(0.48 s each, user+sys 0.08 s), `curl` one process each 2.01 s (0.40 s), `curl` three requests in one process
1.00 s (0.33 s each). 5,918 requests (F9) at 0.48 s is 47 minutes of latency for `gh` against 33 minutes with one
connection, an inference; the saving is a quarter of an hour in a run GitHub's secondary limits will pace anyway
(unread here). Spawn cost is not what decides between the routes.

**The C-boundary surface each adds.**

- **A**: no header, no `link`, **no new C**. `hero_run_go`'s existing `in_path`; one extern line in `process.hero`
  for `hero_run_limit`; every argument crosses `hero_run_arg(word: str)` one word at a time (the design
  `process.hero:11-25` records), so no Heroes value changes layout, ownership or NUL-termination.
- **B**: `curl/curl.h` and `stdio.h` with `link "curl"`, six modules, a home module for three handle types, a lease per
  `char *` option, seven unchecked option/value pairings, and `-lcurl` on the bootstrap line.
- **C**: none beyond what `refresh.hero` has.

## 6. What the brief should correct

1. F7 and the critic: *`process.run` hard-codes the standard-input path to empty*. True of the Heroes wrapper, **but the
   runtime takes one and the empty string means *inherit***, so `--input -` needs only a wrapper parameter. Run.
2. F7 says no JSON reader exists; `selfhost/mutate/json_read.hero` (`read_json`, 407 lines) is one, for `mutate`'s
   recovery arm. What is missing is a **builder**, and under Route A it is not needed.
3. `hero_run_limit` exists and is bound once, in `selfhost/mutate/child.hero:26`; `process.hero` does not bind it
   (searched: `grep -rn hero_run_limit selfhost`).
4. Heroes has **no string-to-integer parse** (`a[3].to_i64()` is `error[bad_operand]`); my probe called `atoll` through
   `extern "stdlib.h"`. A verb reading a milestone number from text has the same need.
5. `.github/workflows/ci.yml:918` already runs `gh release create`: `gh` is the project's own precedent outside the
   compiler; inside it, `curl` is.

## 7. What I did not run

A real rate limit (and so GitHub's actual `Retry-After` and secondary-limit text); a GraphQL **mutation**; the import
endpoint; anything that creates, so no `created_at` or `closed_at`; the callback sink under libcurl (built, not run);
`/proc/<pid>/environ` on Linux; any platform but this Mac; a `gh` older than 2.102.0 or a `curl` older than 8.7.1;
the Windows arm's child-death job; whether `field.defect:` is a recognised search qualifier.

## Files (all under `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-panel-212/ffi-pragmatist/scratch/`)

`probe.hero` (Route A harness), `fake.py` (loopback fake GitHub, plain and TLS), `post_ref.c` and `post/` (Route B,
`post/core.hero`, `opt_*.hero`, `main.hero`, `wrong/`, `cb/`), `vis.sh` (the `ps` comparison), `big.json` (54,818
bytes), `tricky.txt`.
