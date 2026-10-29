# 060: the checker's rules by churn: of 242 codes the golden cases pin, none moved its pinned output in the last week or in four of the last eight weeks, 72 moved at least once in those eight weeks, and 170 never did

Measured 2026-10-29 at M-compatibility-promise for task 2213 (https://github.com/heroes-lang/heroes/issues/2213), on the trunk at `0122c9d1c`, before the paragraph `1.0.0` rests on is written, which the task asks so that *a rule still changing every week is named before `1.0.0` promises what it refuses*. Read-only: git and the files, nothing built and nothing run but the program below, whose output is every number here.

## What is counted, and what each count stands for

**A code is a class.** Every code a golden case annotates, `#~` and `#~v` under `tests/golden/`, is one row: 242, the codes a program is told. `selfhost/diag.hero` keeps no list of them all; it lists the thesis rules, which `--permissive` drops. For each code:

- **cases** and **annotations**: the golden cases that pin it, and how many lines of them do;
- **pinned moved**: the commits since a day that changed or removed a line of an `.expected` naming `[<code>]`, with a line's places (`:12:5`, `12 |`) left out, so that a case that gained a line above its marks moves nothing, and with a case deleted whole left out, that being the corpus moving rather than the rule. Counted over the last 7 days (from 2026-10-22), the last 30 (from 2026-09-29), and the eight weeks from 2026-09-04, Friday to Thursday, the last ending on the day measured, by the weeks that held one. **This is the rule's own output moving under a program**, the column the task asks for;
- **deciding files' weeks**: the modules of `selfhost/` whose code, outside its `test` blocks and its comments, writes the code as a literal (`selfhost/diag.hero` left out, as the thesis rules' registry), and how many of the eight weeks a commit touched one of them. **A proxy, and a coarse one**: `selfhost/check/walk.hero` decides dozens of codes and moves for any of them, so it is printed beside the count that judges and judges nothing.

## What came out

- **None of the 242 moved its pinned output in the last 7 days, and none moved it in four or more of the last eight weeks.** The most any moved is in 3 weeks of 8, five codes: `expected_end_of_line`, `unclosed_bracket`, `reserved_word` and `expected_expression`, the parser's, and `ffi_union_field`.
- **72 moved at least once in the eight weeks; 170 never did.** 93 distinct commits moved one in the last 30 days: 82 whose subject opens `Defect`, 5 panel landings, 4 milestone steps, and 2 defect repairs under a batch's subject.
- **The defect repairs are, where read, what a program already refused is told**: the 10 that moved `expected_end_of_line`, read one by one, are each a repair of how a mistake is told (defects 130, 131, 177, 178, 201, 306, 329, 496 and 646: *told once*, *told as the habit it is*, *a run of stray closers told at once*). The other 74 were not read one by one, and this sentence is a sample's.
- **Of the 11 whose subject opens no `Defect`, nine are the language moving**: M-inferred-cell's `@=` (`fcec76d8d`, `5e80f5fb0`), panel 196's `counted_by`, `lent` on an `@` parameter and a buffer C fills (`6ad90cd62`, `85f8bd9dc`, `fd7fb5764`), panel 186's records over a C union (`888b6249f`, `ebc418a4d`), M-buildable-structs step 9 (`862190420`), and M-readonly-lend step 2 (`a9ada72ab`); the other two are defect repairs under another subject, batch 21's lane for defect 640, the Windows leg's rows of the C boundary told apart (`a9ee9f625`), and batch 14's gate moving defect 439's case after defect 458's (`cbda4ff87`).
- **Whether a VALID program moved is measured by the release corpora, not by this table** (M-compatibility-promise's page, 2026-10-29): `examples/` at `v0.3.0`, `v0.4.0` and `v0.5.0`, 69 programs each, checked by today's compiler, 68 of 69, the one refused `examples/interpreter/main.hero`'s `exit(f.code)`, M-panic-location's `exit(code: u8)` (`737e99535`), which task 2250 makes an `i64` again; at `v0.1.0` and `v0.2.0`, 60 of 67 refused, 58 first on panel 209's `@=`.

## What it leaves for the sitting on the specification

- **A message to a program already refused is no program's meaning**, so the promise need not freeze its words, and most of what moved is words.
- **Whether `1.0.0` promises a diagnostic's CODE** is a question, not a premise: `heroes check --json` hands codes to tools (design.md §4.17, schema 2), and an editor or a harness that reads `never_rebound` is a program of its own, whose input a renamed or a split code breaks. If it does, the 170 codes that never moved in eight weeks are where the list would start, and the five that moved in three weeks of eight are where it would wait.

## The program

The measurement's own record, run once from the repository's root as `python3 -I churn.py churn.tsv`, which wrote the table below and a list of each code's commits beside it; not a tool of the tree, which by CLAUDE.md § 10 would be a `heroes` verb or a program of the harness.

```python
"""Task 2213's inventory: every diagnostic code the golden cases pin, the
selfhost files that construct it, and how much each has moved since a day.

Run from the repository's root with `python3 -I <this file> <out.tsv>`.
Read-only: it asks git and reads files, and writes the one table it is given.
"""
import re
import subprocess
import sys
from collections import defaultdict
from datetime import date
from pathlib import Path

WEEK = "2026-10-22"   # the last 7 days, the 29th included
MONTH = "2026-09-29"  # the last 30 days
WEEKS_FROM = "2026-09-04"  # eight whole weeks, Friday to Thursday, the last ending on the 29th


def git(*args):
    # Some golden cases hold bytes that are not UTF-8 on purpose.
    return subprocess.run(["git", *args], capture_output=True, check=True).stdout.decode("utf-8", errors="replace")


def code_of(text):
    """A module's code alone: its `test` blocks and its comments left out, as
    `.claude/rules/verification.md`'s `code_of` reads one."""
    out, in_test = [], False
    for line in text.split("\n"):
        if line.startswith('test "'):
            in_test = True
            continue
        if in_test and line and not line[0] in " \t#":
            in_test = False
        if not in_test and not line.lstrip().startswith("#"):
            out.append(line)
    return "\n".join(out)


# 1. The codes the world sees: every `#~` and `#~v` annotation of a golden case.
pinned = defaultdict(int)
cases = defaultdict(set)
for path in Path("tests/golden").rglob("*.hero"):
    for m in re.finditer(r"#~v? ([a-z_]+)", path.read_text(errors="replace")):
        pinned[m.group(1)] += 1
        cases[m.group(1)].add(str(path))

# 2. The files that decide each: a selfhost module whose code, outside its
# tests and comments, writes the code as a literal. `diag.hero` is the
# registry that lists the thesis rules, not a rule, and is left out.
modules = {}
for path in Path("selfhost").rglob("*.hero"):
    if str(path) == "selfhost/diag.hero":
        continue
    modules[str(path)] = code_of(path.read_text(errors="replace"))
deciding = {c: sorted(p for p, body in modules.items() if f'"{c}"' in body) for c in pinned}

# 3. The commits since each day that touched a deciding file.
def commits_touching(files, since):
    if not files:
        return []
    return git("log", f"--since={since} 00:00", "--format=%h %cs", "--", *files).split()[::2]

def weeks_touching(files):
    if not files:
        return 0
    days = git("log", f"--since={WEEKS_FROM} 00:00", "--format=%cs", "--", *files).split()
    start = date.fromisoformat(WEEKS_FROM)
    return len({(date.fromisoformat(d) - start).days // 7 for d in days})

# 4. The commits since each day that CHANGED OR REMOVED a pinned line of the
# code's output, a `-` line of an `.expected` naming `[<code>]`: the rule's
# behaviour moved under a program. A case only added moves nothing.
moved = {WEEK: defaultdict(set), MONTH: defaultdict(set), WEEKS_FROM: defaultdict(set)}
moved_weeks = defaultdict(set)
log = git("log", f"--since={WEEKS_FROM} 00:00", "--format=COMMIT %h %cs", "-p", "--unified=0",
          "--", "tests/golden")
start = date.fromisoformat(WEEKS_FROM)

# A line's text without its places: `path:12:5:` and `at path:12:5` and the
# excerpt's `12 |` move when a case gains a line above, and the rule has not.
def unplaced(text):
    text = re.sub(r":\d+:\d+", ":L:C", text)
    return re.sub(r"^(\s*)\d+ \|", r"\1L |", text)

# Per commit and per file: the lines removed and added, and whether the file
# was deleted, which is a case leaving the corpus rather than a rule moving.
def settle(commit, day, files):
    for removed, added, deleted in files.values():
        if deleted:
            continue
        kept = {unplaced(a) for a in added}
        for r in removed:
            if unplaced(r) in kept:
                continue
            for m in re.finditer(r"\[([a-z_]+)\]", r):
                c = m.group(1)
                if c not in pinned:
                    continue
                for since in moved:
                    if day >= since:
                        moved[since][c].add(commit)
                moved_weeks[c].add((date.fromisoformat(day) - start).days // 7)

commit, day, files, current = None, None, {}, None
for line in log.split("\n"):
    if line.startswith("COMMIT "):
        if commit:
            settle(commit, day, files)
        _, commit, day = line.split()
        files, current = {}, None
    elif line.startswith("diff --git "):
        current = line.split(" b/", 1)[1]
        files[current] = ([], [], False)
    elif line.startswith("+++ /dev/null") and current:
        files[current] = (files[current][0], files[current][1], True)
    elif line.startswith("--- ") or line.startswith("+++ "):
        continue
    elif current and current.endswith(".expected") and line.startswith("-"):
        files[current][0].append(line[1:])
    elif current and current.endswith(".expected") and line.startswith("+"):
        files[current][1].append(line[1:])
if commit:
    settle(commit, day, files)

rows = []
for c in pinned:
    files = deciding[c]
    rows.append((
        c, len(cases[c]), pinned[c], len(files),
        len(commits_touching(files, WEEK)), len(commits_touching(files, MONTH)), weeks_touching(files),
        len(moved[WEEK][c]), len(moved[MONTH][c]), len(moved_weeks[c]),
        " ".join(files),
    ))
rows.sort(key=lambda r: (-r[9], -r[8], -r[6], r[0]))

head = ("code", "cases", "annotations", "deciding_files", "file_commits_7d", "file_commits_30d",
        "file_weeks_of_8", "pinned_moved_7d", "pinned_moved_30d", "pinned_weeks_of_8", "files")
with open(sys.argv[1], "w") as out:
    out.write("\t".join(head) + "\n")
    for r in rows:
        out.write("\t".join(str(x) for x in r) + "\n")
with open(sys.argv[1].replace(".tsv", "-commits.tsv"), "w") as out:
    for c in sorted(pinned):
        if moved[MONTH][c]:
            out.write(c + "\t" + " ".join(sorted(moved[MONTH][c])) + "\n")
print(len(rows), "codes written")
```

## The 72 that moved in eight weeks

`pinned moved` over the last 30 days, the weeks of eight that held one, and the deciding files' weeks beside them as the proxy said above.

| code | cases | annotations | pinned moved, 30 days | weeks of 8 | deciding files' weeks of 8 |
|---|---|---|---|---|---|
| `expected_end_of_line` | 109 | 468 | 10 | 3 | 4 |
| `unclosed_bracket` | 33 | 140 | 7 | 3 | 3 |
| `reserved_word` | 62 | 195 | 6 | 3 | 7 |
| `expected_expression` | 65 | 185 | 6 | 3 | 5 |
| `ffi_union_field` | 14 | 14 | 5 | 3 | 5 |
| `expected_separator` | 15 | 48 | 5 | 2 | 3 |
| `ffi_constant_type` | 37 | 62 | 4 | 2 | 7 |
| `missing_body` | 52 | 133 | 3 | 2 | 5 |
| `never_rebound` | 27 | 46 | 3 | 2 | 5 |
| `not_mutable` | 6 | 11 | 3 | 2 | 5 |
| `type_mismatch` | 43 | 83 | 3 | 2 | 5 |
| `unexpected_block` | 13 | 67 | 3 | 2 | 3 |
| `end_lease_of_no_lease` | 2 | 4 | 3 | 2 | 2 |
| `lent_shape` | 2 | 4 | 2 | 2 | 7 |
| `ffi_return_type` | 22 | 130 | 2 | 2 | 5 |
| `expected_declaration` | 13 | 36 | 2 | 2 | 4 |
| `needs_label` | 8 | 23 | 2 | 2 | 4 |
| `expected_case` | 8 | 15 | 2 | 2 | 3 |
| `missing_match_arms` | 11 | 43 | 2 | 2 | 3 |
| `extern_across_modules` | 3 | 8 | 1 | 2 | 5 |
| `unexpected_character` | 43 | 142 | 3 | 1 | 7 |
| `counted_by_shape` | 4 | 13 | 3 | 1 | 5 |
| `cannot_infer` | 17 | 60 | 3 | 1 | 4 |
| `expected_function_type_params_close` | 5 | 37 | 2 | 1 | 6 |
| `bad_operand` | 16 | 39 | 2 | 1 | 5 |
| `expected_field_name` | 4 | 26 | 2 | 1 | 5 |
| `fixed_outside_a_group` | 11 | 63 | 2 | 1 | 5 |
| `continuation_outside_brackets` | 61 | 269 | 2 | 1 | 3 |
| `unknown_escape` | 17 | 54 | 2 | 1 | 3 |
| `expected_binding_symbol` | 1 | 1 | 2 | 1 | 2 |
| `undefined_header_name` | 1 | 2 | 2 | 1 | 2 |
| `ffi_const_field` | 2 | 3 | 1 | 1 | 7 |
| `ffi_field_type` | 18 | 27 | 1 | 1 | 7 |
| `ffi_parameter_type` | 51 | 70 | 1 | 1 | 7 |
| `ffi_unknown_field` | 13 | 15 | 1 | 1 | 7 |
| `expected_function_type` | 7 | 12 | 1 | 1 | 6 |
| `expected_module_name` | 2 | 2 | 1 | 1 | 6 |
| `expected_type` | 15 | 36 | 1 | 1 | 6 |
| `ffi_missing_link` | 5 | 5 | 1 | 1 | 6 |
| `missing_fields` | 10 | 11 | 1 | 1 | 6 |
| `nested_read` | 2 | 3 | 1 | 1 | 6 |
| `no_value` | 10 | 40 | 1 | 1 | 6 |
| `aliased_mutable_arguments` | 5 | 15 | 1 | 1 | 5 |
| `ffi_unknown_name` | 24 | 24 | 1 | 1 | 5 |
| `fixed_array_length` | 2 | 2 | 1 | 1 | 5 |
| `not_a_type` | 1 | 1 | 1 | 1 | 5 |
| `owned_needs_cstr` | 3 | 5 | 1 | 1 | 5 |
| `cell_cannot_end` | 2 | 3 | 1 | 1 | 4 |
| `expected_field` | 3 | 8 | 1 | 1 | 4 |
| `missing_label` | 5 | 10 | 1 | 1 | 4 |
| `not_callable` | 3 | 4 | 1 | 1 | 4 |
| `unmarked_handle_producer` | 4 | 8 | 1 | 1 | 4 |
| `wrong_label` | 19 | 58 | 1 | 1 | 4 |
| `contract_differs` | 4 | 4 | 1 | 1 | 3 |
| `empty_variant` | 4 | 4 | 1 | 1 | 3 |
| `expected_args_close` | 9 | 18 | 1 | 1 | 3 |
| `expected_arm_arrow` | 7 | 29 | 1 | 1 | 3 |
| `expected_map_entry_colon` | 5 | 27 | 1 | 1 | 3 |
| `for_missing_in` | 17 | 74 | 1 | 1 | 3 |
| `hole_not_renderable` | 2 | 4 | 1 | 1 | 3 |
| `line_end_before_continuation` | 30 | 50 | 1 | 1 | 3 |
| `spaced_minus_element` | 9 | 35 | 1 | 1 | 3 |
| `escape_in_header_name` | 2 | 11 | 1 | 1 | 2 |
| `machine_locked_path` | 3 | 10 | 1 | 1 | 2 |
| `package_comparison` | 1 | 2 | 1 | 1 | 2 |
| `tab_in_indentation` | 5 | 33 | 1 | 1 | 2 |
| `unread_releaser` | 5 | 8 | 1 | 1 | 2 |
| `unshowable_name` | 2 | 3 | 1 | 1 | 2 |
| `duplicate_tag` | 1 | 2 | 0 | 1 | 5 |
| `ffi_type` | 7 | 9 | 0 | 1 | 5 |
| `label_on_function_value` | 1 | 1 | 0 | 1 | 4 |
| `expected_pattern` | 18 | 70 | 0 | 1 | 3 |

## The 170 that never moved in eight weeks

`base_prefix_case`, `bound_unit`, `builtin_as_value`, `builtin_name_taken`, `callback_name`, `cell_redeclared`, `char_literal`, `constant_body`, `constant_cycle`, `consumed_borrowed_handle`, `cstr_escapes`, `cstr_in_a_record`, `cstr_out_of_heroes`, `declaration_in_arm`, `declared_place`, `declared_twice`, `digit_not_in_base`, `discarded_failure`, `discarded_value`, `division_by_zero`, `dot_on_a_record_name`, `duplicate_arm`, `duplicate_pattern_binding`, `empty_base_literal`, `empty_record`, `endless_recursion`, `escape_not_needed`, `escape_spelling`, `expected_alias_name`, `expected_case_name`, `expected_constant_type`, `expected_extent`, `expected_extern_block`, `expected_extern_header`, `expected_extern_signature`, `expected_field_type`, `expected_freer`, `expected_group_close`, `expected_name`, `expected_parameter`, `expected_parameter_type`, `expected_params`, `expected_params_close`, `expected_releaser`, `expected_success_value`, `expected_test_name`, `exponent_literal`, `extern_has_body`, `ffi_call_shape`, `ffi_callback_type`, `ffi_declared_two_ways`, `ffi_defined_twice`, `ffi_freer_type`, `ffi_header_refused`, `ffi_incomplete_record`, `ffi_lock_copied`, `ffi_macro_name`, `ffi_missing_header`, `ffi_not_constant`, `ffi_one_file`, `ffi_owned_const_cell`, `ffi_package`, `ffi_partial_operation`, `ffi_tag_is_a_union`, `ffi_unavailable`, `ffi_unknown_tag`, `ffi_writable_parameter`, `field_lend_escapes`, `field_lend_extent`, `field_lend_needs_a_header`, `field_lend_needs_a_place`, `field_lend_uncounted`, `field_lend_written`, `fixed_element`, `fixed_flow`, `fixed_index_out_of_range`, `float_map_key`, `freer_arity`, `generic_equality`, `handle_built`, `handle_map_key`, `handle_used_after_end`, `hole_without_f`, `indentation_jump`, `indentation_not_multiple_of_`, `int_out_of_range`, `invisible_character`, `jump_outside_loop`, `leading_zero`, `lease_escapes`, `lease_written`, `lend_kept`, `lend_needs_a_header`, `lent_parameter_as_value`, `lone_brace`, `main_parameters`, `main_returns`, `marker_mismatch`, `misplaced_mutable_marker`, `misplaced_separator`, `missing_return`, `mixed_arithmetic`, `module_path_climbs`, `module_path_has_no_parts`, `module_path_wants_a_part`, `mutable_parameter_as_value`, `named_parameter_in_function_type`, `needs_parameter_names`, `needs_qualifying`, `nested_fallible`, `no_mutable_globals`, `no_size`, `no_such_case`, `no_such_field`, `non_exhaustive`, `not_a_place`, `not_a_record_name`, `not_iterable`, `not_matchable`, `not_text`, `nul_in_string`, `old_cell_symbol`, `one_mark_per_position`, `option_like_name`, `owned_freer_called`, `parameter_name_not_a_name`, `pointer_element`, `polymorphic_recursion`, `raw_carriage_return`, `raw_tab`, `record_lend_extent`, `record_name_alone`, `repeated_mark`, `rest_not_a_construction`, `rest_not_last`, `rest_outside_a_group`, `shadowed_binding`, `str_element_write`, `stray_carriage_return`, `success_needs_result`, `success_on_parameter`, `success_value_range`, `success_value_type`, `success_without_end`, `tab_in_line`, `trailing_colon`, `trailing_comma`, `transfer_without_receiver`, `try_in_infallible`, `ufcs_on_mutable`, `unit_element`, `unit_field`, `unit_parameter`, `unknown_freer`, `unknown_function`, `unknown_in_module`, `unknown_name`, `unknown_type`, `unordered_element`, `unreachable_statement`, `unread_mark`, `unterminated_char`, `unterminated_string`, `unused_binding`, `unwritable_name`, `use_wants_a_name`, `variant_in_value_position`, `when_on_void`, `wildcard_on_variant`, `wrong_arity`.
