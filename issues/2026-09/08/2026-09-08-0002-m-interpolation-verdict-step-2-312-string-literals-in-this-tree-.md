- [ ] **M-interpolation-verdict step 2** | 312 string literals in this tree hold a brace and 24 of them look exactly like a hole. Work out, from the spec, whether those 24 would break loudly or quietly | `examples/template/main.hero:145-235` · `spec/heroes-spec.md` § Bindings, § Files and layout | the difference between a loud and a quiet migration is the whole of §1.12, and here it is decided by three rules a reader can check

    **Origin:** M-interpolation-verdict step 2, 2026-09-08.
    **The setup:** all 24 are in `examples/template/main.hero`, which
    implements `{key}` substitution by hand, and they read `"Hello, {name}."`,
    `"{name} the {role}"`, `"{anything}"`. Suppose `{n}` became a hole naming a
    binding. Say whether each of the 24 would still compile.
    **The question after:** the answer rests on three separate rules of the
    language, not one. Name them, and then name the one thing that would have
    to be true of that file for the answer to flip to quiet.
