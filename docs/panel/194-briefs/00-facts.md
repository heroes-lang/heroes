# Panel 194, facts measured by the coordinator on 2026-10-06 between 11:32 and 11:39 by the clock read before and after, each with its command

Tree: worktree lane-panel-194 at bef739dd (batch 12's round, before panel 178's merge), its compiler built
from the round's regenerated seed (sha256 8d9cc9f4b1375ee1, fixpoint verified 09:54).

1. Since panel 178's base: `git rev-list --count 517b8e25..bef739dd` = 1020 commits (517b8e25 is
   `git merge-base main lane-panel-178`, 2026-09-25). `git log --oneline 517b8e25..bef739dd -- <f> | wc -l`:
   spec/heroes-spec.md 11, selfhost/check/walk.hero 19, selfhost/print/fmt.hero 9, selfhost/ast.hero 2,
   selfhost/emit/container.hero 2, selfhost/emit/ctype.hero 1.
2. The spec: `heroes measure spec/heroes-spec.md`: real 9518 (claude-opus-5, pinned 2026-10-06), vendored
   maximum 7212; headroom 722 against 10240, of which the FFI floor mortgages 60 (panel 030 R3), so 662.
   § 13 today (`sed -n 364,371p spec/heroes-spec.md`): a group's record is the header's struct; a field may be
   a fixed array `i32[4]`, built with `[a, b, c, d]`, as many elements as the type says; a bit-field is left
   to `partial`. `grep -n rest spec/heroes-spec.md`: no `rest`. No sentence says C's `char` is `i8`
   (`grep -n '`i8`' spec/heroes-spec.md`: the type table only).
   § 13 also says `s.cstr()` aborts on a `str` holding a zero byte (line 386-387).
3. Layout, the instrument's unit (`code_lines` of tests/harness/suite_layout.hero, its DECIDED table):
   selfhost/check/walk.hero 1858 of 1870 (12 left), selfhost/print/fmt.hero 1096 of 1175 (79 left),
   selfhost/ast.hero 550 of 550 (none left). `heroes run tests/harness/main.hero -- ./heroes layout`: 5 passed.
4. `missing_fields`: emitted at selfhost/check/walk.hero:1986 (`data_errors.missing_fields`,
   selfhost/data_errors.hero:57). `grep -rl missing_fields tests/golden`: 18 files under
   tests/golden/unsupported/ (group records: e.g. ffi-a-construction-of-a-plain-struct-leaves-out-a-field),
   1 under tests/golden/run/ (a comment), none under tests/golden/check/: no golden pins it on a Heroes record.
5. The header census, panel 178's `census.sh` (lane-panel-178:docs/panel/178-briefs/census.sh, its TU path made
   a variable), 35 headers:
   | leg | fields | >8 | >=64 | records >8 | public |
   | Darwin arm64, Apple clang 21.0.0, `-I/opt/homebrew/include` | 108 | 81 | 27 | 62 | 42 |
   | Linux arm64, heroes-linux-arm64, Debian clang 22.1.8 | 77 | 50 | 14 | 34 | 29 |
   | Linux x86-64 | unrun: no amd64 image on this machine (`docker run --platform linux/amd64 heroes-linux` refused) |
   Panel 178's table read 108/81/27/61/42 and 77/50/14/34/29 (and 23 public on x86-64, 2026-09-24, carried,
   not re-run). On Darwin today's clang does not search /opt/homebrew/include by default: without the flag
   openssl/sha.h and openssl/hmac.h are missing and the public count is 39 (SHA256state_st, SHA512state_st,
   SHAstate_st, evp_cipher_info_st absent); Homebrew's openssl is 4.0.3 (`brew list --versions`).
6. Defects 091 to 094: re-run by the agent bringing 178 in (its report); 093 looks answered by defect 245's
   repair (spec § 13's `cstr` abort sentence), unproven until run.
